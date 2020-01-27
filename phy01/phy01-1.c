uint16_t satsum(uint16_t x, uint16_t y) {
	uint16_t u = x + y;
	if (u < x)
		u = ~0;
	return u;
}
