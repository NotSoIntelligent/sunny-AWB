#include "main.h"
#include "bmpHeader.h"
#include "awbCommon.h"
#include <string.h>
#include <math.h>

#include "awbTypes.h"

/* Calculate RG and BG ratio and calculate Mean for it */
/* Function assumes uncompressed RGB888 Format input buffer */
struct colorScores calculateIlluminantScores (uint8_t *buf, dibHeader *dib)
{
	int i = 0, ignorePxls = 0;
	int tempScore[NUM_ILLUMINANTS + 1] = {0};
	float ratioRG = 0, ratioBG = 0;
	struct colorScores score = {0};
	struct RGBPix *pix = NULL;

	pix = (struct RGBPix*) buf;

	/* Usually R/G and B/G performed on Bayer, but for current implementation we are multiplying RGB's G by 2 */	
	for (i = 0; i < (dib->width * dib->height); i++) {
		if (pix->G != 0) {
			ratioRG = ((float)pix->R / (2 * pix->G));
			ratioBG = ((float)pix->B / (2 * pix->G));
			pix++;
			
			/* find illuminant */
			tempScore[findFit(ratioRG, ratioBG)]++;
		} else {
			ignorePxls++;
			pix++;
		}
	}

	memcpy (&score, &tempScore, sizeof(int) * (NUM_ILLUMINANTS + 1));

	printf ("Pixels Ignored = %d\n", ignorePxls);

	return score;
}

/* To search for perfect fit for given RG,BG coordinate point in the given set of reference points */
/* Takes Global constant array of reference points */
int findFit (float x, float y)
{
	int i = 0, ret = 0;
	float tempX = 0, tempY = 0;

	/* Find where RG point Lies */
	if ( x > refPts[NUM_ILLUMINANTS-1][0] && y < refPts[NUM_ILLUMINANTS-1][1] ) {
		return NUM_ILLUMINANTS;    // Rejecting Points below Low CCT
	} else if ( x < refPts[0][0] && y > refPts[0][1] ) {
		return NUM_ILLUMINANTS;    // Rejecting Points above HIGH CCT
	} else {
		for (i = 1; i < NUM_ILLUMINANTS; i++) {  // no need to check index 0 as already checked
		 	if ( x >= refPts[i][0] ) continue;
			else break;
		}
	}

//	printf ("\n (%f, %f) i = %d ", x, y, i);

	/* Find Fit if its i or i-1 */
	if (i <= 0 || i >= NUM_ILLUMINANTS) {
		return NUM_ILLUMINANTS;    // Invalid Case
	} else {
		/* Check If it belongs to i or i-1 Zone by finding Distance beweeen points */
		tempX = distBtw2Pts (refPts[i-1][0], refPts[i-1][1], x, y);
		tempY = distBtw2Pts (refPts[i][0], refPts[i][1], x, y);
		
		if (tempX == 0) {
			ret = i-1;
		} else if (tempY == 0) {
			ret = i;
		} else {
			/* checking which distance is greater */
			if ( tempX > tempY) {
				ret = i;
			} else {
				ret = i-1;
			}

			/* Checking If the new point is not too far away from ref Pts */
			if ( calcEucDist (tempX, tempY, refPtsDist[i-1]) >= RATIO_BUFFER) {
//				printf (" non gray ");
				ret = NUM_ILLUMINANTS; // Too far away Case
			}
		}
	}
//	printf ("ret = %d", ret);
	return ret;
}

float interpolate (float x1, float y1, float x2, float y2, float x)
{
	return ( y1 + (((x - x1) * (y2 - y1)) / (x2 - x1)) );
}

float distBtw2Pts (float x1, float y1, float x2, float y2)
{
	return ( sqrt ((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)) );
}

float calcEucDist (float side1, float side2, float base)
{
	float area = 0, height = 0;
	float s = ( side1 + side2 + base ) / 2;

	area = sqrt ( s * (s - side1) * (s - side2) * (s - base) );
	height = ( 2 * area ) / base;

	return height;
}

/* AWB Core Functions */
int awbProcess (uint8_t *buf, dibHeader *dib)
{
	struct colorScores score = {0};
	
	score = calculateIlluminantScores (buf,dib);

	printf ("Color Score High CCT : %d\n", score.highCCT);
	printf ("Color Score D65      : %d\n", score.D65);
	printf ("Color Score D50      : %d\n", score.D50);
	printf ("Color Score CWF      : %d\n", score.CWF);
	printf ("Color Score TL84     : %d\n", score.TL84);
	printf ("Color Score A        : %d\n", score.A);
	printf ("Color Score H        : %d\n", score.H);
	printf ("Color Score Low CCT  : %d\n", score.lowCCT);
	printf ("Color Score invalid  : %d\n", score.invalid);


	return 0;
}
