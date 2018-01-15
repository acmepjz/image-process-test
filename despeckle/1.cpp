#include <allheaders.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>

PIX *pixHMTDwa_101(PIX *pixd, PIX *pixs, const char *selname);
PIX *pixFHMTGen_101(PIX *pixd, PIX *pixs, const char *selname);

const char* const seq_3hm[] = {
	"3hm", NULL
};

const char* const seq_5hm[] = {
	"5hm", NULL
};

const char* const seq_3hm2[] = {
	"3hm2_h", "3hm2_v", "3hm2_d1", "3hm2_d2",
	"3hm", NULL
};

const char* const seq_3hm3[] = {
	"3hm3_h", "3hm3_v", "3hm3_c1", "3hm3_c2", "3hm3_c3", "3hm3_c4",
	"3hm2_h", "3hm2_v", "3hm2_d1", "3hm2_d2",
	"3hm", NULL
};

const char* const seq_3hm4[] = {
	"3hm4_c",
	"3hm3_h", "3hm3_v", "3hm3_c1", "3hm3_c2", "3hm3_c3", "3hm3_c4",
	"3hm2_h", "3hm2_v", "3hm2_d1", "3hm2_d2",
	"3hm", NULL
};

void usage() {
	printf("Usage: despeckle <type> <input> <output>\n"
		"  type:\n"
		"    * '3hm': remove isolated pixels in 3x3\n"
		"    * '5hm': remove isolated pixels in 5x5\n"
		"    * '3hm2': remove connected components up to 2 pixels in 3x3\n"
		"    * '3hm3': remove connected components up to 3 pixels in 3x3\n"
		"    * '3hm4': remove connected components up to 4 pixels in 3x3\n"
		"    * '4cc<n>': remove conn. comp. up to <n> pixels using 4-connectivity\n"
		"    * '8cc<n>': remove conn. comp. up to <n> pixels using 8-connectivity\n"
		);
	exit(1);
}

void runHMT(PIX *pixs, const char* const* seq) {
	for (int i = 0; seq[i]; i++) {
		const char* selname = seq[i];

		PIX *pixt = pixFHMTGen_101(NULL, pixs, selname);
		pixSubtract(pixs, pixs, pixt);
		pixDestroy(&pixt);
	}
}

int main(int argc, char** argv) {
	if (argc != 4) usage();

	const char* const* seq = NULL;
	char* type = argv[1];

	// used for connected component mode
	int connectivity = 0, threshold = 0;

	if (strcmp(type, "3hm") == 0) seq = seq_3hm;
	else if (strcmp(type, "5hm") == 0) seq = seq_5hm;
	else if (strcmp(type, "3hm2") == 0) seq = seq_3hm2;
	else if (strcmp(type, "3hm3") == 0) seq = seq_3hm3;
	else if (strcmp(type, "3hm4") == 0) seq = seq_3hm4;
	else if ((type[0] == '4' || type[0]=='8') && type[1] == 'c' && type[2] == 'c' && type[3]) {
		connectivity = type[0] - '0';
		threshold = atoi(type + 3);
		if (threshold <= 0) usage();
	} else usage();

	PIX *pixs = pixRead(argv[2]);
	if (!pixs) {
		printf("Error: failed to load file '%s'\n", argv[2]);
		return 1;
	}

	PIX *pixt;

	if (pixs->d != 1) {
		pixt = pixConvertTo1(pixs, 128);
		pixDestroy(&pixs);
		pixs = pixt;
	}

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

	int ret = 0;

	if (pixWriteAutoFormat(argv[3], pixs)) {
		printf("Error: failed to save file '%s'\n", argv[3]);
		ret = 1;
	}

	pixDestroy(&pixs);

	return ret;
}
