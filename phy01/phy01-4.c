#include <stdint.h>



int get_most_significant_bit(uint32_t u32) {
	int exponent = 31;
	for (; exponent >= 0; --exponent)
		if (u32 & ((uint32_t)1 << exponent))
			break;
	return exponent;
}

int check_int(uint32_t u32) {
	int can_be_represented = 1;
	const int float_fraction_size = 23;
	int excessive_bits = (get_most_significant_bit(u32)
		- float_fraction_size);
	if (excessive_bits > 0) {
		uint32_t mask = ~(~(uint32_t)0 << excessive_bits);
		can_be_represented = (~u32 & mask) == mask;
	}
	return can_be_represented;
}

