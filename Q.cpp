#include "ImR.h"
#include "DTC.h"
#include "Q.h"

#define BLOCK_SIZE 64 // 8*8

float Q_matrix[BLOCK_SIZE] = { 16, 11, 10, 16, 24 , 40 , 51 , 61,
							 12, 12, 14, 19, 26 , 58 , 60 , 55,
							 14, 13, 16, 24, 40 , 57 , 69 , 56,
							 14, 17, 22, 29, 51 , 87 , 80 , 62,
							 18, 22, 37, 56, 68 , 109, 103, 77,
							 24, 35, 55, 64, 81 , 104, 113, 92,
							 49, 64, 78, 87, 103, 121, 120, 101,
							 72, 92, 95, 98, 112, 100, 103, 99
};

uint8_t Zigzag_index[BLOCK_SIZE] = { 1 , 2 , 9 , 3 , 10, 17, 4, 11,
						18, 25, 5 , 12, 19, 26, 33,6 ,
						12, 20, 27, 34, 41, 7, 14, 21,
						28, 35, 42, 49, 8, 15, 22, 29,
						36, 43, 50, 57,16, 23, 30, 37,
						44, 51, 58, 24, 31, 38, 45, 52,
						59, 32, 39, 46, 53, 60, 40, 47,
						54, 61, 48, 58, 62, 56, 63, 64
};

static void Q(int32_t* arr);
static void dQ(int32_t* arr);
static void run_length_encoding(DCT_Coeff_t* Y_DCT_Coeff, uint8_t* stream, uint32_t* stream_len);
static void run_length_decoding(Stream_t* stream, DCT_Coeff_t* Y_DCT_Coeff);

// Quantization_Block
char Quantization_Block(DCT_Coeff_t* Y_DCT_Coeff, DCT_Coeff_t* U_DCT_Coeff, DCT_Coeff_t* V_DCT_Coeff, Stream_t *stream) {
	stream->image_hight = Y_DCT_Coeff->image_hight;
	stream->image_width = Y_DCT_Coeff->image_width;

	// Y_DCT_Coeff quantization 
	if (Y_DCT_Coeff->block_hight * Y_DCT_Coeff->block_width != BLOCK_SIZE)
		_ASSERT_EXPR(false, "Size Mismatch");
	for (uint32_t i = 0; i < Y_DCT_Coeff->number_of_blocks; i++) {
		// Quantization (dividing by matrix)
		Q(Y_DCT_Coeff->Coeff_buff_arr[i]);
#ifdef Debug
		printf("\nBlock: %d\n", i+1);
#endif //Debug
	}
	// from normal to zigzag index transformation
	// run length encoding (uint8_t ->count, uint8_t -> variable) image hight and width to be sent in the end 
	
	//stream->y_block_stream = new uint8_t[Y_DCT_Coeff->number_of_blocks*BLOCK_SIZE];
	//run_length_encoding(Y_DCT_Coeff, stream->y_block_stream, &stream->y_stream_len);

	// U_DCT_Coeff quantization 
	if (U_DCT_Coeff->block_hight * U_DCT_Coeff->block_width != BLOCK_SIZE)
		_ASSERT_EXPR(false, "Size Mismatch");
	for (uint32_t i = 0; i < U_DCT_Coeff->number_of_blocks; i++) {
		// Quantization (dividing by matrix)
		Q(U_DCT_Coeff->Coeff_buff_arr[i]);
	}
	// from normal to zigzag index transformation
	// run length encoding (uint8_t ->count, uint8_t -> variable) image hight and width to be sent in the end 

	//stream->u_block_stream = new uint8_t[U_DCT_Coeff->number_of_blocks*BLOCK_SIZE];
	//run_length_encoding(U_DCT_Coeff, stream->u_block_stream, &stream->u_stream_len);

	// V_DCT_Coeff quantization 
	if (V_DCT_Coeff->block_hight * V_DCT_Coeff->block_width != BLOCK_SIZE)
		_ASSERT_EXPR(false, "Size Mismatch");
	for (uint32_t i = 0; i < V_DCT_Coeff->number_of_blocks; i++) {
		// Quantization (dividing by matrix)
		Q(V_DCT_Coeff->Coeff_buff_arr[i]);
	}
	// from normal to zigzag index transformation
	// run length encoding (uint8_t ->count, uint8_t -> variable) image hight and width to be sent in the end 

	//stream->v_block_stream = new uint8_t[V_DCT_Coeff->number_of_blocks*BLOCK_SIZE];
	//run_length_encoding(V_DCT_Coeff, stream->v_block_stream, &stream->v_stream_len);


	return 0;
}
// deQuantization_Block
char deQuantization_Block(Stream_t *stream, DCT_Coeff_t* Y_DCT_Coeff, DCT_Coeff_t* U_DCT_Coeff, DCT_Coeff_t* V_DCT_Coeff) {
	Y_DCT_Coeff->image_hight = stream->image_hight;
	Y_DCT_Coeff->image_width = stream->image_width;

	// from  zigzag to normal indexing transformation 
	// run length decoding (block identification = 00) extract number of blocks


	// deQuantzation ( Multiplying by the matrix)
	if (Y_DCT_Coeff->block_hight * Y_DCT_Coeff->block_width != BLOCK_SIZE)
		_ASSERT_EXPR(false, "Size Mismatch");
	for (uint32_t i = 0; i < Y_DCT_Coeff->number_of_blocks; i++) {
		dQ(Y_DCT_Coeff->Coeff_buff_arr[i]);
	}


	// from  zigzag to normal indexing transformation 
	// run length decoding (block identification = 00) extract number of blocks


	// deQuantzation ( Multiplying by the matrix)
	if (U_DCT_Coeff->block_hight * U_DCT_Coeff->block_width != BLOCK_SIZE)
		_ASSERT_EXPR(false, "Size Mismatch");
	for (uint32_t i = 0; i < U_DCT_Coeff->number_of_blocks; i++) {
		dQ(U_DCT_Coeff->Coeff_buff_arr[i]);
	}




	// from  zigzag to normal indexing transformation 
	// run length decoding (block identification = 00) extract number of blocks


	// deQuantzation ( Multiplying by the matrix)
	if (V_DCT_Coeff->block_hight * V_DCT_Coeff->block_width != BLOCK_SIZE)
		_ASSERT_EXPR(false, "Size Mismatch");
	for (uint32_t i = 0; i < V_DCT_Coeff->number_of_blocks; i++) {
		dQ(V_DCT_Coeff->Coeff_buff_arr[i]);
	}






	return 0;
}

void Q(int32_t* arr)
{
	for (uint8_t i = 0; i < BLOCK_SIZE; i++) {
		arr[i] = (arr[i] / Q_matrix[i]) + 0.5;
#ifdef Debug
		printf("%d, ", arr[i]);
		if (i % 8 == 0)
			printf("\n");
#endif // Debug
	}
}

void dQ(int32_t* arr) {
	for (uint8_t i = 0; i < BLOCK_SIZE; i++) {
		arr[i] = arr[i] * Q_matrix[i];
	}
}

void run_length_encoding(DCT_Coeff_t* Y_DCT_Coeff, uint8_t * stream, uint32_t * stream_len) {
	*stream_len = 0;



}

void run_length_decoding(Stream_t *stream, DCT_Coeff_t* Y_DCT_Coeff) {
	
}