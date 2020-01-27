#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void fill(int x, int n, int i, int j, int w, char* matrix) {
	char tmp[w + 2];
	sprintf(tmp, "%*d\n", w, x);
	strncpy(matrix + (n * w + 1) * i + j * w, tmp, (j < n - 1 ? w : w + 1));
}

void generate_spiral_matrix(char* file, uint32_t n, uint32_t w) {
	int fd = open(file, O_RDWR | O_CREAT, 0666);
	char* matrix = mmap(NULL, w * n * n, PROT_READ | PROT_WRITE,
		MAP_SHARED, fd, 0);
	posix_fallocate(fd, 0, w * n * n + n);
	for (int j = 0; j < w * n * n; ++j) matrix[j] = ' ';
	int start_row = 0, end_row = n - 1, start_col = 0, end_col = n - 1;
	int i = 1;
	while (start_row <= end_row && start_col <= end_col) {
		for (int j = start_col; j <= end_col; ++j)
			fill(i++, n, start_row, j, w, matrix);
		++start_row;
		for (int j = start_row; j <= end_row; ++j)
			fill(i++, n, j, end_col, w, matrix);
		--end_col;
		for (int j = end_col; j >= start_col; --j)
			fill(i++, n, end_row, j, w, matrix);
		--end_row;
		for (int j = end_row; j >= start_row; --j)
			fill(i++, n, j, start_col, w, matrix);
		++start_col;
	}
	munmap(matrix, w * n * n);
	close(fd);
}

int main(int argc, char* argv[]) {
	generate_spiral_matrix(argv[1], atoi(argv[2]), atoi(argv[3]));
	return 0;
}

