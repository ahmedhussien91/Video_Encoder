#include "DTC.h"
// CPP program to perform discrete cosine transform 
#include <bits.h> 
using namespace std;
#define pi 3.142857 
const int m = 8, n = 8;


static int dctTransform(int32_t* matrix);
static int idctTransform(int32_t* matrix);
static void apply_dct(DCT_Coeff_t* Y_DCT_Coeff);
static void apply_idct(DCT_Coeff_t* Y_DCT_Coeff);
static void rearrange_array(uint8_t* arr_1D, uint32_t size_arr_1D, DCT_Coeff_t* Y_DCT_Coeff);
static void reconstruct_array(DCT_Coeff_t* Y_DCT_Coeff, uint8_t* arr_1D, uint32_t size_arr_1D);

char DTC_Block(YUV_Image_t* YUV_Image, DCT_Coeff_t* Y_DCT_Coeff, DCT_Coeff_t* U_DCT_Coeff, DCT_Coeff_t* V_DCT_Coeff) {
	// Y slice array into 8X8 blocks
	Y_DCT_Coeff->block_hight = 8;
	Y_DCT_Coeff->block_width = 8;
	Y_DCT_Coeff->number_of_blocks = YUV_Image->Y_size_bytes / (Y_DCT_Coeff->block_hight * Y_DCT_Coeff->block_width);
	Y_DCT_Coeff->image_hight = YUV_Image->Y_pixels_hight;
	Y_DCT_Coeff->image_width = YUV_Image->Y_pixels_width;
	rearrange_array(YUV_Image->Y_buff, YUV_Image->Y_size_bytes, Y_DCT_Coeff);
	apply_dct(Y_DCT_Coeff);
	// U slice array into 8X8 blocks
	U_DCT_Coeff->block_hight = 8;
	U_DCT_Coeff->block_width = 8;
	U_DCT_Coeff->number_of_blocks = YUV_Image->Y_size_bytes / (4*U_DCT_Coeff->block_hight * U_DCT_Coeff->block_width);
	U_DCT_Coeff->image_hight = YUV_Image->Y_pixels_hight/4;
	U_DCT_Coeff->image_width = YUV_Image->Y_pixels_width/4;
	rearrange_array(YUV_Image->U_buff, YUV_Image->U_size_bytes, U_DCT_Coeff);
	apply_dct(U_DCT_Coeff);


	// V slice array into 8X8 blocks
	V_DCT_Coeff->block_hight = 8;
	V_DCT_Coeff->block_width = 8;
	V_DCT_Coeff->number_of_blocks = YUV_Image->Y_size_bytes / (4*V_DCT_Coeff->block_hight * V_DCT_Coeff->block_width);
	V_DCT_Coeff->image_hight = YUV_Image->Y_pixels_hight/4;
	V_DCT_Coeff->image_width = YUV_Image->Y_pixels_width/4;
	rearrange_array(YUV_Image->V_buff, YUV_Image->V_size_bytes, V_DCT_Coeff);
	apply_dct(V_DCT_Coeff);

	return 0;
}


char IDTC_Block(DCT_Coeff_t* Y_DCT_Coeff, DCT_Coeff_t* U_DCT_Coeff, DCT_Coeff_t* V_DCT_Coeff, YUV_Image_t* YUV_Image) {
	YUV_Image->Y_pixels_hight = Y_DCT_Coeff->image_hight;
	YUV_Image->Y_pixels_width = Y_DCT_Coeff->image_width;
	YUV_Image->Y_size_bytes = Y_DCT_Coeff->number_of_blocks * Y_DCT_Coeff->block_hight * Y_DCT_Coeff->block_width;
	YUV_Image->U_size_bytes = U_DCT_Coeff->number_of_blocks * U_DCT_Coeff->block_hight * U_DCT_Coeff->block_width;
	YUV_Image->V_size_bytes = V_DCT_Coeff->number_of_blocks * V_DCT_Coeff->block_hight * V_DCT_Coeff->block_width;

	// Y slice array into 8X8 blocks
	apply_idct(Y_DCT_Coeff);
	reconstruct_array(Y_DCT_Coeff, YUV_Image->Y_buff, YUV_Image->Y_size_bytes);
	// U slice array into 8X8 blocks
	apply_idct(U_DCT_Coeff);
	reconstruct_array(U_DCT_Coeff, YUV_Image->U_buff, YUV_Image->U_size_bytes);

	// V slice array into 8X8 blocks
	apply_idct(V_DCT_Coeff);
	reconstruct_array(V_DCT_Coeff, YUV_Image->V_buff, YUV_Image->V_size_bytes);

	return 0;
}

void rearrange_array(uint8_t* arr_1D, uint32_t size_arr_1D, DCT_Coeff_t* Y_DCT_Coeff) { // uint32_t Image_width, uint32_t block_size, uint32_t** arr_Blocks_2D) {

	//block_size = 64 (8x8)
	//image_width = 512 
	// create 64 x arr_Blocks_2D_size containers for 1d arr
	uint32_t block_size = (Y_DCT_Coeff->block_hight * Y_DCT_Coeff->block_width);
	uint32_t Image_width = Y_DCT_Coeff->image_width;
	uint32_t arr_Blocks_2D_size = (size_arr_1D / block_size);
	Y_DCT_Coeff->Coeff_buff_arr = new int32_t * [arr_Blocks_2D_size];
	for (uint32_t i = 0; i < arr_Blocks_2D_size; i++) {
		Y_DCT_Coeff->Coeff_buff_arr[i] = new int32_t[block_size];
	}

	// fill cotainer with 2d data
	for (uint32_t i = 0; i < arr_Blocks_2D_size; i++) {
		uint32_t col_offset = Image_width;
		uint32_t col_idx = -1;
		for (uint32_t j = 0; j < block_size; j++) {

			if (j % 8 == 0)
			{
				col_idx++;
			}
			Y_DCT_Coeff->Coeff_buff_arr[i][j] = arr_1D[(j%8) + (col_idx*col_offset) + (i*8)];
		}
	}
}

void reconstruct_array(DCT_Coeff_t* Y_DCT_Coeff, uint8_t* arr_1D, uint32_t size_arr_1D) { // uint32_t Image_width, uint32_t block_size, uint32_t** arr_Blocks_2D) {

	//block_size = 64 (8x8)
	//image_width = 512 
	// create 64 x arr_Blocks_2D_size containers for 1d arr
	uint32_t block_size = (Y_DCT_Coeff->block_hight * Y_DCT_Coeff->block_width);
	uint32_t Image_width = Y_DCT_Coeff->image_width;
	uint32_t arr_Blocks_2D_size = Y_DCT_Coeff->number_of_blocks;
	size_arr_1D = block_size * Y_DCT_Coeff->number_of_blocks;
	
	arr_1D = new uint8_t[size_arr_1D];

	// fill cotainer with 2d data
	for (uint32_t i = 0; i < arr_Blocks_2D_size; i++) {
		uint32_t col_offset = Image_width;
		uint32_t col_idx = -1;
		for (uint32_t j = 0; j < block_size; j++) {

			if (j % 8 == 0)
			{
				col_idx++;
			}
			arr_1D[(j % 8) + (col_idx * col_offset) + (i * 8)] = Y_DCT_Coeff->Coeff_buff_arr[i][j];
		}
	}
}

void apply_dct(DCT_Coeff_t* Y_DCT_Coeff) {
	for (uint32_t i = 0; i < Y_DCT_Coeff->number_of_blocks; i++) {
		
		dctTransform(Y_DCT_Coeff->Coeff_buff_arr[i]);
	}
}

void apply_idct(DCT_Coeff_t* Y_DCT_Coeff) {
	for (uint32_t i = 0; i < Y_DCT_Coeff->number_of_blocks; i++) {

		idctTransform(Y_DCT_Coeff->Coeff_buff_arr[i]);
	}
}


// Function to find discrete cosine transform and print it 
int dctTransform(int32_t * matrix)
{
	int i, j, k, l;

	// dct will store the discrete cosine transform 
	float dct[m][n];

	float ci, cj, dct1, sum;

	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {

			// sum will temporarily store the sum of  
			// cosine signals 
			sum = 0;
			for (k = 0; k < m; k++) {
				for (l = 0; l < n; l++) {
					dct1 = matrix[k + l*8] *
						cos((2 * k + 1) * i * pi / (2 * m)) *
						cos((2 * l + 1) * j * pi / (2 * n));
					sum = sum + dct1;
				}
			}
			dct[i][j] =  sum;
		}
	}

	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			matrix[i + j * 8] = dct[i][j];
		}
	}
	return 0;
}

// Function to find discrete cosine transform and print it 
int idctTransform(int32_t* matrix)
{
	int i, j, k, l;

	// dct will store the discrete cosine transform 
	float dct[m][n];

	float ci, cj, dct1, sum;

	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {

			// sum will temporarily store the sum of  
			// cosine signals 
			sum = 0;
			for (k = 0; k < m; k++) {
				for (l = 0; l < n; l++) {
					dct1 = matrix[k + l * 8] *
						cos((2 * k + 1) * i * pi / (2 * m)) *
						cos((2 * l + 1) * j * pi / (2 * n));
					sum = sum + dct1;
				}
			}
			dct[i][j] = (4/(m*n))*sum;
		}
	}

	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			matrix[i + j * 8] = dct[i][j];
		}
	}
	return 0;
}
