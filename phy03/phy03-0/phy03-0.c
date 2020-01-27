#include <stdio.h>
#include "phy03-0-constants.c"

int calculate();
extern uint32_t R;
uint32_t A = 10;
uint32_t B = 2;
uint32_t C = 30;
uint32_t D = 3;

int main() {
	calculate();
	printf("%u", R);
	return 0;
}
