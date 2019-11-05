#include "ImR.h"

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

uint8_t* buff;
BITMAPFILEHEADER fh;
BITMAPINFOHEADER ih;
uint32_t image_hight_pixels;
uint32_t image_width_pixels;
uint16_t image_pixel_size;
uint32_t image_size_bytes;


char ImR_ImageRead(const char* ImagePath, const char* ImageType, Out_Image_t * out_image)
{
	FILE* file;
	file = fopen(ImagePath, "rb");

	if (file != NULL) { // file opened
		size_t x = fread(&fh, sizeof(BITMAPFILEHEADER), 1, file); //reading the FILEHEADER
		x = fread(&ih, sizeof(BITMAPINFOHEADER), 1, file);
		// create Buffer
		image_size_bytes = (ih.biHeight * ih.biWidth * ih.biBitCount + ((ih.biHeight * ih.biWidth * ih.biBitCount) % 32)) / 8;
		buff = new uint8_t[image_size_bytes];
		image_hight_pixels = ih.biHeight;
		image_width_pixels = ih.biWidth;
		image_pixel_size = ih.biBitCount;
		x = fread(buff, image_size_bytes, 1, file);

		fclose(file);
	}

	uint32_t idx = 0;
	out_image->pixel_size = image_pixel_size;
	if (3 == (out_image->pixel_size / 8))
	{
		out_image->R_buff = new uint8_t[image_size_bytes / 3];
		out_image->G_buff = new uint8_t[image_size_bytes / 3];
		out_image->B_buff = new uint8_t[image_size_bytes / 3];
		for (uint32_t i = 0; i < image_size_bytes; i=i+3)
		{
			out_image->R_buff[idx] = buff[i+2];
			out_image->G_buff[idx] = buff[i+1];
			out_image->B_buff[idx] = buff[i];
			idx++;
		}
		out_image->pixels_hight = image_hight_pixels;
		out_image->pixels_width = image_width_pixels;
	}
	else if (1 == (out_image->pixel_size / 8))
	{

		out_image->R_buff = new uint8_t[image_size_bytes];
		out_image->G_buff = new uint8_t[image_size_bytes];
		out_image->B_buff = new uint8_t[image_size_bytes];
		for (uint32_t i = 0; i < image_size_bytes; i = i+1)
		{
			out_image->R_buff[idx] = buff[i];
			out_image->G_buff[idx] = buff[i];
			out_image->B_buff[idx] = buff[i];
			idx++;
		}
		out_image->pixels_hight = image_hight_pixels;
		out_image->pixels_width = image_width_pixels;
	}
	else {
		_ASSERT(false);
	}

	delete buff;

	return 1;
}
