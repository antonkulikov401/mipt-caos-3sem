#include <stdio.h>
#include <stdlib.h>

extern float dot_product(int N, const float* A, const float* B);

int main() {
	int N = 6;
	float* A = (float*)malloc(4*N);
	float* B = (float*)malloc(4*N);
	A[0] = 1;
	//A[1] = 2;
	//A[2] = 3;
	//A[3] = 4;
	//A[4] = 2;
	//A[5] = 3;
	B[0] = 2;
	//B[1] = 4;
	//B[2] = 6;
	//B[3] = 8;
	//B[4] = 4;
	//B[5] = 5;
	printf("%f", dot_product(1, A, B));
	return 0;
}
