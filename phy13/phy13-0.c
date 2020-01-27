#include <sys/epoll.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
	int fd;
	size_t size;
} read_data;

static read_data* add_epoll_fd(int epoll_fd, int fd) {
	int flags = fcntl(fd, F_GETFL);
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);
	struct epoll_event event;
	read_data* data = (read_data*)malloc(sizeof(read_data));
	data->fd = fd;
	data->size = 0;
	event.events = EPOLLIN;
	event.data.ptr = data;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event);
	return data;
}

static void handle_event(struct epoll_event* event, int* files_left) {
	read_data* data = event->data.ptr;
	if (event->events & EPOLLIN) {
		char buff[4096];
		ssize_t read_bytes = read(data->fd, buff, sizeof(buff));
		if (read_bytes > 0) data->size += read_bytes;
		else if (read_bytes == 0) {
			close(data->fd);
			*files_left -= 1;
		}
	}
}

extern size_t read_data_and_count(size_t N, int in[N]) {
	int epoll_fd = epoll_create1(0);
	read_data** info = (read_data**)malloc(N * sizeof(read_data*));
	for (int i = 0; i < N; ++i) 
		info[i] = add_epoll_fd(epoll_fd, in[i]);
	const int max_events = 10000;
	int files_left = N;
	struct epoll_event events[max_events];
	while(files_left > 0) {
		int n = epoll_wait(epoll_fd, events, max_events, -1);
		for (int i = 0; i < n; ++i) handle_event(&events[i], &files_left);
	}
	size_t total = 0;
	for (int i = 0; i < N; ++i) total += info[i]->size;
	close(epoll_fd);
	free(info);
	return total;
}

