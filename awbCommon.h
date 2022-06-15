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
	int CWF;
	int TL84;
	int U30;
	int A;
	int H;
	int lowCCT;
	int invalid;
};

struct colorDistances {
	float highCCT;
	float D65;
	float CWF;
	float TL84;
	float U30;
	float A;
	float H;
	float lowCCT;
	float invalid;
};

struct awbMetaData {
	struct colorScores score;
	struct colorDistances dist;
};

/* RGB Gains for standard illuminants */

int findFit (float x, float y, float *fit);
int awbProcess (uint8_t *buf, dibHeader *dib);
float distBtw2Pts (float x1, float y1, float x2, float y2);
float interpolate (float x1, float y1, float x2, float y2, float x);
float calcEucDist (float side1, float side2, float base);
struct awbMetaData calculateIlluminantScores (uint8_t *buf, dibHeader *dib);
float calcVectDist (float x1, float y1, float x2, float y2, float x, float y, float refDist);
void printMetaData (struct awbMetaData meta);
