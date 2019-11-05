#pragma once
#include <iostream>

typedef struct {
	uint8_t* y_block_stream;
	uint32_t y_stream_len;
	uint8_t* u_stream;
	uint32_t u_stream_len;
	uint8_t* v_stream;
	uint32_t v_stream_len;
	uint32_t image_hight;
	uint32_t image_width;
}Stream_t;

// Quantization_Block
char Quantization_Block(DCT_Coeff_t* Y_DCT_Coeff, DCT_Coeff_t* U_DCT_Coeff, DCT_Coeff_t* V_DCT_Coeff, Stream_t* stream);
// deQuantization_Block
char deQuantization_Block(Stream_t *stream, DCT_Coeff_t* Y_DCT_Coeff, DCT_Coeff_t* U_DCT_Coeff, DCT_Coeff_t* V_DCT_Coeff);