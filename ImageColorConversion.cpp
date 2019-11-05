// ImageColorConversion.cpp : This file contains the 'main' function. Program execution begins and ends there.
// 
#define _CRT_SECURE_NO_DEPRECATE
#include<opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>  
#include <iostream>
#include "Bitmap.h"
#include "Image.h"
#include "ImageType.h"
#include "ImageColorConversion.h"
#include "Ecoder.h"
#include "Decoder.h"
#include "ImR.h"
#include "CC.h"
#include "DTC.h"
#include "Q.h"
#include <math.h>       /* log */

void write_raw_image(ImageInfo_t* imageinfo, const char *filename)
{
	FILE* file;
	file = fopen(filename, "wb");
	fwrite(imageinfo->buff, imageinfo->buff_size, 1, file);
	fclose(file);
}

void write_raw_image(Out_Image_t* imageinfo, const char* filename)
{
	FILE* file;
	file = fopen(filename, "wb");
	uint32_t image_size = imageinfo->pixels_hight * imageinfo->pixels_width * 3;
	uint8_t * buff = new uint8_t[image_size];
	uint32_t idx = 0;
	for (uint32_t i = 0; i < image_size; i=i + 3)
	{
		buff[i] = imageinfo->R_buff[idx];
		buff[i+1] = imageinfo->G_buff[idx];
		buff[i+2] = imageinfo->B_buff[idx];
		idx++;
	}
	fwrite(buff, image_size, 1, file);
	fclose(file);
}

void Display_raw(ImageInfo_t* imageinfo, const char* info)
{
	// read an image
	cv::Mat image;
	if ((imageinfo->pixel_size / 8) == 3)
		image = cv::Mat(imageinfo->hight, imageinfo->width, CV_8UC3, (uint8_t*)imageinfo->buff);
	else if ((imageinfo->pixel_size / 8) == 1)
		image = cv::Mat(imageinfo->hight, imageinfo->width, CV_8UC1, (uint8_t*)imageinfo->buff);
	// create image window named "My Image"
	cv::namedWindow(info, 0);
	// show the image on window
	cv::imshow("My Image", image);
	// wait key for 5000 ms
	cv::waitKey(5000);

}

void calculateSNR(ImageInfo_t* original_image, ImageInfo_t* Processed_Image, ImageInfo_t* Diff_Image, const char* info)
{
	uint32_t sumOfSquareError = 0, MeanSquareError=0;
	int32_t tmp=0;
	for (uint32_t i = 0; i < original_image->buff_size; i++)
	{
		tmp = (int32_t)original_image->buff[i] - (uint32_t)Processed_Image->buff[i];
		if (tmp >= 0) {
			Diff_Image->buff[i] = tmp;
		}
		else {
			Diff_Image->buff[i] = -1 * tmp;
		}
		sumOfSquareError = (Diff_Image->buff[i] * Diff_Image->buff[i]) + sumOfSquareError;
	}
	MeanSquareError = 10 * log10((255 * 255) / (sumOfSquareError/((double)original_image->hight*original_image->width*(original_image->pixel_size/8))));
	std::cout << "MeanSquareError of " << info << ": " << MeanSquareError << "\n";
}


int main()
{
	///* read Bitmap image - Diff Image */
	//Bitmap* Imagetype0 = new Bitmap("barbara_gray.bmp");
	//Image image_diff = Image((ImageType*)Imagetype0);
	//// test Display Image using OpenCV
	//// Generate RGB
	//image_diff.createRGPfromGRAY();

	///* read Bitmap image - Original Image */
	//Bitmap* Imagetype = new Bitmap("barbara_gray.bmp");
	//Image image_orig = Image((ImageType*)Imagetype);
	//// test Display Image using OpenCV
	//// Generate RGB
	//image_orig.createRGPfromGRAY();
	//write_raw_image(&image_orig.ImageInfo, "barbara_gray_ORIG");

	///* AVG Conversion*/
	//Bitmap* Imagetype4 = new Bitmap("barbara_gray.bmp");
	//Image image4 = Image((ImageType*)Imagetype4);
	//// Generate RGB
	//image4.createRGPfromGRAY();

	//Ecoder encoder;
	//Decoder decoder;
	//
	//// CC 
	//encoder.Color_Compression(&image4);
	//// DCT
	//Dct_Coeff_t dct_coeff_out;
	//encoder.DCT_Compression(&image4, &dct_coeff_out);
	////IDCT
	//decoder.inverse_DCT_Compression(&image4);
	//// ICC
	//decoder.inverse_Color_Compression(&image4);

	//write_raw_image(&image4.ImageInfo, "Original_RGB_avg411");
	//calculateSNR(&image_orig.ImageInfo, &image4.ImageInfo, &image_diff.ImageInfo, "Original_RGB_avg411");
	//write_raw_image(&image_diff.ImageInfo, "Diff_RGB_avg411");

	/////////////////////////////////////////////////////////////////////////////////////////////////
	///* read Bitmap image - Diff Image */
	//Bitmap* Imagetype0_l = new Bitmap("lena_color.bmp");
	//Image image_diff_l = Image((ImageType*)Imagetype0_l);
	//// test Display Image using OpenCV
	//// Generate RGB
	//image_diff_l.createImagefromRGB();

	///* read Bitmap image - Original Image */
	//Bitmap* Imagetype_l = new Bitmap("lena_color.bmp");
	//Image image_orig_l = Image((ImageType*)Imagetype_l);
	//// test Display Image using OpenCV
	//// Generate RGB
	//image_orig_l.createImagefromRGB();
	//write_raw_image(&image_orig_l.ImageInfo, "Original_RGB_avg411lena_color_ORIG");

	///* AVG Conversion*/
	//Bitmap* Imagetype4_l = new Bitmap("lena_color.bmp");
	//Image image4_l = Image((ImageType*)Imagetype4_l);
	//// Generate RGB from Gray Image
	//image4_l.createImagefromRGB();

	//Ecoder encoder_l;
	//Decoder decoder_l;
	//// CC
	//encoder_l.Color_Compression(&image4_l);
	//// DCT
	//Dct_Coeff_t dct_coeff;
	//encoder_l.DCT_Compression(&image4_l, &dct_coeff);
	//// IDCT
	//decoder_l.inverse_DCT_Compression(&image4_l);
	//// ICC
	//decoder_l.inverse_Color_Compression(&image4_l);
	//
	//write_raw_image(&image4_l.ImageInfo, "Original_RGB_avg411lena_color");
	//calculateSNR(&image_orig_l.ImageInfo, &image4_l.ImageInfo, &image_diff_l.ImageInfo, "Original_RGB_411lena_color");
	//write_raw_image(&image_diff_l.ImageInfo, "Diff_RGB_avg411lena_color");
	////calculate SNR 

	// Read Image
	Out_Image_t lena_color_out_image;
	Out_Image_t barbara_gray_out_image;
	ImR_ImageRead("lena_color.bmp", "Bitmap", &lena_color_out_image);
	ImR_ImageRead("barbara_gray.bmp", "Bitmap", &barbara_gray_out_image);
	write_raw_image(&lena_color_out_image, "lena_color_out_image");
	write_raw_image(&barbara_gray_out_image, "barbara_gray_out_image");
	// CC_Block
	YUV_Image_t lena_color_YUV;
	YUV_Image_t barbara_gray_YUV;
	CC_Block(&lena_color_out_image, &lena_color_YUV, "411_AVG");
	CC_Block(&barbara_gray_out_image, &barbara_gray_YUV, "411_AVG");
	
	// DCT_Block
	DCT_Coeff_t Y_DCT_Coeff;
	DCT_Coeff_t U_DCT_Coeff;
	DCT_Coeff_t V_DCT_Coeff;
	DTC_Block(&lena_color_YUV, &Y_DCT_Coeff, &U_DCT_Coeff, &V_DCT_Coeff);
	DTC_Block(&barbara_gray_YUV, &Y_DCT_Coeff, &U_DCT_Coeff, &V_DCT_Coeff);

	// Quantization_Block
	Stream_t stream;
	Quantization_Block(&Y_DCT_Coeff, &U_DCT_Coeff, &V_DCT_Coeff, &stream);
	// deQuantization_Block
	deQuantization_Block(&stream, &Y_DCT_Coeff, &U_DCT_Coeff, &V_DCT_Coeff);

	//IDCT_Block
	IDTC_Block(&Y_DCT_Coeff, &U_DCT_Coeff, &V_DCT_Coeff, &lena_color_YUV);
	IDTC_Block(&Y_DCT_Coeff, &U_DCT_Coeff, &V_DCT_Coeff, &barbara_gray_YUV);


	// ICC_Block
	ICC_Block(&lena_color_YUV, &lena_color_out_image, "411_AVG");
	ICC_Block(&barbara_gray_YUV, &barbara_gray_out_image, "411_AVG");
	write_raw_image(&lena_color_out_image, "lena_color_out_image_afterICC");
	write_raw_image(&barbara_gray_out_image, "barbara_gray_out_image_afterICC");
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
