#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	int x;
	int y;
	float z;
	char* y_str = (char*)malloc(100);
	x = strtol(argv[1], 0, 27);
	scanf("%f", &z);
	scanf("%s", y_str);
	y = strtol(y_str, 0, 16);
	printf("%.3f", x + y + z);
	free(y_str);
	return 0;
}
