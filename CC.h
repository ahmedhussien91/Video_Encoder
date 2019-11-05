#pragma once
#include <iostream>
#include <stdio.h>
#include "ImR.h"

typedef struct {
	uint8_t* Y_buff;
	uint8_t* U_buff;
	uint8_t* V_buff;
	uint32_t Y_size_bytes;
	uint32_t U_size_bytes;
	uint32_t V_size_bytes;
	uint32_t Y_pixels_hight;
	uint32_t Y_pixels_width;

}YUV_Image_t;

char CC_Block(Out_Image_t* RGB_Image, YUV_Image_t* YUV_image, const char* CC_type);
char ICC_Block(YUV_Image_t* YUV_image, Out_Image_t* RGB_Image, const char* ICC_type);
