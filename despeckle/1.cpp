#include <allheaders.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>
#include <vector>
#include <algorithm>

PIX *pixHMTDwa_101(PIX *pixd, PIX *pixs, const char *selname);
PIX *pixFHMTGen_101(PIX *pixd, PIX *pixs, const char *selname);
PIX *pixMorphDwa_102(PIX *pixd, PIX *pixs, l_int32 operation, const char *selname);
PIX *pixFMorphopGen_102(PIX *pixd, PIX *pixs, l_int32 operation, const char *selname);

#define SEQ_3HM \
	"3hm", NULL

const char* const seq_3hm[] = {
	SEQ_3HM,
	NULL,
};

const char* const seq_5hm[] = {
	"5hm", NULL,
	NULL,
};

#define SEQ_3HM2 \
	"3hm2_h", NULL, \
	"3hm2_v", NULL, \
	"3hm2_d1", NULL, \
	"3hm2_d2", NULL, \
	SEQ_3HM

const char* const seq_3hm2[] = {
	SEQ_3HM2,
	NULL,
};

#define SEQ_3HM3 \
	"3hm3_h", NULL, \
	"3hm3_v", NULL, \
	"3hm3_c1", NULL, \
	"3hm3_c2", NULL, \
	"3hm3_c3", NULL, \
	"3hm3_c4", NULL, \
	SEQ_3HM2

const char* const seq_3hm3[] = {
	SEQ_3HM3,
	NULL,
};

#define SEQ_3HM4 \
	"3hm4_c", NULL, \
	SEQ_3HM3

const char* const seq_3hm4[] = {
	SEQ_3HM4,
	NULL,
};

const char* const seq_smooth[] = {
	"st_av1", NULL,
	"st_ah1", NULL,
	"st_av2", NULL,
	"st_ah2", NULL,
	"st_bv1", NULL,
	"st_bh1", NULL,
	"st_bv2", NULL,
	"st_bh2", NULL,
	"st_cv1", "morph2_v",
	"st_ch1", "morph2_h",
	"st_cv2", "morph2_v",
	"st_ch2", "morph2_h",
	"st_dv1", "morph3_v",
	"st_dh1", "morph3_h",
	"st_dv2", "morph3_v",
	"st_dh2", "morph3_h",
	NULL,
};

void usage() {
	printf("Usage: despeckle <type> [<type>...] <input> <output>\n"
		"  type:\n"
		"    * '3hm': remove isolated pixels in 3x3\n"
		"    * '5hm': remove isolated pixels in 5x5\n"
		"    * '3hm2': remove connected components up to 2 pixels in 3x3\n"
		"    * '3hm3': remove connected components up to 3 pixels in 3x3\n"
		"    * '3hm4': remove connected components up to 4 pixels in 3x3\n"
		"    * '4cc<n>': remove conn. comp. up to <n> pixels using 4-connectivity\n"
		"    * '8cc<n>': remove conn. comp. up to <n> pixels using 8-connectivity\n"
		"    * 'smooth': morphological smooth modified from Scan Tailor\n"
		);
	exit(1);
}

void runHMT(PIX *pixs, const char* const* seq) {
	for (int i = 0; seq[i]; i += 2) {
		const char* selname = seq[i];
		const char* selname2 = seq[i + 1];

		PIX *pixt = pixFHMTGen_101(NULL, pixs, selname);
		if (selname2) {
			PIX *pixtmp = pixFMorphopGen_102(NULL, pixt, L_MORPH_DILATE, selname2);
			pixDestroy(&pixt);
			pixt = pixtmp;
		}
		pixSubtract(pixs, pixs, pixt);
		pixDestroy(&pixt);
	}
}

void runDespeckle(PIX *&pixs, const std::string &type) {
	const char* const* seq = NULL;

	// used for connected component mode
	int connectivity = 0, threshold = 0;

	if (type == "3hm") seq = seq_3hm;
	else if (type == "5hm") seq = seq_5hm;
	else if (type == "3hm2") seq = seq_3hm2;
	else if (type == "3hm3") seq = seq_3hm3;
	else if (type == "3hm4") seq = seq_3hm4;
	else if (type == "smooth") seq = seq_smooth;
	else if ((type[0] == '4' || type[0] == '8') && type[1] == 'c' && type[2] == 'c' && type[3]) {
		connectivity = type[0] - '0';
		threshold = atoi(type.c_str() + 3);
		if (threshold <= 0) usage();
	} else usage();

	PIX *pixt;

	if (connectivity == 4 || connectivity == 8) {
		for (int i = 0; i < 2; i++) {
			pixt = pixConnCompAreaTransform(pixs, connectivity);
			pixDestroy(&pixs);

			pixThresholdToValue(pixt, pixt, threshold, 0);
			pixThresholdToValue(pixt, pixt, threshold + 1, 0x7FFFFFFF);
			pixs = pixConvertTo1(pixt, 1);
			pixDestroy(&pixt);
		}
	} else {
		pixt = pixAddBorder(pixs, 32, 0);
		pixDestroy(&pixs);
		pixs = pixt;

		runHMT(pixs, seq);

		pixs = pixInvert(pixs, pixs);
		runHMT(pixs, seq);
		pixs = pixInvert(pixs, pixs);

		pixt = pixRemoveBorder(pixs, 32);
		pixDestroy(&pixs);
		pixs = pixt;
	}
}

int main(int argc, char** argv) {
	if (argc < 4) usage();

	std::vector<std::string> types;
	for (int i = 1; i < argc - 2; i++) {
		types.push_back(argv[i]);
	}

	const char* inputFile = argv[argc - 2];
	const char* outputFile = argv[argc - 1];

	// load input file

	PIX *pixs = pixRead(inputFile);
	if (!pixs) {
		printf("Error: failed to load file '%s'\n", inputFile);
		return 1;
	}

	PIX *pixt;

	if (pixs->d != 1) {
		pixt = pixConvertTo1(pixs, 128);
		pixDestroy(&pixs);
		pixs = pixt;
	}

	// run despeckle
	for (int i = 0; i < (int)types.size(); i++) {
		runDespeckle(pixs, types[i]);
	}

	int ret = 0;

	if (pixWriteAutoFormat(outputFile, pixs)) {
		printf("Error: failed to save file '%s'\n", outputFile);
		ret = 1;
	}

	pixDestroy(&pixs);

	return ret;
}
