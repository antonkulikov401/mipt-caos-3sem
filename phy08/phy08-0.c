#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

long get_file_size(int fd) {
	FILE* file = fdopen(fd, "r");
    fseek(file, 0, SEEK_END);
	return ftell(file);
}

void print_substr_entries(char* file, char* substr) {
	int fd = open(file, O_RDONLY);
	if (fd == -1) return;
	long size = get_file_size(fd);
	char* txt = mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);
	if ((long)txt == -1) return;
	char* next = strstr(txt, substr);
	while (next != NULL) {
		printf("%ld\n", next - txt);
		next = strstr(next + 1, substr);
	}
	munmap(txt, size);
	close(fd);
}

int main(int argc, char* argv[]) {
	print_substr_entries(argv[1], argv[2]);
	return 0;
}

