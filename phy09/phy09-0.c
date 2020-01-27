#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char* argv[]) {
	const int n = atoi(argv[1]);
	int i = 1;
	for (; i < n; ++i) {
		pid_t pid = fork();
		if (pid != 0) {
			wait(NULL);
			break;
		}
	}
	char tmp[16];
	printf("%d%c", n - i + 1, i == 1 ? '\n' : ' ');
	return 0;
}

