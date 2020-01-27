#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

void pipe_chain(int n, char* cmd[]) {
	int read_fd = open("temp0", O_RDWR | O_CREAT | O_TRUNC, 0666);
	int write_fd = open("temp1", O_RDWR | O_CREAT | O_TRUNC, 0666);
	for (int i = 0; i < n; ++i) {
		pid_t pid = fork();
		if (pid == 0) {
			if (i != 0) dup2(read_fd, STDIN_FILENO);
			if (i != n - 1) dup2(write_fd, STDOUT_FILENO);
			close(read_fd);
			close(write_fd);
			execlp(cmd[i], cmd[i], NULL);
		}
		wait(NULL);
		close(read_fd);
		close(write_fd);
		char new_read[6];
		char new_write[6];
		sprintf(new_read, "%s%d", "temp", (i + 1) % 2);
		sprintf(new_write, "%s%d", "temp", i % 2);
		read_fd = open(new_read, O_RDWR);
		write_fd = open(new_write , O_RDWR | O_TRUNC);
	}
	close(read_fd);
	close(write_fd);
	remove("temp0");
	remove("temp1");
}

int main(int argc, char* argv[]) {
	pipe_chain(argc - 1, argv + 1);
	return 0;
}

