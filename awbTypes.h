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

const int standardCCT[8] = {
	9000,
	6500,
	4100,
	4000,
	3000,
	2850,
	2200,
	1000,
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

/* RGB Color correction Matrices for Standard illuminants
 * to be Calibrated for each input of reference points */
const float rgbCCM [8][3][3] = {
	/* High CCT CCM */
	{
		{1, 0, 0},
		{0, 1, 0},
		{0, 0, 1},
	},
	/* D65 CCM */
	{
		{ 0.84184,-0.18537,-0.18946},
		{-0.21188, 0.91108,-0.21320},
		{-0.13617,-0.23009, 0.80741},
	},
	/* CWF CCM */
	{
		{ 0.58970,-0.09724, 0.13263},
		{-0.06640, 0.80484, 0.03729},
		{ 0.01704,-0.13102, 0.95301},
	},
	/* TL84 CCM */
	{
		{0.50196, 0.00362, 0.02634},
		{-0.06141, 0.56226, 0.02491},
		{-0.01345, 0.01592, 0.95369},
	},
	/* U30 CCM */
	{
		{ 0.39460,-0.13054, 0.05902},
		{-0.01401, 0.82385,-0.03654},
		{ 0.15534, 0.17785, 1.68775},
	},
	/* A CCM */
	{
		{ 0.19174,-0.08791, 0.03489},
		{ 0.45989, 0.93654, 0.13054},
		{ 0.14984, 0.11621, 1.77123},
	},
	/* H CCM */
	{
		{ 0.25353,-0.18663,-0.00779},
		{ 0.24522, 1.27952, 0.27463},
		{ 0.20059, 0.42290, 3.34762},
	},
	/* Low CCT CCM */
	{
		{1, 0, 0},
		{0, 1, 0},
		{0, 0, 1},
	},
};
