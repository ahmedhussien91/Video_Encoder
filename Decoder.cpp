#include "Decoder.h"

void Decoder::inverse_Color_Compression(Image* input_image) {
	// convert 411 to 444..
	input_image->convert444toavg411();
	// convert YUV to RGB
	input_image->convertYUVtoRGB();
}

void Decoder::inverse_DCT_Compression(Image* input_image) {

}