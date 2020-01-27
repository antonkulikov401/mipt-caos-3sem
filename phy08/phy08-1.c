#include <stdio.h>
#include <sys/mman.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

typedef struct Item
{
  int value;
  uint32_t next_pointer;
} item;

int64_t get_file_size(int fd) {
	FILE* file = fdopen(fd, "r");
    fseek(file, 0, SEEK_END);
	return ftell(file);
}

int main(int argc, char* argv[]) {
	int fd = open(argv[1], O_RDONLY);
	if (fd == -1) return 0;
	int64_t size = get_file_size(fd);
	char* items = mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);
	if ((int64_t)items == -1) return 0;
	uint32_t pos = 0;
	do {
		printf("%d ", ((item*)(items + pos))->value);
		pos = ((item*)(items + pos))->next_pointer;
	} while(pos != 0);
	munmap(items, size);
	close(fd);
	return 0;
}

