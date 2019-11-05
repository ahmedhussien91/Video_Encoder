#include "Bitmap.h"

Bitmap::Bitmap(const char* filename) {
	FILE* file;
	file = fopen(filename, "rb");

	if (file != NULL) { // file opened
		size_t x = fread(&fh, sizeof(BITMAPFILEHEADER), 1, file); //reading the FILEHEADER
		x = fread(&this->ih, sizeof(BITMAPINFOHEADER), 1, file) + x;
		// create Buffer
		image_size_bytes = (ih.biHeight*ih.biWidth*ih.biBitCount + ((ih.biHeight * ih.biWidth * ih.biBitCount)%32))/8;
		this->buff = new uint8_t[image_size_bytes];
		this->image_hight_pixels= ih.biHeight;
		this->image_width_pixels= ih.biWidth;
		this->image_pixel_size = ih.biBitCount;
		x = fread(this->buff, image_size_bytes, 1, file) + x;

		fclose(file);
	}
}

void Bitmap::read_image(ImageInfo_t* ImageInfo) {
	ImageInfo->buff			= this->buff;
	ImageInfo->buff_size	= this->image_size_bytes;
	ImageInfo->hight = this->image_hight_pixels;
	ImageInfo->width = this->image_width_pixels;
	ImageInfo->pixel_size = this->image_pixel_size;
}

Bitmap::~Bitmap() {
	delete this->buff;
}