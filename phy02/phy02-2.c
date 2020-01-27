#include <stdio.h>

int solve(int A, int B, int C, int D);

int main() {
	int a, b, c, d;
	scanf("%d %d %d %d", &a, &b, &c, &d);
	printf("%d\n", solve(a, b, c, d));
	return 0;
}
