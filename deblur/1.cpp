#include <FreeImage.h>
#include <fftw3.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <string>

class Deblur {
public:
	Deblur();
public:
	std::string inputFile;
	std::string outputFile;
	std::string kernelFile;
	int maxKernelSize;
	int rx, ry, rw, rh;
	double lambda_;
	double gamma_;
	double sigma_;
	double beta_max;
	double mu_max;
	double lambda_min;
	double lambda_step;

	void run();
private:
	void deblur(const double *yImage, double *kImage, int width, int height, int maxSize);

	// The following are reused during calculation
	double *xImage; // reconstructed image
	double *tempImage;
	fftw_complex *nabla_h_fft, *nabla_v_fft, *yImage_fft;
	fftw_complex *kImage_fft, *uImage_fft, *temp3_fft, *xImage_fft;
	double *nabla_norm, *temp_denominator;
};

Deblur::Deblur()
	: maxKernelSize(0)
	, rx(0), ry(0), rw(0), rh(0)
	, lambda_(0.004), gamma_(2.0), sigma_(1.0)
	, beta_max(8.0), mu_max(1E+5), lambda_min(1E-4), lambda_step(0.9)
{
}

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

static void saveImage2(const char* filename, const double* data, int width, int height, int xshift, int yshift, bool normalize, int flags) {
	double maxValue = 1.0;

	if (normalize) {
		maxValue = 0.0;
		for (int i = 0, m = width*height; i < m; i++) {
			if (data[i] > maxValue) maxValue = data[i];
		}
	}

	FIBITMAP *bm = FreeImage_Allocate(width, height, 8);

	int x11 = xshift, y1 = yshift;
	if (x11 < 0) x11 += width;
	if (y1 < 0) y1 += height;
	for (int y = 0; y < height; y++) {
		unsigned char *scanline = FreeImage_GetScanLine(bm, y1);
		int x1 = x11;
		for (int x = 0; x < width; x++) {
			double value = *(data)++;
			scanline[x1] = (value <= 0.0) ? 0 : (value >= maxValue) ? 255 : (unsigned char)(value * 255.0 / maxValue + 0.5);
			x1++;
			if (x1 >= width) x1 -= width;
		}
		y1++;
		if (y1 >= height) y1 -= height;
	}

	saveImage(filename, bm, flags);
	FreeImage_Unload(bm);
}

// width, height: size of input, assumed to be even
static void pyrDown(const double* input, double* output, int width, int height) {
	int w2 = width / 2, h2 = height / 2;
	for (int y = 0; y < h2; y++) {
		for (int x = 0; x < w2; x++) {
			*(output++) = (input[0] + input[1] + input[width] + input[width + 1]) * 0.25;
			input += 2;
		}
		input += width;
	}
}

// width, height: size of input
static void pyrUp(const double* input, double* output, int width, int height) {
	const double *scanline0 = input;
	for (int y = 0; y < height; y++) {
		const double *scanline1 = (y < height - 1) ? (scanline0 + width) : input;
		for (int x = 0; x < width; x++) {
			int x2 = (x < width - 1) ? (x + 1) : 0;
			double tmp0, tmp1, tmp2;
			output[0] = tmp0 = scanline0[x];
			output[1] = tmp1 = (tmp0 + scanline0[x2]) * 0.5;
			output[width * 2] = tmp2 = (tmp0 + scanline1[x]) * 0.5;
			output[width * 2 + 1] = (tmp1 + tmp2) * 0.5;
			output += 2;
		}
		output += width * 2;
		scanline0 = scanline1;
	}
}

void Deblur::run() {
	FIBITMAP *input, *tmp;
	
	tmp = loadImage(inputFile.c_str(), 0);
	if (!tmp) return;

	const int w = FreeImage_GetWidth(tmp);
	const int h = FreeImage_GetHeight(tmp);
	if (rw <= 0) rw = w;
	if (rh <= 0) rh = h;
	if ((rw & (rw - 1)) || (rh & (rh - 1))) {
		FreeImage_Unload(tmp);
		printf("Image size is not a power of two\n");
		return;
	}

	input = FreeImage_ConvertToGreyscale(tmp);
	FreeImage_Unload(tmp);

	const int m = rw * rh;
	const int m2 = (rw / 2 + 1) * rh;

	// load the region of image
	double *yImage = fftw_alloc_real(m); // blurred image
	double *lp = yImage;
	for (int y = 0; y < rh; y++) {
		int y0 = y + ry;
		y0 = y0 < 0 ? 0 : (y0 >= h) ? (h - 1) : y0;
		unsigned char *scanline = FreeImage_GetScanLine(input, y0);
		for (int x = 0; x < rw; x++) {
			int x0 = x + rx;
			x0 = x0 < 0 ? 0 : (x0 >= w) ? (w - 1) : x0;
			*(lp++) = scanline[x0] / 255.0;
		}
	}
	FreeImage_Unload(input);

	xImage = fftw_alloc_real(m); // reconstructed image
	double *kImage = fftw_alloc_real(m); // estimated kernel

	tempImage = fftw_alloc_real(m);

	nabla_h_fft = fftw_alloc_complex(m2);
	nabla_v_fft = fftw_alloc_complex(m2);
	yImage_fft = fftw_alloc_complex(m2);
	kImage_fft = fftw_alloc_complex(m2);
	uImage_fft = fftw_alloc_complex(m2);
	temp3_fft = fftw_alloc_complex(m2);
	xImage_fft = fftw_alloc_complex(m2);

	nabla_norm = fftw_alloc_real(m2);
	temp_denominator = fftw_alloc_real(m2);

	if (sigma_ < 1E-4) beta_max = -1.0;

	deblur(yImage, kImage, rw, rh, maxKernelSize);

	fftw_free(yImage);
	fftw_free(tempImage);

	fftw_free(nabla_h_fft);
	fftw_free(nabla_v_fft);
	fftw_free(yImage_fft);
	fftw_free(kImage_fft);
	fftw_free(uImage_fft);
	fftw_free(temp3_fft);

	fftw_free(nabla_norm);
	fftw_free(temp_denominator);

	if (!outputFile.empty()) {
		fftw_plan temp_plan = fftw_plan_dft_c2r_2d(rh, rw, xImage_fft, xImage, FFTW_ESTIMATE);
		fftw_execute(temp_plan);
		fftw_destroy_plan(temp_plan);
		const double d = 1.0 / m;
		for (int i = 0; i < m; i++) xImage[i] *= d;
		saveImage2(outputFile.c_str(), xImage, rw, rh, 0, 0, false, 0);
	}

	fftw_free(xImage);
	fftw_free(xImage_fft);

	if (!kernelFile.empty()) {
		saveImage2(kernelFile.c_str(), kImage, rw, rh, rw / 2, rh / 2, true, 0);
	}

	fftw_free(kImage);
}

#define REAL_ABAR_MUL_B(A,B) (A[i][0] * B[i][0] + A[i][1] * B[i][1])
#define IMAG_ABAR_MUL_B(A,B) (A[i][0] * B[i][1] - A[i][1] * B[i][0])
#define NORM_A(A) REAL_ABAR_MUL_B(A,A)

void Deblur::deblur(const double *yImage, double *kImage, int width, int height, int maxSize) {
	const int m = width * height;
	const int m2 = (width / 2 + 1) * height;

	if (width >= 32 && height >= 32 && maxSize != 1) {
		// estimate the kernel from coarser level
		int w2 = width / 2, h2 = height / 2;
		double *yImage2 = fftw_alloc_real(w2 * h2); // blurred image
		double *kImage2 = fftw_alloc_real(w2 * h2); // estimated kernel

		pyrDown(yImage, yImage2, width, height);
		deblur(yImage2, kImage2, w2, h2, (maxSize + 1) / 2);
		pyrUp(kImage2, kImage, w2, h2);

		fftw_free(yImage2);
		fftw_free(kImage2);
	} else {
		// use identity
		kImage[0] = 1.0;
		for (int i = 1; i < m; i++) {
			kImage[i] = 0.0;
		}
	}

	// TODO: should use the analytic expression of the FFT of differential operators
	fftw_plan temp_plan;

	memset(tempImage, 0, m*sizeof(double));
	tempImage[0] = -1.0; tempImage[width - 1] = 1.0;
	temp_plan = fftw_plan_dft_r2c_2d(height, width, tempImage, nabla_h_fft, FFTW_ESTIMATE);
	fftw_execute(temp_plan);
	fftw_destroy_plan(temp_plan);

	memset(tempImage, 0, m*sizeof(double));
	tempImage[0] = -1.0; tempImage[m - width] = 1.0;
	temp_plan = fftw_plan_dft_r2c_2d(height, width, tempImage, nabla_v_fft, FFTW_ESTIMATE);
	fftw_execute(temp_plan);
	fftw_destroy_plan(temp_plan);

	// precompute norm
	for (int i = 0; i < m2; i++) {
		nabla_norm[i] = NORM_A(nabla_h_fft) + NORM_A(nabla_v_fft);
	}

	// precompute FFT(y)
	temp_plan = fftw_plan_dft_r2c_2d(height, width, const_cast<double*>(yImage), yImage_fft, FFTW_ESTIMATE);
	fftw_execute(temp_plan);
	fftw_destroy_plan(temp_plan);

	double lambda = lambda_;

	for (int t = 0; t < 5; t++) {
		// precompute FFT(k)
		temp_plan = fftw_plan_dft_r2c_2d(height, width, kImage, kImage_fft, FFTW_ESTIMATE);
		fftw_execute(temp_plan);
		fftw_destroy_plan(temp_plan);

		// --- solve xImage ---
		memcpy(xImage, yImage, m*sizeof(double));
		const double threshold0 = 0.75; // TODO: Otsu thresholding
		double beta = lambda*sigma_ / (threshold0 * threshold0);

		for (;;) {
			const double beta_new = beta * 2.0;

			// solve u
			const double threshold = sqrt(lambda*sigma_ / beta);
			for (int i = 0; i < m; i++) {
				double d = xImage[i];
				tempImage[i] = (d < -threshold || d > threshold) ? d : 0.0;
				// tempImage[i] = (d < threshold) ? 0.0 : (d > 1.0 - threshold) ? 1.0 : d; // test only
			}

			// precompte FFT(u)
			temp_plan = fftw_plan_dft_r2c_2d(height, width, tempImage, uImage_fft, FFTW_ESTIMATE);
			fftw_execute(temp_plan);
			fftw_destroy_plan(temp_plan);

			// precompute numerator and denominator
			for (int i = 0; i < m2; i++) {
				// numerator
				uImage_fft[i][0] = REAL_ABAR_MUL_B(kImage_fft, yImage_fft) + beta * uImage_fft[i][0];
				uImage_fft[i][1] = IMAG_ABAR_MUL_B(kImage_fft, yImage_fft) + beta * uImage_fft[i][1];
				// denominator
				temp_denominator[i] = NORM_A(kImage_fft) + beta;
			}

			double mu = 2.0 * lambda;

			for (;;) {
				const double mu_new = 2.0 * mu;

				const double grad_threshold = sqrt(lambda / mu);

				// solve g_h
				double *lp = tempImage;
				double *scanline = xImage;
				for (int y = 0; y < height; y++) {
					for (int x = 0; x < width; x++) {
						int x2 = (x < width - 1) ? (x + 1) : 0;
						double grad = scanline[x2] - scanline[x];
						*(lp++) = (grad < -grad_threshold || grad > grad_threshold) ? grad : 0.0;
					}
					scanline += width;
				}

				// precompute FFT
				temp_plan = fftw_plan_dft_r2c_2d(height, width, tempImage, temp3_fft, FFTW_ESTIMATE);
				fftw_execute(temp_plan);
				fftw_destroy_plan(temp_plan);

				// add to numerator
				for (int i = 0; i < m2; i++) {
					xImage_fft[i][0] = uImage_fft[i][0] + mu * REAL_ABAR_MUL_B(nabla_h_fft, temp3_fft);
					xImage_fft[i][1] = uImage_fft[i][1] + mu * IMAG_ABAR_MUL_B(nabla_h_fft, temp3_fft);
				}

				// solve g_v
				lp = tempImage;
				scanline = xImage;
				for (int y = 0; y < height; y++) {
					double *scanline1 = (y < height - 1) ? (scanline + width) : xImage;
					for (int x = 0; x < width; x++) {
						double grad = scanline1[x] - scanline[x];
						*(lp++) = (grad < -grad_threshold || grad > grad_threshold) ? grad : 0.0;
					}
					scanline = scanline1;
				}

				// precompute FFT
				temp_plan = fftw_plan_dft_r2c_2d(height, width, tempImage, temp3_fft, FFTW_ESTIMATE);
				fftw_execute(temp_plan);
				fftw_destroy_plan(temp_plan);

				// solve x
				for (int i = 0; i < m2; i++) {
					double denominator = temp_denominator[i] + mu * nabla_norm[i];
					xImage_fft[i][0] = (xImage_fft[i][0] + mu * REAL_ABAR_MUL_B(nabla_v_fft, temp3_fft)) / denominator;
					xImage_fft[i][1] = (xImage_fft[i][1] + mu * IMAG_ABAR_MUL_B(nabla_v_fft, temp3_fft)) / denominator;
				}
				if (mu_new > mu_max && beta_new > beta_max) {
					break; // save an IFFT and an FFT
				} else {
					temp_plan = fftw_plan_dft_c2r_2d(height, width, xImage_fft, xImage, FFTW_ESTIMATE);
					fftw_execute(temp_plan);
					fftw_destroy_plan(temp_plan);
					const double d = 1.0 / m;
					for (int i = 0; i < m; i++) xImage[i] *= d;
#ifdef _DEBUG
					saveImage2("debug.png", xImage, width, height, 0, 0, false, 0);
					printf("t=%d, beta=%0.3g, mu=%0.3g  Press enter to continue...", t, beta, mu);
					getchar();
#endif
				}

				// next step
				mu = mu_new;
				if (mu > mu_max) break;
			}

			// next step
			beta = beta_new;
			if (beta > beta_max) break;
		}

		// --- solve kImage using FFT(x) ---
		for (int i = 0; i < m2; i++) {
			const double d = nabla_norm[i] / (nabla_norm[i] * NORM_A(xImage_fft) + gamma_);
			kImage_fft[i][0] = REAL_ABAR_MUL_B(xImage_fft, yImage_fft) * d;
			kImage_fft[i][1] = IMAG_ABAR_MUL_B(xImage_fft, yImage_fft) * d;
		}
		temp_plan = fftw_plan_dft_c2r_2d(height, width, kImage_fft, kImage, FFTW_ESTIMATE);
		fftw_execute(temp_plan);
		fftw_destroy_plan(temp_plan);
		{
			double sum = 0.0;
			double *lp = kImage;
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					if (*lp <= 0.0 || (maxSize > 0 && ((x > maxSize && x < width - maxSize) || (y > maxSize && y < height - maxSize)))) {
						*lp = 0.0;
					} else {
						sum += *lp;
					}
					lp++;
				}
			}
			sum = 1.0 / sum;
			for (int i = 0; i < m; i++) {
				kImage[i] *= sum;
			}
		}

		// --- advance to next step
		lambda *= lambda_step;
		if (lambda < lambda_min) lambda = lambda_min;
	}
}

void usage() {
	puts(
		"Usage: deblur [options] <input> <output>\n"
		"Options:\n"
		"  -k, --kernel <file>        output estimated kernel\n"
		"  -m, --max-kernel-size <n>  maximal kernel radius\n"
		"  -r, --rect <x>,<y>,<w>,<h> specify the region to be processed\n"
		"  --gamma <n>                weight for blur kernel (default 2.0)\n"
		"  --sigma <n>                weight for # of non-zero pixels (default 1.0)\n"
		);
	exit(1);
}

int main(int argc, char** argv) {
	Deblur deblur;
	for (int i = 1; i < argc; i++) {
		std::string s = argv[i];
		if (!s.empty() && s[0] == '-') {
			if (s == "-k" || s == "--kernel") {
				if (++i >= argc) usage();
				deblur.kernelFile = argv[i];
			} else if (s == "-m" || s == "--max-kernel-size") {
				if (++i >= argc) usage();
				char *end;
				deblur.maxKernelSize = strtol(argv[i], &end, 10);
				if (*end || deblur.maxKernelSize < 0) usage();
			} else if (s == "-r" || s == "--rect") {
				if (++i >= argc) usage();
				if (sscanf(argv[i], "%d,%d,%d,%d", &deblur.rx, &deblur.ry, &deblur.rw, &deblur.rh) != 4) usage();
			} else if (s == "--gamma") {
				if (++i >= argc) usage();
				char *end;
				deblur.gamma_ = strtod(argv[i], &end);
				if (*end || deblur.gamma_ < 0.0) usage();
			} else if (s == "--sigma") {
				if (++i >= argc) usage();
				char *end;
				deblur.sigma_ = strtod(argv[i], &end);
				if (*end || deblur.sigma_ < 0.0) usage();
			} else {
				usage();
			}
		} else if (deblur.inputFile.empty()) {
			deblur.inputFile = s;
		} else if (deblur.outputFile.empty()) {
			deblur.outputFile = s;
		}
	}
	if (deblur.inputFile.empty() || deblur.outputFile.empty()) usage();
	FreeImage_Initialise();
	deblur.run();
	FreeImage_DeInitialise();
	return 0;
}
