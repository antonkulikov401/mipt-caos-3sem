#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[2]));
	addr.sin_addr.s_addr = inet_addr(argv[1]);
	connect(sock, (struct sockaddr*)&addr, sizeof(addr));
	signal(SIGPIPE, SIG_IGN);
	int value = 0;
	while (scanf("%d", &value) != EOF) {
		if (write(sock, &value, sizeof(value)) <= 0) break;
		if (read(sock, &value, sizeof(value)) <= 0) break;
		printf("%d ", value);
		fflush(stdout);
	}
	close(sock);
	return 0;
}

