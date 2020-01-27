#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
	int input = open(argv[1], O_RDONLY);
	if (input == -1) return 1;
	int output_numbers = open(argv[2], O_CREAT | O_WRONLY, 0666);
	int output_other = open(argv[3], O_CREAT | O_WRONLY, 0666);
	if (output_numbers == -1 || output_other == -1) return 2;
	
	char buff;
	ssize_t read_bytes = 0;
	while (read_bytes = read(input, &buff, 1)) {
		ssize_t write_result = 0;
		if (isdigit(buff))
			write_result = write(output_numbers, &buff, 1);
		else
			write_result = write(output_other, &buff, 1);
		if (read_bytes == -1 || write_result == -1) return 3;
	}
	
	close(input);
	close(output_numbers);
	close(output_other);
	return 0;
}

