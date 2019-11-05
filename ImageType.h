#pragma once
typedef struct {
	uint8_t* buff;
	uint32_t buff_size;
	uint32_t hight;	// hight in pixels
	uint32_t width; // width in pixels
	uint32_t pixel_size; // pixel size in bits
}ImageInfo_t;

class ImageType {
public:
	virtual void read_image(ImageInfo_t* ImageInfo) {};

};