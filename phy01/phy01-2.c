#include <stdint.h>
#include <stdbool.h>



typedef enum {
	PlusZero	  = 0x00,
	MinusZero	  = 0x01,
	PlusInf		  = 0xF0,
	MinusInf	  = 0xF1,
	PlusRegular   = 0x10,
	MinusRegular  = 0x11,
	PlusDenormal  = 0x20,
	MinusDenormal = 0x21,
	SignalingNaN  = 0x30,
	QuietNaN	  = 0x31
} float_class_t;

const uint64_t gk_exponent_size = 11;
const uint64_t gk_fraction_size = 52;

bool is_negative(uint64_t* value) {
	return *value >> (gk_exponent_size + gk_fraction_size) == 1;
}

bool is_exponent_filled_with_zeroes(uint64_t* value) {
	return (*value << 1) >> (gk_fraction_size + 1) == 0; 
}

bool is_exponent_filled_with_ones(uint64_t* value) {
	return (*value << 1) >> (gk_fraction_size + 1) == 
		~(~(uint64_t)0 << gk_exponent_size);
}
	
bool is_fraction_filled_with_zeroes(uint64_t* value) {
	return *value << (gk_exponent_size + 1) == 0; 
}

bool is_first_fraction_digit_one(uint64_t* value) {
	return ((*value << (gk_exponent_size + 1)) >> 
		(gk_exponent_size + gk_fraction_size)) == 1;
}

float_class_t classify(double* value_ptr) {
	uint64_t* x = (uint64_t*)value_ptr;
	float_class_t float_class = PlusRegular;

	if (is_exponent_filled_with_ones(x)) {
		if (is_fraction_filled_with_zeroes(x)) {
			if (is_negative(x))
				float_class = MinusInf;
			else
				float_class = PlusInf;
		}
		else if (is_first_fraction_digit_one(x))
			float_class = QuietNaN;
		else
			float_class = SignalingNaN;
	}

	else if (is_exponent_filled_with_zeroes(x)) {
		if (is_fraction_filled_with_zeroes(x)) {
			if (is_negative(x))
				float_class = MinusZero;
			else
				float_class = PlusZero;
		}
		else if (is_negative(x))
			float_class = MinusDenormal;
		else
			float_class = PlusDenormal;
	}

	else if (is_negative(x))
		float_class = MinusRegular;

	return float_class;
}

