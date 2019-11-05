#include "Image.h"
#include "Bitmap.h"


Image::Image(ImageType* p_imagetype) {
	p_imagetype->read_image(&ImageInfo);

}

void Image::createRGPfromGRAY(void)
{
	RGB_buff = new uint8_t[ImageInfo.buff_size * 3];
	for (uint32_t i = 0; i < ImageInfo.buff_size; i=i+1)
	{
		RGB_buff[3*i] = ImageInfo.buff[i];
		RGB_buff[3*i+1] = ImageInfo.buff[i];
		RGB_buff[3*i+2] = ImageInfo.buff[i];
	}
	ImageInfo.buff = RGB_buff;
	ImageInfo.buff_size = ImageInfo.buff_size * 3;
	ImageInfo.pixel_size = ImageInfo.pixel_size * 3;
}

void Image::createImagefromRGB(void)
{
	RGB_buff = new uint8_t[ImageInfo.buff_size];
	for (uint32_t i = 0; i < ImageInfo.buff_size; i=i+3) {
		RGB_buff[i]   = ImageInfo.buff[i+2];
		RGB_buff[i + 1] = ImageInfo.buff[i + 1];
		RGB_buff[i+2] = ImageInfo.buff[i];
	}
	ImageInfo.buff = RGB_buff;
}

void Image::convertRGBtoYUV(void)
{
	YUV_buff = new uint8_t[ImageInfo.buff_size];
	for (uint32_t i = 0; i < ImageInfo.buff_size; i=i+3)
	{
		int16_t tmp1, tmp2, tmp3;
		double R = RGB_buff[i];
		double G = RGB_buff[i+1];
		double B = RGB_buff[i+2];
		tmp1	= ((int16_t)(0.299	 * R + 0.587   *G + 0.114  *B));//(0.257 * R + 0.504 * G + 0.098 * B + 16); //y
		tmp2	= ((int16_t)(-0.14713* R - 0.28886 *G + 0.436  *B));//(-0.148 * R - 0.291 * G + 0.439 * B + 128); //u
		tmp3	= ((int16_t)(0.615	 * R - 0.511499*G - 0.10001*B));//(0.439 * R - 0.368 * G - 0.071 * B + 128); //v
	
		if (tmp1 >= 255)
			YUV_buff[i] = (uint8_t)255;
		else if (tmp1 < 0)
			YUV_buff[i] = 0;
		else
			YUV_buff[i] = (uint8_t)(tmp1);
	
		if (tmp2 >= 255)
			YUV_buff[i+1] = (uint8_t)255;
		else if (tmp2 < 0)
			YUV_buff[i+1] = 0;
		else 
			YUV_buff[i+1] = (uint8_t)tmp2;
		
		if (tmp3 >= 255)
			YUV_buff[i+2] = (uint8_t)255;
		else if (tmp3 < 0)
			YUV_buff[i+2] = 0;
		else
			YUV_buff[i+2] = (uint8_t)tmp3;
	}
	ImageInfo.buff = YUV_buff;
}

void Image::convertYUVtoRGB(void)
{
	for (uint32_t i = 0; i < ImageInfo.buff_size; i=i+3)
	{
		int16_t tmp1, tmp2, tmp3;
		double Y = ImageInfo.buff[i];
		double U = ImageInfo.buff[i + 1];
		double V = ImageInfo.buff[i + 2];
		tmp1 = ((int16_t)(Y				+ 1.13983*V	));//(1.164*Y + 1.596*V);
		tmp2 = ((int16_t)(Y	- 0.39465*U	- 0.58060*V	));//(1.164*Y - 0.392*U - 0.813*V);
		tmp3 = ((int16_t)(Y	+ 2.03211*U				));//(1.164*Y + 2.017*U);
		if (tmp1 >= 255)
			RGB_buff[i] = (uint8_t)255;
		else if (tmp1 <=0)
			RGB_buff[i] = (uint8_t)0;
		else
			RGB_buff[i] = (uint8_t)tmp1;

		if (tmp2 >= 255)
			RGB_buff[i+1] = (uint8_t)255;
		else if (tmp2 <= 0)
			RGB_buff[i+1] = (uint8_t)0;
		else
			RGB_buff[i+1] = (uint8_t)tmp2;

		if (tmp3 >= 255)
			RGB_buff[i+2] = (uint8_t)255;
		else if (tmp3 <= 0)
			RGB_buff[i+2] = (uint8_t)0;
		else
			RGB_buff[i+2] = (uint8_t)tmp3;
	}
	ImageInfo.buff = RGB_buff;
}

void Image::convert444to422(void) {
	uint32_t YUV_422_buf_size = (ImageInfo.buff_size / 3 + (2 * (ImageInfo.buff_size / 6)));
	YUV_buff422 = new uint8_t[YUV_422_buf_size];
	uint32_t index_422 = 0;
	for (uint32_t i = 0; i < (ImageInfo.buff_size); i = i + 6)
	{
		// Y & U even
		YUV_buff422[index_422] = ImageInfo.buff[i];
		YUV_buff422[index_422 + 1] = ImageInfo.buff[i + 1];
		
		index_422 = index_422 + 4;
	}
	index_422 = 2;
	for (uint32_t i = 3; i < (ImageInfo.buff_size); i = i + 6)
	{
		// Y & V odd
		YUV_buff422[index_422] = ImageInfo.buff[i];
		YUV_buff422[index_422 + 1] = ImageInfo.buff[i + 2];

		index_422 = index_422 + 4;
	}

}

void Image::convert422to444(void) {
	uint32_t YUV_422_buf_size = (ImageInfo.buff_size / 3 + (2 * (ImageInfo.buff_size / 6)));
	 
	uint32_t index_422 = 0;
	for (uint32_t i = 0; i < (YUV_422_buf_size); i = i + 8)
	{
		// Y & U even + odd V
		ImageInfo.buff[index_422] = YUV_buff422[i];
		ImageInfo.buff[index_422 + 1] = YUV_buff422[i + 1];
		ImageInfo.buff[index_422 + 2] = YUV_buff422[i + 2 + 1];
		
		ImageInfo.buff[index_422 + 6] = YUV_buff422[i + 4];
		ImageInfo.buff[index_422 + 6 + 1] = YUV_buff422[i + 4 + 1];
		ImageInfo.buff[index_422 + 6 + 2] = YUV_buff422[i + 2 + 1];

		index_422 = index_422 + 12;
	}
	index_422 = 3;
	for (uint32_t i = 2; i < (YUV_422_buf_size); i = i + 8)
	{
		// Y & V odd  + even U
		ImageInfo.buff[index_422] = YUV_buff422[i];
		ImageInfo.buff[index_422 + 1] = YUV_buff422[i + 2 + 1];
		ImageInfo.buff[index_422 + 2] = YUV_buff422[i + 1];

		ImageInfo.buff[index_422 + 6] = YUV_buff422[i + 4];
		ImageInfo.buff[index_422 + 6 + 1] = YUV_buff422[i + 2 + 1];
		ImageInfo.buff[index_422 + 6 + 2] = YUV_buff422[i + 4 + 1];

		index_422 = index_422 + 12;
	}
}

void Image::convert444to411(void) {
	uint32_t YUV_411_buf_size = (ImageInfo.buff_size / 3 + (2 * (ImageInfo.buff_size / 12)));
	YUV_buff411 = new uint8_t[YUV_411_buf_size];
	uint32_t index_411 = 0;
	for (uint32_t i = 0; i < (ImageInfo.buff_size); i = i + 12)
	{
		// Y0 U0 V0 
		YUV_buff411[index_411] = ImageInfo.buff[i];
		YUV_buff411[index_411+1] = ImageInfo.buff[i + 1];
		YUV_buff411[index_411+2] = ImageInfo.buff[i + 2];
	
		// Y1 Y2 Y3
		YUV_buff411[index_411+3] = ImageInfo.buff[i+3];
		YUV_buff411[index_411 + 4] = ImageInfo.buff[i + 6];
		YUV_buff411[index_411 + 5] = ImageInfo.buff[i + 9];

		index_411 = index_411 + 6;
	}

}

void Image::convert411to444(void) {
	uint32_t YUV_411_buf_size = (ImageInfo.buff_size / 3 + (2 * (ImageInfo.buff_size / 12)));

	uint32_t index_411 = 0;
	for (uint32_t i = 0; i < (YUV_411_buf_size); i = i + 6)
	{
		// Y0 U0 V0 
		ImageInfo.buff[index_411] = YUV_buff411[i];
		ImageInfo.buff[index_411 + 1] = YUV_buff411[i + 1];
		ImageInfo.buff[index_411 + 2] = YUV_buff411[i + 2];
		// Y1 U1 V1 
		ImageInfo.buff[index_411 + 3] = YUV_buff411[i+3];
		ImageInfo.buff[index_411 + 4] = YUV_buff411[i + 1];
		ImageInfo.buff[index_411 + 5] = YUV_buff411[i + 2];
		// Y2 U2 V2 
		ImageInfo.buff[index_411 + 6] = YUV_buff411[i+4];
		ImageInfo.buff[index_411 + 7] = YUV_buff411[i + 1];
		ImageInfo.buff[index_411 + 8] = YUV_buff411[i + 2];
		// Y3 U3 V3 
		ImageInfo.buff[index_411 + 9] = YUV_buff411[i+5];
		ImageInfo.buff[index_411 + 10] = YUV_buff411[i + 1];
		ImageInfo.buff[index_411 + 11] = YUV_buff411[i + 2];

		index_411 = index_411 + 12;
	}
}



void Image::convert444toavg411(void) {
	uint32_t YUV_411_buf_size = (ImageInfo.buff_size / 3 + (2 * (ImageInfo.buff_size / 12)));
	YUV_buff411 = new uint8_t[YUV_411_buf_size];
	uint32_t index_411 = 0;
	for (uint32_t i = 0; i < (ImageInfo.buff_size); i = i + 12)
	{
		// Y0 U0 V0 
		YUV_buff411[index_411] = ImageInfo.buff[i];
		YUV_buff411[index_411+1] = (uint32_t)(ImageInfo.buff[i + 1]+ ImageInfo.buff[i + 1+3]+ ImageInfo.buff[i + 1+6] + ImageInfo.buff[i + 1+9])/4;
		YUV_buff411[index_411+2] = (uint32_t)(ImageInfo.buff[i + 2] + ImageInfo.buff[i + 2 + 3] + ImageInfo.buff[i + 2 + 6] + ImageInfo.buff[i + 2 + 9]) / 4; 
	
		// Y1 Y2 Y3
		YUV_buff411[index_411+3] = ImageInfo.buff[i+3];
		YUV_buff411[index_411 + 4] = ImageInfo.buff[i + 6];
		YUV_buff411[index_411 + 5] = ImageInfo.buff[i + 9];

		index_411 = index_411 + 6;
	}

}



