#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[1]));
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    signal(SIGPIPE, SIG_IGN);
    int value = 0;
    while (scanf("%d", &value) != EOF) {
        sendto(sock, &value, sizeof(value), 0,
			(struct sockaddr*)&addr, sizeof(addr));
        recvfrom(sock, &value, sizeof(value), 0, NULL, NULL);
        printf("%d ", value);
        fflush(stdout);
    }
    close(sock);
    return 0;
}

