#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	int status = 0;
	char buff[5000];
	int res = scanf("%s", buff);
	if (res == EOF) {
		printf("0\n");
		return 0;
	}
	pid_t pid = fork();
	if (pid == 0) {
		while (1) {
			res = scanf("%s", buff);
			if (res == EOF) return 0;
			pid_t pid = fork();
			if (pid != 0) {
				wait(&status);
				return WEXITSTATUS(status) + 1;
			}
		}
	}
	wait(&status);
	printf("%d\n", WEXITSTATUS(status) + 1);
	return 0;
}

