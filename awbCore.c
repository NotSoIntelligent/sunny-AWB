#include "main.h"
#include "bmpHeader.h"
#include "awbCommon.h"
#include <string.h>
#include <math.h>

#include "awbTypes.h"

#define CALIB 0

/* Calculate RG and BG ratio and calculate Mean for it */
/* Function assumes uncompressed RGB888 Format input buffer */
struct awbMetaData calculateIlluminantScores (uint8_t *buf, dibHeader *dib)
{
	int i = 0, ignorePxls = 0;
	int *scorePtr = NULL;
	float *distPtr = NULL;
	int fit = 0;
	float tempDist = 0;
	float ratioRG = 0, ratioBG = 0;
	struct awbMetaData meta = {0};
	struct RGBPix *pix = NULL;
#if (CALIB)
	float rgSum = 0, bgSum = 0;
#endif
	pix = (struct RGBPix*) buf;

	/* Initializations */
	scorePtr = (int *)&(meta.score);
	distPtr = (float *)&(meta.dist);

	/* Usually R/G and B/G performed on Bayer, but for current implementation we are multiplying RGB's G by 2 */	
	for (i = 0; i < (dib->width * dib->height); i++) {
		if (pix->G != 0) {
			ratioRG = ((float)pix->R / (2 * pix->G));
			ratioBG = ((float)pix->B / (2 * pix->G));
			pix++;

#if (CALIB)
			rgSum = rgSum + ratioRG;
			bgSum = bgSum + ratioBG;
#endif
			/* find illuminant */
			fit = findFit(ratioRG, ratioBG, &tempDist);
			*(scorePtr + fit) = *(scorePtr + fit) + 1;
			*(distPtr + fit) = *(distPtr + fit) + tempDist;

		} else {
			ignorePxls++;
			pix++;
		}
	}

	printf ("Pixels Ignored = %d\n", ignorePxls);
#if (CALIB)
	rgSum = rgSum / (i - 1 - ignorePxls) ;
	bgSum = bgSum / (i - 1 - ignorePxls) ;
	printf ("CALIB rgSum = %f bgSum = %f\n", rgSum, bgSum);
#endif

	/* Store distance averages */
	for (i = 0; i < NUM_ILLUMINANTS + 1; i++) {
		if (*(scorePtr + i) != 0) {
			*(distPtr + i) = (*(distPtr + i)) / (*(scorePtr + i));
		} else {
			*(distPtr + i) = 0;
		}
	}

	return meta;
}

/* To search for perfect fit for given RG,BG coordinate point in the given set of reference points */
/* Takes Global constant array of reference points */
int findFit (float x, float y, float *dist)
{
	int i = 0, ret = 0;
	float tempX = 0, tempY = 0;

	*dist = 0;
	/* Find where RG point Lies */
	if ( x > refPts[NUM_ILLUMINANTS-1][0] && y < refPts[NUM_ILLUMINANTS-1][1] ) {
		return NUM_ILLUMINANTS;    // Rejecting Points below Low CCT
	} else if ( x < refPts[0][0] && y > refPts[0][1] ) {
		return NUM_ILLUMINANTS;    // Rejecting Points above HIGH CCT
	} else {
		for (i = 1; i < NUM_ILLUMINANTS; i++) {  // no need to check index 0 as already checked
			if ( x >= refPts[i][0] ) {
				continue;
			} else {
				if ((x+y) <= RATIO_BUFFER) {
					break;
				} else {
					return NUM_ILLUMINANTS;    // Rejecting Points too far away from reference Line
				}
			}
		}
	}

	/* Find Fit if its i or i-1 */
	if (i <= 0 || i >= NUM_ILLUMINANTS) {
		return NUM_ILLUMINANTS;    // Invalid Case
	} else {
		tempX = calcVectDist (refPts[i-1][0], refPts[i-1][1], refPts[i][0], refPts[i][1], x, y, refPtsDist[i-1]);
		tempY = refPtsDist[i-1] - tempX;

		*dist = (tempX < tempY) ? tempX : tempY;
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
		}
	}
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

float calcVectDist (float x1, float y1, float x2, float y2, float x, float y, float refDist)
{
	/* Form vector with x1,y1 and x2,y2
	* Form vector with x1,y1 and x,y
	* Find Dot product
	* and by other definition of dot product find projection of x,y on line between x1,y1 and x2,y2*/

	float vectDist = 0;
	vectDist = ( (((x - x1) * (x2 - x1)) + ((y - y1) * (y2 - y1))) / refDist );

//	printf (" x, y = (%f, %f)\n x1, y1 = (%f, %f)\n x2, y2 = (%f, %f)\n refDist = %f vectDist = %f\n\n", x, y, x1, y1, x2, y2, refDist, vectDist);

	return vectDist;
}

/* AWB Core Functions */
int awbProcess (uint8_t *buf, dibHeader *dib)
{
	struct awbMetaData meta = {0};
	
	meta = calculateIlluminantScores (buf,dib);
	printMetaData (meta);

	return 0;
}

void printMetaData (struct awbMetaData meta)
{
	printf ("Color Score High CCT : %d\tdistance : %f\n", meta.score.highCCT, meta.dist.highCCT);
	printf ("Color Score D65      : %d\tdistance : %f\n", meta.score.D65,     meta.dist.D65);
	printf ("Color Score CWF      : %d\tdistance : %f\n", meta.score.CWF,     meta.dist.CWF);
	printf ("Color Score TL84     : %d\tdistance : %f\n", meta.score.TL84,    meta.dist.TL84);
	printf ("Color Score U30      : %d\tdistance : %f\n", meta.score.U30,     meta.dist.U30);
	printf ("Color Score A        : %d\tdistance : %f\n", meta.score.A,       meta.dist.A);
	printf ("Color Score H        : %d\tdistance : %f\n", meta.score.H,       meta.dist.H);
	printf ("Color Score Low CCT  : %d\tdistance : %f\n", meta.score.lowCCT,  meta.dist.lowCCT);
	printf ("Color Score invalid  : %d\tdistance : %f\n", meta.score.invalid, meta.dist.invalid);

}
