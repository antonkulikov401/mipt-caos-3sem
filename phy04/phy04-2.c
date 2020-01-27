#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern void mergesort(int from, int to, const int *in, int *out);

int main() {
	int size = 9;
	int from = 1;
	int to = 8;
	int* in = (int*)malloc(4*size);
	int* out = (int*)malloc(4*(to-from));
	memset(out, 111, 4*(to-from));	

	in[0] = 9;
	in[1] = 7;
	in[2] = 3;
	in[3] = 42;
	in[4] = 54;
	in[5] = 6;
	in[6] = 23;
	in[7] = 7;
	in[8] = 1;

	mergesort(from, to, in, out);
	for (int i = 0; i < (to-from); ++i)
		printf("%d ", out[i]);
	printf("\n");
	return 0;
}
