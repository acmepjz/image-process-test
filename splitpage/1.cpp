#include <FreeImage.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <string>
#include <vector>
#include <algorithm>

static FIBITMAP* loadImage(const char* filename, int flags) {
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(filename, 0);

	if (fif == FIF_UNKNOWN) fif = FreeImage_GetFIFFromFilename(filename);

	FIBITMAP* dib = NULL;

	if ((fif == FIF_UNKNOWN)
		|| !FreeImage_FIFSupportsReading(fif)
		|| (dib = FreeImage_Load(fif, filename, flags)) == NULL) {
		printf("Failed to load '%s'.\n", filename);
	}

	return dib;
}

static void saveImage(const char* filename, FIBITMAP* bm, int flags) {
	FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(filename);

	// ad-hoc!!!
	if (fif == FIF_JPEG && flags == 0) {
		flags = 95;
	}

	if ((fif == FIF_UNKNOWN)
		|| !FreeImage_FIFSupportsWriting(fif)
		|| !FreeImage_Save(fif, bm, filename, flags)) {
		printf("Failed to save '%s'.\n", filename);
	}
}

class SplitPage {
public:
	int startX;
	int endX;
	std::string inputFile;
	std::string outputFile;
	std::string algorithm;
	int outputWidth;
	bool inverted;

	SplitPage();
	void run();

private:
};

SplitPage::SplitPage()
	: startX(-1)
	, endX(-1)
	, outputWidth(-1)
	, inverted(false)
{
}

void SplitPage::run() {
	FIBITMAP *input, *tmp;

	tmp = loadImage(inputFile.c_str(), 0);
	if (!tmp) return;

	const int w = FreeImage_GetWidth(tmp);
	const int h = FreeImage_GetHeight(tmp);

	input = FreeImage_ConvertToGreyscale(tmp);
	FreeImage_Unload(tmp);

	if (startX < 0) startX = (w + 2) / 4;
	if (startX > w) startX = w;
	if (endX < 0) endX = (3 * w + 2) / 4;
	if (endX > w) endX = w;

	int resultX = startX;

	if (endX > startX) {
		if (algorithm == "max_average") {
			int xx = 0;
			for (int y = 0; y < h; y++) {
				unsigned char *scanline = FreeImage_GetScanLine(input, y);
				int mx = startX;
				unsigned char mv = scanline[startX];
				if (inverted) {
					for (int x = startX + 1; x < endX; x++) {
						if (scanline[x] > mv) {
							mx = x;
							mv = scanline[x];
						}
					}
				} else {
					for (int x = startX + 1; x < endX; x++) {
						if (scanline[x] < mv) {
							mx = x;
							mv = scanline[x];
						}
					}
				}
				xx += mx - startX;
			}
			resultX += (xx + h / 2) / h;
		} else if (algorithm == "max_max") {
			std::vector<int> hist;
			hist.resize(endX - startX, 0);
			for (int y = 0; y < h; y++) {
				unsigned char *scanline = FreeImage_GetScanLine(input, y);
				int mx = startX;
				unsigned char mv = scanline[startX];
				if (inverted) {
					for (int x = startX + 1; x < endX; x++) {
						if (scanline[x] > mv) {
							mx = x;
							mv = scanline[x];
						}
					}
				} else {
					for (int x = startX + 1; x < endX; x++) {
						if (scanline[x] < mv) {
							mx = x;
							mv = scanline[x];
						}
					}
				}
				hist[mx - startX]++;
			}
			resultX += std::max_element(hist.begin(), hist.end()) - hist.begin();
		} else {
			if (!algorithm.empty() && algorithm != "max") {
				fprintf(stderr, "unknown algorithm: '%s'\n", algorithm.c_str());
			}
			std::vector<int> hist;
			hist.resize(endX - startX, 0);
			for (int y = 0; y < h; y++) {
				unsigned char *scanline = FreeImage_GetScanLine(input, y);
				for (int x = startX; x < endX; x++) {
					hist[x - startX] += scanline[x];
				}
			}
			resultX += (inverted ? std::max_element(hist.begin(), hist.end()) : std::min_element(hist.begin(), hist.end())) - hist.begin();
		}
	}

	size_t lps = std::string::npos;

	if (!outputFile.empty()) {
		lps = outputFile.find_first_of('*');
		if (lps == std::string::npos) {
			lps = outputFile.find_last_of('.');
			if (lps == std::string::npos) {
				lps = outputFile.size();
				outputFile += "*";
			} else {
				outputFile = outputFile.substr(0, lps) + "*" + outputFile.substr(lps);
			}
		}
	}

	if (lps == std::string::npos) {
		printf("%d\n", resultX);
		return;
	}

	RGBQUAD bg = inverted ? RGBQUAD{ 0, 0, 0, 0 } : RGBQUAD{ 255, 255, 255, 0 };

	int w1 = outputWidth > 0 ? outputWidth : resultX;
	tmp = FreeImage_EnlargeCanvas(input, w1 - resultX, 0, resultX - w, 0, &bg, FI_COLOR_IS_RGB_COLOR);
	saveImage((outputFile.substr(0, lps) + "_1L" + outputFile.substr(lps + 1)).c_str(), tmp, 0);
	FreeImage_Unload(tmp);

	w1 = outputWidth > 0 ? outputWidth : w - resultX;
	tmp = FreeImage_EnlargeCanvas(input, -resultX, 0, resultX + w1 - w, 0, &bg, FI_COLOR_IS_RGB_COLOR);
	saveImage((outputFile.substr(0, lps) + "_2R" + outputFile.substr(lps + 1)).c_str(), tmp, 0);
	FreeImage_Unload(tmp);
}

void usage() {
	puts(
		"Usage: splitpage [options] <input>\n"
		"Options:\n"
		"  -r, --range <sx>,<ex>  specify the region to be processed\n"
		"                         (default 25% of width to 75% of width)\n"
		"  -o, --outout <file>    output image files (will be converted to grayscale)\n"
		"                         ('*' in file name will be replaced by 'L' or 'R')\n"
		"  -w, --width <w>        width of the output image files\n"
		"  -i, --inverted         input image is inverted\n"
		"  -a, --algorithm <alg>  algorithm: 'max' (default), 'max_average',\n"
		"                         'max_max', 'least_square', 'hough'\n"
		"\n"
		"The program output the x coordinate of split point to stdout by default.\n"
		);
	exit(1);
}

int main(int argc, char** argv) {
	SplitPage splitpage;
	for (int i = 1; i < argc; i++) {
		std::string s = argv[i];
		if (!s.empty() && s[0] == '-') {
			if (s == "-o" || s == "--output") {
				if (++i >= argc) usage();
				splitpage.outputFile = argv[i];
			} else if (s == "-a" || s == "--algorithm") {
				if (++i >= argc) usage();
				splitpage.algorithm = argv[i];
			} else if (s == "-w" || s == "--width") {
				if (++i >= argc) usage();
				char *end;
				splitpage.outputWidth = strtol(argv[i], &end, 10);
				if (*end) usage();
			} else if (s == "-r" || s == "--range") {
				if (++i >= argc) usage();
				if (sscanf(argv[i], "%d,%d", &splitpage.startX, &splitpage.endX) != 2) usage();
			} else if (s == "-i" || s == "--inverted") {
				splitpage.inverted = true;
			} else {
				usage();
			}
		} else if (splitpage.inputFile.empty()) {
			splitpage.inputFile = s;
			break;
		}
	}
	if (splitpage.inputFile.empty()) usage();
	FreeImage_Initialise();
	splitpage.run();
	FreeImage_DeInitialise();
	return 0;
}
