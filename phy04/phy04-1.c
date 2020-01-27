#include <stdlib.h>
#include <stdio.h>
#include <math.h>



extern double my_sin_test(double x);

extern double my_sin(double x);

int main() {
	for (double x = -5; x <= 5; x += 0.01) {
		double s = my_sin_test(x);
		double m = my_sin(x);
		double e = sin(x);
		printf("sin(x)    = %a\nmy_sin(x) = %a\n", s, m);
		printf("do they equal (my and test): %d\n", s == m);
		printf("do they equal (test and etalon): %d\n", m == e);
		printf("-------------\n");
	}
	return 0;
}
