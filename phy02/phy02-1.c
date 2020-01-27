#include <stdio.h>
#include <stdlib.h>

int summ(int x0, int N, int* X);

int main() {
	int* x = (int*)malloc(3*sizeof(int));
	x[0] = 1;
	x[1] = 2;
	x[2] = -10;
	int x0 = 5;
	printf("%d\n", summ(x0, 3, x));
	free(x);
	return 0;
}
