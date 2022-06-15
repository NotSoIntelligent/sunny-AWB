#define NUM_ILLUMINANTS 8
#define RATIO_BUFFER 1.5

/* reference Points Taken for an example sensor and 
 * these value determines the detection of illuminant */
const float refPts[8][2]= {
	{ 0.3457, 0.8432 },
	{ 0.494582, 0.521942 }, // D65
	{ 0.554094, 0.302984 }, // CWF
	{ 0.625128, 0.322844 }, // TL84
	{ 0.791323, 0.158105 }, // U30
	{ 0.871423, 0.189032 }, // A
	{ 1.091704, 0.123175 }, // HZ
	{ 1.3070, 0.0700 },
};

/* Values formed from Calibration after these reference points*/
/* Lut for distance between reference points for optimisation */
const float refPtsDist [7] = {
	0.35408,
	0.226901,
	0.073758,
	0.234008,
	0.085863,
	0.229915,
	0.221766,
};
