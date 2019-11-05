#pragma once

#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <stdio.h>
#include "ImageType.h"

#  define BF_TYPE 0x4D42             /* "MB" */

#pragma pack(2)
typedef struct                       /**** BMP file header structure ****/
{
	uint16_t	bfType;           /* Magic number for file */
	uint32_t	bfSize;           /* Size of file */
	uint16_t	bfReserved1;      /* Reserved */
	uint16_t	bfReserved2;      /* ... */
	uint32_t	bfOffBits;        /* Offset to bitmap data */
} BITMAPFILEHEADER;



typedef struct                       /**** BMP file info structure ****/
{
	uint32_t	biSize;           /* Size of info header */
	int32_t		biWidth;          /* Width of image */
	int32_t		biHeight;         /* Height of image */
	uint16_t	biPlanes;         /* Number of color planes */
	uint16_t	biBitCount;       /* Number of bits per pixel */
	uint32_t	biCompression;    /* Type of compression to use */
	uint32_t	biSizeImage;      /* Size of image data */
	int32_t		biXPelsPerMeter;  /* X pixels per meter */
	int32_t		biYPelsPerMeter;  /* Y pixels per meter */
	uint32_t	biClrUsed;        /* Number of colors used */
	uint32_t	biClrImportant;   /* Number of important colors */
} BITMAPINFOHEADER;
#pragma pack()
/*
 * Constants for the biCompression field...
 */

#  define BI_RGB       0             /* No compression - straight BGR data */
#  define BI_RLE8      1             /* 8-bit run-length compression */
#  define BI_RLE4      2             /* 4-bit run-length compression */
#  define BI_BITFIELDS 3             /* RGB bitmap with RGB masks */

typedef struct                       /**** Colormap entry structure ****/
{
	uint8_t  rgbBlue;          /* Blue value */
	uint8_t  rgbGreen;         /* Green value */
	uint8_t  rgbRed;           /* Red value */
} RGBQUAD;

class Bitmap:ImageType {
public:
	Bitmap(const char* filename);
	virtual void read_image(ImageInfo_t* ImageInfo);
	~Bitmap();
	uint8_t *buff;
	BITMAPFILEHEADER fh;
	BITMAPINFOHEADER ih;
	uint32_t image_hight_pixels;
	uint32_t image_width_pixels;
	uint16_t image_pixel_size;
	uint32_t image_size_bytes;
private:

};

