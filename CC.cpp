#include "CC.h"
#include "ImR.h"

static void convertRGBtoYUV(Out_Image_t* RGB_Image, YUV_Image_t* YUV_image);
static void convertYUVtoRGB(YUV_Image_t* YUV_image, Out_Image_t* RGB_Image);
static void YUV_convert444_to_avg411(YUV_Image_t* YUV_image_in, YUV_Image_t* YUV_image_out);
static void YUV_convert411_to_444(YUV_Image_t* YUV_image_in, YUV_Image_t* YUV_image_out);



char CC_Block(Out_Image_t* RGB_Image, YUV_Image_t* YUV_image, const char* CC_type)
{
	// enumerate cc_type
	uint8_t cc_type = 1;
	switch (cc_type)
	{
	case 1:
		YUV_Image_t tmp_YUV;
		convertRGBtoYUV(RGB_Image, &tmp_YUV);
		YUV_convert444_to_avg411(&tmp_YUV, YUV_image);
		break;
	default:
		_ASSERT(false);
		break;
	}
	return 1;
}

char ICC_Block(YUV_Image_t* YUV_image, Out_Image_t* RGB_Image, const char* ICC_type)
{
	// enumerate cc_type
	uint8_t icc_type = 1;
	switch (icc_type)
	{
	case 1:
		YUV_Image_t tmp_YUV;
		YUV_convert411_to_444(YUV_image, &tmp_YUV);
		convertYUVtoRGB(&tmp_YUV, RGB_Image);
		break;
	default:
		_ASSERT(false);
		break;
	}
	return 1;
}

void convertRGBtoYUV(Out_Image_t* RGB_Image, YUV_Image_t* YUV_image) {
	uint32_t Image_size = 0;
	Image_size = RGB_Image->pixels_hight * RGB_Image->pixels_width;
	YUV_image->Y_pixels_hight = RGB_Image->pixels_hight;
	YUV_image->Y_pixels_width = RGB_Image->pixels_width;

	YUV_image->Y_size_bytes = Image_size;
	YUV_image->U_size_bytes = Image_size;
	YUV_image->V_size_bytes = Image_size;

	YUV_image->Y_buff = new uint8_t[Image_size];
	YUV_image->U_buff = new uint8_t[Image_size];
	YUV_image->V_buff = new uint8_t[Image_size];

	for (uint32_t i = 0; i < Image_size; i = i + 1)
	{
		int16_t tmp1, tmp2, tmp3;
		float R = RGB_Image->R_buff[i];
		float G = RGB_Image->G_buff[i];
		float B = RGB_Image->B_buff[i];
		tmp1 = ((int16_t)(0.299 * R + 0.587 * G + 0.114 * B));//(0.257 * R + 0.504 * G + 0.098 * B + 16); //y
		tmp2 = ((int16_t)(-0.14713 * R - 0.28886 * G + 0.436 * B));//(-0.148 * R - 0.291 * G + 0.439 * B + 128); //u
		tmp3 = ((int16_t)(0.615 * R - 0.511499 * G - 0.10001 * B));//(0.439 * R - 0.368 * G - 0.071 * B + 128); //v

		if (tmp1 >= 255)
			YUV_image->Y_buff[i] = (uint8_t)255;
		else if (tmp1 < 0)
			YUV_image->Y_buff[i] = 0;
		else
			YUV_image->Y_buff[i] = (uint8_t)(tmp1);

		if (tmp2 >= 255)
			YUV_image->U_buff[i] = (uint8_t)255;
		else if (tmp2 < 0)
			YUV_image->U_buff[i] = 0;
		else
			YUV_image->U_buff[i] = (uint8_t)tmp2;

		if (tmp3 >= 255)
			YUV_image->V_buff[i] = (uint8_t)255;
		else if (tmp3 < 0)
			YUV_image->V_buff[i] = 0;
		else
			YUV_image->V_buff[i] = (uint8_t)tmp3;
	}
	delete RGB_Image->R_buff;
	delete RGB_Image->G_buff;
	delete RGB_Image->B_buff;

}

void convertYUVtoRGB(YUV_Image_t* YUV_image, Out_Image_t* RGB_Image)
{
	uint32_t Image_size = 0;
	Image_size = YUV_image->Y_pixels_hight* YUV_image->Y_pixels_width;
	RGB_Image->pixels_hight = YUV_image->Y_pixels_hight;
	RGB_Image->pixels_width = YUV_image->Y_pixels_width;
	RGB_Image->pixel_size = 32;

	RGB_Image->R_buff = new uint8_t[Image_size];
	RGB_Image->G_buff = new uint8_t[Image_size];
	RGB_Image->B_buff = new uint8_t[Image_size];

	for (uint32_t i = 0; i < Image_size; i = i + 1)
	{
		int16_t tmp1, tmp2, tmp3;
		float Y = YUV_image->Y_buff[i];
		float U = YUV_image->U_buff[i];
		float V = YUV_image->V_buff[i];
		tmp1 = ((int16_t)(Y + 1.13983 * V));//(1.164*Y + 1.596*V);
		tmp2 = ((int16_t)(Y - 0.39465 * U - 0.58060 * V));//(1.164*Y - 0.392*U - 0.813*V);
		tmp3 = ((int16_t)(Y + 2.03211 * U));//(1.164*Y + 2.017*U);
		if (tmp1 >= 255)
			RGB_Image->R_buff[i] = (uint8_t)255;
		else if (tmp1 <= 0)
			RGB_Image->R_buff[i] = (uint8_t)0;
		else
			RGB_Image->R_buff[i] = (uint8_t)tmp1;

		if (tmp2 >= 255)
			RGB_Image->G_buff[i] = (uint8_t)255;
		else if (tmp2 <= 0)
			RGB_Image->G_buff[i] = (uint8_t)0;
		else
			RGB_Image->G_buff[i] = (uint8_t)tmp2;

		if (tmp3 >= 255)
			RGB_Image->B_buff[i] = (uint8_t)255;
		else if (tmp3 <= 0)
			RGB_Image->B_buff[i] = (uint8_t)0;
		else
			RGB_Image->B_buff[i] = (uint8_t)tmp3;
	}
	delete YUV_image->Y_buff;
	delete YUV_image->U_buff;
	delete YUV_image->V_buff;

}

void YUV_convert444_to_avg411(YUV_Image_t* YUV_image_in, YUV_Image_t* YUV_image_out) {
	YUV_image_out->Y_pixels_hight = YUV_image_in->Y_pixels_hight;
	YUV_image_out->Y_pixels_width = YUV_image_in->Y_pixels_width;

	YUV_image_out->Y_size_bytes = YUV_image_in->Y_size_bytes;
	YUV_image_out->U_size_bytes = YUV_image_in->U_size_bytes/4;
	YUV_image_out->V_size_bytes = YUV_image_in->V_size_bytes/4;

	YUV_image_out->Y_buff = new uint8_t[YUV_image_out->Y_size_bytes];
	YUV_image_out->U_buff = new uint8_t[YUV_image_out->U_size_bytes];
	YUV_image_out->V_buff = new uint8_t[YUV_image_out->V_size_bytes];

	uint32_t UV_idx = 0;
	for (uint32_t i = 0; i < YUV_image_out->Y_size_bytes; i = i + 4)
	{
		// Y0 U0 V0 
		YUV_image_out->Y_buff[i] = YUV_image_in->Y_buff[i];
		YUV_image_out->U_buff[UV_idx] = (uint32_t)(YUV_image_in->U_buff[i] + YUV_image_in->U_buff[i + 1] + YUV_image_in->U_buff[i + 2] + YUV_image_in->U_buff[i + 3]) / 4.0;
		YUV_image_out->V_buff[UV_idx] = (uint32_t)(YUV_image_in->V_buff[i] + YUV_image_in->V_buff[i + 1] + YUV_image_in->V_buff[i + 2] + YUV_image_in->V_buff[i + 3]) / 4.0;

		// Y1 Y2 Y3
		YUV_image_out->Y_buff[i+1] = YUV_image_in->Y_buff[i + 1];
		YUV_image_out->Y_buff[i+2] = YUV_image_in->Y_buff[i + 2];
		YUV_image_out->Y_buff[i+3] = YUV_image_in->Y_buff[i + 3];

		UV_idx++;
	}
	delete YUV_image_in->Y_buff;
	delete YUV_image_in->U_buff;
	delete YUV_image_in->V_buff;

}

void YUV_convert411_to_444(YUV_Image_t* YUV_image_in, YUV_Image_t* YUV_image_out) {
	YUV_image_out->Y_pixels_hight = YUV_image_in->Y_pixels_hight;
	YUV_image_out->Y_pixels_width = YUV_image_in->Y_pixels_width;
	
	YUV_image_out->Y_size_bytes = YUV_image_in->Y_size_bytes;
	YUV_image_out->U_size_bytes = YUV_image_in->U_size_bytes * 4;
	YUV_image_out->V_size_bytes = YUV_image_in->V_size_bytes * 4;

	YUV_image_out->Y_buff = new uint8_t[YUV_image_out->Y_size_bytes];
	YUV_image_out->U_buff = new uint8_t[YUV_image_out->U_size_bytes];
	YUV_image_out->V_buff = new uint8_t[YUV_image_out->V_size_bytes];

	uint32_t UV_idx = 0;
	for (uint32_t i = 0; i < YUV_image_out->Y_size_bytes; i = i + 4)
	{
		// Y0 Y1 Y2 Y3 
		YUV_image_out->Y_buff[i]	 = YUV_image_in->Y_buff[i];
		YUV_image_out->Y_buff[i + 1] = YUV_image_in->Y_buff[i + 1];
		YUV_image_out->Y_buff[i + 2] = YUV_image_in->Y_buff[i + 2];
		YUV_image_out->Y_buff[i + 3] = YUV_image_in->Y_buff[i + 3];

		YUV_image_out->U_buff[i]	 = YUV_image_in->U_buff[UV_idx];
		YUV_image_out->U_buff[i + 1] = YUV_image_in->U_buff[UV_idx];
		YUV_image_out->U_buff[i + 2] = YUV_image_in->U_buff[UV_idx];
		YUV_image_out->U_buff[i + 3] = YUV_image_in->U_buff[UV_idx];

		YUV_image_out->V_buff[i]	 = YUV_image_in->V_buff[UV_idx];
		YUV_image_out->V_buff[i + 1] = YUV_image_in->V_buff[UV_idx];
		YUV_image_out->V_buff[i + 2] = YUV_image_in->V_buff[UV_idx];
		YUV_image_out->V_buff[i + 3] = YUV_image_in->V_buff[UV_idx];
		UV_idx++;
	}

	delete YUV_image_in->Y_buff;
	delete YUV_image_in->U_buff;
	delete YUV_image_in->V_buff;


}