#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdint.h>

int main(int argc, char* argv[]) {
    int input = open(argv[1], O_RDONLY);

    int value = 0;
	uint32_t next_pointer = 0;
    while (1) {
    	int r = read(input, &value, 4);
		read(input, &next_pointer, 4);
		if (!r) break;
		printf("%d ", value);
		if (next_pointer == 0) break;
		lseek(input, next_pointer, SEEK_SET);
	}

    close(input);
    return 0;
}

