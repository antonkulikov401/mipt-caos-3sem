#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	printf("%d\n", getpid());
	fflush(stdout);
	sigset_t signals;
	sigemptyset(&signals);
	sigaddset(&signals, SIGINT);
	sigaddset(&signals, SIGTERM);
	sigprocmask(SIG_BLOCK, &signals, NULL);
	int sigint_count = 0;
	int sig = 0;
	while (1) {
		sigwait(&signals, &sig);
		if (sig == SIGINT) ++sigint_count;
		if (sig == SIGTERM) {
			printf("%d\n", sigint_count);
			break;
		}
	}
	return 0;
}

