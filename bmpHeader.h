/* Header Files */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define FOURBYTE 4
#define OUT_FILENAME "OutputProcessedImage.bmp"

/* Definitions - Structures */
typedef struct{
	unsigned char magic[2];
}bmpfileMagic;

typedef struct{
uint32_t filesz;
uint16_t creator1;
uint16_t creator2;
uint32_t bmp_offset;
}bmpHeader;

typedef struct{
uint32_t header_sz;
uint32_t width;
uint32_t height;
uint16_t nplanes;
uint16_t bitspp;
uint32_t compress_type;
uint32_t bmp_bytesz;
uint32_t hres;
uint32_t vres;
uint32_t ncolors;
uint32_t nimpcolors;
} dibHeader;

/* Definitions - Functions */
void displayHeader(bmpHeader *bmpHead);
void displayDIBHeader(dibHeader *dib);
int removePad(uint8_t *buf, dibHeader *dib);
int addPad(uint8_t *buf, dibHeader *dib);
int saveImage (uint8_t *buf, bmpfileMagic *bmpMagic, bmpHeader *bmpHead, dibHeader *dib);
int getImage (uint8_t **buf, bmpfileMagic *bmpMagic, bmpHeader *bmpHead, dibHeader *dib, char *filename);
