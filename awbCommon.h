struct RGBPix {
	uint8_t B;
	uint8_t G;
	uint8_t R;
};

struct RGBAPix {
	uint8_t R;
	uint8_t G;
	uint8_t B;
	uint8_t A;
};

struct colorScores {
	int highCCT;
	int D65;
	int D50;
	int CWF;
	int TL84;
	int A;
	int H;
	int lowCCT;
	int invalid;
};

/* RGB Gains for standard illuminants */

int findFit (float x, float y);
int awbProcess (uint8_t *buf, dibHeader *dib);
float distBtw2Pts (float x1, float y1, float x2, float y2);
float interpolate (float x1, float y1, float x2, float y2, float x);
float calcEucDist (float side1, float side2, float base);
struct colorScores calculateIlluminantScores (uint8_t *buf, dibHeader *dib);
