#include "Ecoder.h"
// CPP program to perform discrete cosine transform 
#include <bits.h> 
using namespace std;
#define pi 3.142857 




void Ecoder::Color_Compression(Image* input_image)
{
	// convert RGB image to YUV
	input_image->convertRGBtoYUV();
	// convert 444 to 422 
	input_image->convert444toavg411();

}

void Ecoder::DCT_Compression(Image* input_image, Dct_Coeff_t *dct_coeff_out)
{
	uint32_t Pixel_size_in_bytes = (input_image->ImageInfo.pixel_size / 8);
	uint32_t R_image_size = input_image->ImageInfo.buff_size / Pixel_size_in_bytes;
	uint32_t width = (R_image_size / 2) * (6 / 4);

	uint32_t dct_coeff_Index_y = 0;
	uint32_t dct_coeff_Index_u = 0;
	uint32_t dct_coeff_Index_v = 0;
	dct_coeff_out->Y_coeff_buff= new float[R_image_size];
	dct_coeff_out->U_coeff_buff= new float[R_image_size/4];
	dct_coeff_out->V_coeff_buff= new float[R_image_size/4];
	dct_coeff_out->Size = R_image_size;

	uint32_t col_i = 0;
	uint32_t row_i = 0;
	for (uint32_t idx = 0; idx < R_image_size/64-1; idx++)
	{
		uint8_t temp[8][8];
		for (uint8_t j = 0; j < 8; j++)
		{
			for (uint8_t i = 0; i < 12; i++)
			{
				uint8_t index=0;
				if ((i % 6 != 1) && (i % 6 != 2)) {
					temp[j][index] = input_image->ImageInfo.buff[(row_i + j)*width + col_i + i];
					index++;
				}
			}
		}
		float dct[DCT_block_hight][DCT_block_width];
		(void)dctTransform(temp, dct);
		col_i = (col_i + 12)% width;
		if (col_i == 0)
			row_i = row_i + 8;

	}
}

// Function to find discrete cosine transform and print it 
int Ecoder::dctTransform(uint8_t matrix[][DCT_block_width], float dct[][DCT_block_width])
{
	int i, j, k, l;

	// dct will store the discrete cosine transform 
	//float dct[DCT_block_hight][DCT_block_width];

	float ci, cj, dct1, sum;

	for (i = 0; i < DCT_block_hight; i++) {
		for (j = 0; j < DCT_block_width; j++) {

			// ci and cj depends on frequency as well as 
			// number of row and columns of specified matrix 
			if (i == 0)
				ci = 1 / sqrt(DCT_block_hight);
			else
				ci = sqrt(2) / sqrt(DCT_block_hight);
			if (j == 0)
				cj = 1 / sqrt(DCT_block_width);
			else
				cj = sqrt(2) / sqrt(DCT_block_width);

			// sum will temporarily store the sum of 
			// cosine signals 
			sum = 0;
			for (k = 0; k < DCT_block_hight; k++) {
				for (l = 0; l < DCT_block_width; l++) {
					dct1 = matrix[k][l] *
						cos((2 * k + 1) * i * pi / (2 * DCT_block_hight)) *
						cos((2 * l + 1) * j * pi / (2 * DCT_block_width));
					sum = sum + dct1;
				}
			}
			dct[i][j] = ci * cj * sum;
		}
	}

	for (i = 0; i < DCT_block_hight; i++) {
		for (j = 0; j < DCT_block_width; j++) {
			printf("%f\t", dct[i][j]);
		}
		printf("\n");
	}
	return 1;
}

