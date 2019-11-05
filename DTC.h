#pragma once
#include "CC.h"

typedef struct {
	int32_t **Coeff_buff_arr;
	uint32_t number_of_blocks;
	uint16_t block_width;
	uint16_t block_hight;
	uint32_t image_hight;
	uint32_t image_width;
}DCT_Coeff_t;

char DTC_Block(YUV_Image_t* YUV_Image, DCT_Coeff_t * Y_DCT_Coeff, DCT_Coeff_t* U_DCT_Coeff, DCT_Coeff_t* V_DCT_Coeff);
char IDTC_Block(DCT_Coeff_t* Y_DCT_Coeff, DCT_Coeff_t* U_DCT_Coeff, DCT_Coeff_t* V_DCT_Coeff, YUV_Image_t* YUV_Image);