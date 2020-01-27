#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	FILE* files[argc - 1];
	for (int i = 0; i < argc - 1; ++i)
		files[i] = fopen(argv[i + 1], "r");
	sigset_t signals;
	sigfillset(&signals);
	sigprocmask(SIG_BLOCK, &signals, NULL);
	char* buff = NULL;
	size_t len = 0;
	int sig;
	while (1) {
		sigwait(&signals, &sig);
		if (sig == SIGRTMIN) break;
		if (sig > SIGRTMAX || sig < SIGRTMIN) continue;
		int n = sig - SIGRTMIN - 1;
		ssize_t read_bytes = getline(&buff, &len, files[n]);
		if (read_bytes != -1) {
			printf("%s", buff);
			fflush(stdout);
		}
	}
	for (int i = 0; i < argc - 1; ++i)
		fclose(files[i]);
	free(buff);
	return 0;
}

