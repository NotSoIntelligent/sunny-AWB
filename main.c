/* Author : Sunny */
/* Project: custom AWB */

#include "main.h"
#include "bmpHeader.h"
#include "awbCommon.h"

int main(int argc, char *argv[])
{
	char *filename = NULL;
	int ret = 0;

	uint8_t *buf = NULL;
	bmpfileMagic bmpMagic;
	bmpHeader bmpHead;
	dibHeader dib;

	/* Process CMD Line arguments and get Filename */
	ret = processArgs(argc, argv, &filename);
	if (ret) {
		printf ("Error in CMDLINE arguments\n");
		goto deinit;
	}

	/* get Image Data and BMP Header Data */
	ret = getImage (&buf, &bmpMagic, &bmpHead, &dib, filename);
	if (ret) {
		printf ("Error in Getting Image\n");
		goto deinit;
	}

	/* remove BMP File padding before feeding for Image Processing */
	ret = removePad(buf, &dib);
	if (ret == -1) goto deinit;
	else ret = 0;

	/* AWB Process */
	ret = awbProcess (buf, &dib);
	if (ret) {
		printf ("Error in AWB Processing\n");
		goto deinit;
	}

	/* Add BMP File padding before saving to File */
	ret = addPad(buf, &dib);
	if (ret == -1) goto deinit;
	else ret = 0;

	ret = saveImage (buf, &bmpMagic, &bmpHead, &dib);
	if (ret) {
		printf ("Error in Getting Image\n");
		goto deinit;
	}

deinit:
	printf ("\nImage processing Done : ret(%d)\n", ret);
	if (!buf) {free(buf); buf = NULL;}
	return ret;
}

int processArgs (int argc, char *argv[], char **filename)
{
	if (argc != 2) {
		printUsage();
		return -1;
	} else {
		*filename = argv[1];
		return 0;
	}
}

void printUsage ()
{
	printf ("USAGE: ./<executable_name> <filename>\n");
}
