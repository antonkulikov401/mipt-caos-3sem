#include <stdio.h>
#include <stdlib.h>

extern void summ(int N, const int *A, const int *B, int *R);

int main() {
	const int N = 2;
	int* A = (int*)malloc(N*sizeof(int));
	int* B = (int*)malloc(N*sizeof(int));
	int* R = (int*)malloc(N*sizeof(int));
	A[0] = 2;
	A[1] = -1;
	B[0] = 3;
	B[1] = 4;
	summ(N, A, B, R);
	for (int i = 0; i < N; ++i)
		printf("%d\n", R[i]);
	return 0;
}
