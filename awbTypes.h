#define NUM_ILLUMINANTS 8
#define RATIO_BUFFER 0.14

/* reference Points Taken for an example sensor and 
 * these value determines the detection of illuminant */
const float refPts[8][2]= {
	{ 0.3457, 0.8432 },
	{ 0.5454, 0.6434 }, // D65
	{ 0.6048, 0.5833 }, // D50
	{ 0.6201, 0.4402 }, // CWF
	{ 0.7057, 0.4696 }, // TL84
	{ 0.9004, 0.4015 }, // A
	{ 1.0661, 0.3805 }, // HZ
	{ 1.3070, 0.3300 },
};

/* Lut for distance between reference points for optimisation */
const float refPtsDist [7] = {
	0.282489,
	0.084501,
	0.143916,
	0.090508,
	0.206266,
	0.167025,
	0.246136,	
};
