#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]) {
	const int buffer_size = 8192;
	char expr[buffer_size];
	fgets(expr, buffer_size, stdin);
	char cmd[buffer_size];
	sprintf(cmd, "print(%s)", expr);
	execlp("python3", "python3", "-c", cmd, NULL);
	return 0;
}

