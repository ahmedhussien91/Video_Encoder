#pragma once
#include "Image.h"
const int DCT_block_hight = 8, DCT_block_width = 8;

class Ecoder
{
public:
	Ecoder(){
	};
	
	void Color_Compression(Image* input_image);
	void DCT_Compression(Image* input_image, Dct_Coeff_t *dct_coeff_out);
	int dctTransform(uint8_t matrix[][DCT_block_width], float dct[][DCT_block_width]);

private:
};

