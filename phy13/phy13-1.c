#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/epoll.h>
#include <ctype.h>

volatile sig_atomic_t stop_flag = 0;

void sigterm_handler(int sig) {
	stop_flag = 1;
}

int start_server(in_port_t port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    int flags = fcntl(sock, F_GETFL);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = port;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    bind(sock, (struct sockaddr*)&addr, sizeof(addr));
    listen(sock, SOMAXCONN);
    signal(SIGPIPE, SIG_IGN);
    signal(SIGTERM, sigterm_handler);
    return sock;
}

void respond(struct epoll_event* event) {
	if (!(event->events & EPOLLIN)) return;
	const int buff_size = 8192;
	char buff[buff_size];
	int n = read(event->data.fd, buff, buff_size);
	for (int i = 0; i < n; ++i) buff[i] = toupper(buff[i]);
	dprintf(event->data.fd, "%s", buff);
	shutdown(event->data.fd, SHUT_RDWR);
	close(event->data.fd);
}

int main(int argc, char* argv[]) {
	int sock = start_server(htons(atoi(argv[1])));
	int epoll_fd = epoll_create1(0);
	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = sock;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock, &event);
	const int max_events = 10000;
	struct epoll_event events[max_events];
	while (!stop_flag) {
		int n = epoll_wait(epoll_fd, events, max_events, 100);
		for (int i = 0; i < n; ++i) {
			if (events[i].data.fd == sock) {
				struct sockaddr_in cli_addr;
    			socklen_t addr_len = sizeof(cli_addr);
    			int client = accept(sock, (struct sockaddr*)&cli_addr,
					&addr_len);
				event.data.fd = client;
				epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client, &event);	
			}
			else respond(&events[i]);
		}
	}
	close(epoll_fd);
	close(sock);
	return 0;
}

