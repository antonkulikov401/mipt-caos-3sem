#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	printf("%d\n", getpid());
	fflush(stdout);
	int value = 0;
	scanf("%d", &value);
	sigset_t signals;
	sigemptyset(&signals);
	sigaddset(&signals, SIGUSR1);
	sigaddset(&signals, SIGUSR2);
	sigaddset(&signals, SIGTERM);
	sigprocmask(SIG_BLOCK, &signals, NULL);
	int sig = 0;
	while (1) {
		sigwait(&signals, &sig);
		if (sig == SIGUSR1) {
			++value;
			printf("%d\n", value);
			fflush(stdout);
		}
		if (sig == SIGUSR2) {
			value *= -1;
			printf("%d\n", value);
			fflush(stdout);
		}
		if (sig == SIGTERM) break;
	}
	return 0;
}

