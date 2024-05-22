/* BMP Operations */
#include "bmpHeader.h"
#include <string.h>

static int negHeightFlag = 0;

/* To Display Contents of BMP Header */
void displayHeader( bmpHeader *bmpHead )
{
	printf("\nHEADER CONTENTS :-\n\nBMP file size : %d bytes\nBMP offset : %d bytes\n\n",
			bmpHead->filesz ,bmpHead->bmp_offset  );

	return;
}

/* To Display Contents of DIB Header */
void displayDIBHeader( dibHeader *dib )
{
	printf("\nDIB HEADER CONTENTS :-\n\
			\nHeader size\t\t\t\t: %d bytes\
			\nWidth\t\t\t\t\t: %d\
			\nHeight\t\t\t\t\t: %d\
			\nNumber of color planes\t\t\t: %d\
			\nNumber of bits per pixel\t\t: %d bits\
			\nCompression type\t\t\t: %d\
			\nRaw image size\t\t\t\t: %d bytes\
			\nHorizontal res of image\t\t\t: %d\
			\nVertical res of image\t\t\t: %d\
			\nNumber of colors\t\t\t: %d\
			\nImportant colors\t\t\t: %d\n\n",
			dib->header_sz, dib->width, dib->height, dib->nplanes,
			dib->bitspp, dib->compress_type, dib->bmp_bytesz, dib->hres,
			dib->vres, dib->ncolors, dib->nimpcolors );

	return;
}

/* NOTE: Below 2 Funtions are Time Consuming and not needed and can be optimised in Core algorithm itself */
/* Functions are implemented to avoid clutters and unwanted processing in Core algorithm */
/* Remove Padding and return image Buffer */
int removePad (uint8_t *buf, dibHeader *dib)
{
	int i = 0;
	int bufSize = dib->width * dib->bitspp;
	uint8_t *ptr = buf;
	int pad = bufSize % FOURBYTE;

	if (pad == 0) {
		printf ("%s: Image Width is multiple of 4 No Padding\n", __func__);
		return -2;
	}
	for (i = 1; i < dib->height; i++) {
		if ((ptr + (i * bufSize)) != memmove ((ptr + (i * bufSize)), (ptr + (i * (bufSize + pad))), bufSize)) {
			printf ("%s memmove() Failure\n", __func__);
			return -1;
		}
	}

	return 0;
}

/* Add Padding and return image Buffer for BMP */
int addPad (uint8_t *buf, dibHeader *dib)
{
	int i = 0;
	int bufSize = dib->width * dib->bitspp;
	uint8_t *ptr = buf;
	int pad = bufSize % FOURBYTE;
	uint32_t padByte = 0;
	
	if (pad == 0) {
		printf ("%s: Image Width is multiple of 4 No Padding\n", __func__);
		return -2;
	}
	for (i = --(dib->height); i >= 0; i--) {
		if (i == 0) {
			if ((ptr + (i * bufSize)) != memmove ((ptr + (i * (bufSize + pad))), (ptr + (i * bufSize)), bufSize)) {
				printf ("%s memmove() Failure\n", __func__);
				return -1;
			}
		}
		if ((ptr + (i * (bufSize + pad))) + bufSize != memcpy ((ptr + (i * (bufSize + pad))) + bufSize, &padByte, pad)) {
			printf ("%s memcpy() Failure\n", __func__);
			return -1;
		}
	}
	
	return 0;
}

/* Save rawimage into a BMP File */
int saveImage (uint8_t *buf, bmpfileMagic *bmpMagic, bmpHeader *bmpHead, dibHeader *dib) 
{
	FILE *fp = NULL;
	char filename[] = OUT_FILENAME;
	int ret = 0;

	fp = fopen (filename, "wb");
	if (!fp) {
		printf ("%s File Open error!\n", __func__);
		ret = -1;
		goto endGetImageSaveImage;
	}

	if (negHeightFlag) {
		dib->height = 0 - dib->height;
		negHeightFlag = 0;
	}

	if (fwrite (bmpMagic, sizeof(bmpfileMagic), 1, fp) != 1) {
		printf ("%s File Write error!\n", __func__);
		ret = -1;
		goto endGetImageSaveImage;
	}
	if (fwrite (bmpHead, sizeof(bmpHeader), 1, fp) != 1) {
		printf ("%s File Write error!\n", __func__);
		ret = -1;
		goto endGetImageSaveImage;
	}
	if (fwrite (dib, sizeof(dibHeader), 1, fp) != 1) {
		printf ("%s File Write error!\n", __func__);
		ret = -1;
		goto endGetImageSaveImage;
	}

	if (fwrite (buf, 1, dib->bmp_bytesz, fp) != dib->bmp_bytesz) {
		printf ("%s File Write error!\n", __func__);
		ret = -1;
		goto endGetImageSaveImage;
	}

endGetImageSaveImage:
	if (!fp) { fclose(fp); fp = NULL; }
	return ret;
}

/* get Image Data from BMP File */
int getImage (uint8_t **buf, bmpfileMagic *bmpMagic, bmpHeader *bmpHead, dibHeader *dib, char *filename) 
{
	FILE *fp = NULL;
	int ret = 0;

	fp = fopen (filename, "rb");
	if (fp == NULL) {
		printf ("%s File Open error!\n", __func__);
		ret = -1;
		goto endGetImage;
	}

	if (fread (bmpMagic, sizeof(bmpfileMagic), 1, fp) != 1) {
		printf ("%s File read error!\n", __func__);
		ret = -1;
		goto endGetImage;
	}
	if (bmpMagic->magic[0] != 0x42 && bmpMagic->magic[0] != 0x4d) { 
		printf ("%s Not an BMP File!\n", __func__);
		ret = -1;
	}

	if (fread (bmpHead, sizeof(bmpHeader), 1, fp) != 1) {
		printf ("%s File read error!\n", __func__);
		ret = -1;
		goto endGetImage;
	}
	if (fread (dib, sizeof(dibHeader), 1, fp) != 1) {
		printf ("%s File read error!\n", __func__);
		ret = -1;
		goto endGetImage;
	}

	displayHeader(bmpHead);
	displayDIBHeader(dib);

	/* Some BMP images have negative Height to denote inversion*/
	if ((int)dib->height < 0) {
		dib->height = abs(dib->height);
		negHeightFlag = 1;
	}

	*buf = (uint8_t*) malloc (sizeof(uint8_t) * dib->bmp_bytesz);
	if (!*buf) {
		printf ("%s malloc Failure!\n", __func__);
		ret = -1;
		goto endGetImage;
	}

	if (fread (*buf, dib->bmp_bytesz, 1, fp) != 1) {
		printf ("%s File read error!\n", __func__);
		ret = -1;
		goto endGetImage;
	}

endGetImage:
	if (!fp)   { fclose(fp); fp = NULL; }
	if (!*buf) { free(*buf); *buf = NULL; }
	return ret;
}

