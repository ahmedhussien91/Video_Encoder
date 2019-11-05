#pragma once

#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <stdio.h>

typedef struct 
{
	uint8_t* R_buff;
	uint8_t* G_buff;
	uint8_t* B_buff;
	uint32_t pixels_hight;
	uint32_t pixels_width;
	uint8_t  pixel_size;
}Out_Image_t;


char ImR_ImageRead(const char* ImagePath, const char* ImageType, Out_Image_t* out_image);