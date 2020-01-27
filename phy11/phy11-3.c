#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	sigset_t signals;
	sigfillset(&signals);
	sigprocmask(SIG_BLOCK, &signals, NULL);
	siginfo_t info;
	while (1) {
		sigwaitinfo(&signals, &info);
		if (info.si_signo == SIGRTMIN) {
			int n = info.si_value.sival_int;
			if (n == 0) break;
			info.si_value.sival_int--;
			sigqueue(info.si_pid, info.si_signo, info.si_value);
		}
	}
	return 0;
}

