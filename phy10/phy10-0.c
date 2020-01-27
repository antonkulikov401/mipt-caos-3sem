#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
	int in_pipe_fd[2];
	int out_pipe_fd[2];
	pipe(in_pipe_fd);
	pipe(out_pipe_fd);
	pid_t pid = fork();
	if (pid == 0) {
		dup2(in_pipe_fd[0], STDIN_FILENO);
		dup2(out_pipe_fd[1], STDOUT_FILENO);
		close(out_pipe_fd[0]);
		close(out_pipe_fd[1]);
		close(in_pipe_fd[0]);
		close(in_pipe_fd[1]);
		execlp(argv[1], argv[1], NULL);
	}
    close(in_pipe_fd[0]);
	int input = open(argv[2], O_RDONLY);
    const int buffer_size = 4096;
    char buff[buffer_size];
	ssize_t read_bytes = 0;
	while ((read_bytes = read(input, buff, buffer_size)))
    	write(in_pipe_fd[1], buff, read_bytes);
	close(in_pipe_fd[1]);
	wait(NULL);
	close(out_pipe_fd[1]);
	ssize_t total = 0;
	while((read_bytes = read(out_pipe_fd[0], buff, buffer_size)))
		total += read_bytes;
	printf("%ld", total);
	close(out_pipe_fd[0]);
	return 0;
}

