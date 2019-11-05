#pragma once
#include "Image.h"

class Decoder
{
public:

	Decoder() {};
	void inverse_Color_Compression(Image* input_image);
	void inverse_DCT_Compression(Image* input_image);
};

