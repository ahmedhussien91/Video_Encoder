#pragma once
#include <stdint.h>
#include <iostream>
#include "ImageType.h"

typedef struct {
	float* Y_coeff_buff;
	float* U_coeff_buff;
	float* V_coeff_buff;
	uint32_t  Size;
}Dct_Coeff_t;

typedef enum {
	RAW,
	BMP,
	TIF,
	TXT
}ImageType_E; 

typedef struct {
	uint32_t hight;
	uint32_t width;
}image_size_t;

class Image
{
public:
	Image(ImageType* p_imagetype);

	void createRGPfromGRAY(void);
	void createImagefromRGB(void);
	void convertRGBtoYUV(void);
	void convertYUVtoRGB(void);
	void convert444to422(void);
	void convert422to444(void);
	void convert444to411(void);
	void convert411to444(void);
	void convert444toavg411(void);
	void convertYUVtoYUVBlocks444(void);
	void set_hight(uint32_t hight) { ImageInfo.hight = hight; };
	void set_width(uint32_t width) { ImageInfo.width = width; };

	uint32_t get_hight(void) { return ImageInfo.hight; };
	uint32_t get_width(void) { return ImageInfo.width; };
	
	~Image() { delete ImageInfo.buff; };
	ImageInfo_t ImageInfo;
	uint8_t *RGB_buff;
	uint8_t *YUV_buff;
	uint8_t* YUV_buff422;
	uint8_t* YUV_buff411;
	uint8_t* YUV_buffavg;
private:

};

