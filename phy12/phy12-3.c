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
	signal(SIGINT, sigterm_handler);
	return sock;
}

void respond(int client, char* root) {
	const int buff_size = 8192;
	char buff[buff_size];
	read(client, buff, buff_size);
	int p = strstr(buff, "HTTP/1.1") - buff - 5;
	char file_name[buff_size];
	strncpy(file_name, buff + 4, buff_size);
	file_name[p] = 0;
	char path[buff_size];
	strcpy(path, root);
	strcat(path, "/");
	strcat(path, file_name);
	int fd = open(path, O_RDONLY);
	if (fd == -1) {
		if (access(path, F_OK) == -1)
			dprintf(client,
				"HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n");
		else if (access(path, R_OK) == -1)
			dprintf(client,
				"HTTP/1.1 403 Forbidden\r\nContent-Length: 0\r\n\r\n");
		return;
	}
	struct stat st;
	lstat(path, &st);
	dprintf(client, "HTTP/1.1 200 OK\r\n");
	if (!(st.st_mode & S_IXUSR)) {
		dprintf(client, "Content-Length: %ld\r\n\r\n", st.st_size);
		int n = 0;
		while ((n = read(fd, buff, buff_size)) > 0) write(client, buff, n);
		return;
	}
	pid_t pid = fork();
	if (pid == 0) {
		dup2(client, STDOUT_FILENO);
		execlp(path, path, NULL);
	}
	wait(NULL);
}

int main(int argc, char* argv[]) {
	int sock = start_server(htons(atoi(argv[1])));
	struct sockaddr_in cli_addr;
	socklen_t addr_len = sizeof(cli_addr);
	while (!stop_flag) {
		int client = accept(sock, (struct sockaddr*)&cli_addr, &addr_len);
		if (client == -1) continue;
		respond(client, argv[2]);
		shutdown(client, SHUT_RDWR);
		close(client);
	}
	close(sock);
	return 0;
}

