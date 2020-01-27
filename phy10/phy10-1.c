#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <sys/types.h>
#include <limits.h>

typedef enum error_type { none, error, warning } error_type;

typedef struct vector {
	int* data;
	int size;
	int cap;
} vector;

void init_vector(vector* vec) {
	vec->size = 0;
	vec->cap = 16;
	vec->data = (int*)malloc(vec->cap * sizeof(int));
	memset(vec->data, 0, vec->cap * sizeof(int));
}

void push_back(vector* vec, int x) {
	if (vec->size == vec->cap) {
		vec->cap *= 2;
		int* buff = realloc(vec->data, vec->cap * sizeof(int));
		vec->data = buff;
	}
	vec->data[vec->size++] = x;
}

int find(vector* vec, int x) {
	for (size_t i = 0; i < vec->size; ++i)
		if (vec->data[i] == x) return 1;
	return 0;
}

void parse_gcc_string(char* str, error_type* err_type, size_t* line) {
	regex_t err_regex;
	regex_t war_regex;
	regcomp(&err_regex, "^:[0-9]+:[0-9]+: (error|fatal error):", REG_EXTENDED);
	regcomp(&war_regex, "^:[0-9]+:[0-9]+: warning:", REG_EXTENDED);
	int tmp;
	if (!regexec(&err_regex, str, 0, NULL, 0)) {
		*err_type = error;
		sscanf(str, ":%ld:%d:", line, &tmp);
	}
	else if (!regexec(&war_regex, str, 0, NULL, 0)) {
		*err_type = warning;
		sscanf(str, ":%ld:%d:", line, &tmp);
	}
	else {
		*err_type = none;
		*line = 0;
	}
	regfree(&err_regex);
	regfree(&war_regex);
}

int main(int argc, char* argv[]) {
	int pipe_fd[2];
	pipe(pipe_fd);
	pid_t pid = fork();
	if (pid == 0) {
		dup2(pipe_fd[1], STDERR_FILENO);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		execlp("gcc", "gcc", argv[1], NULL);
	}
	close(pipe_fd[1]);
	wait(NULL);
	size_t err_count = 0;
	size_t war_count = 0;
	char* buff = NULL;
	size_t zero = 0;
	FILE* output = fdopen(pipe_fd[0], "r");
	vector errors;
	vector warnings;
	init_vector(&errors);
	init_vector(&warnings);
	while (getline(&buff, &zero, output) != -1) {
		error_type err_type = none;
		size_t line = 0;
		if (strlen(buff) <= strlen(argv[1])) continue;
		parse_gcc_string(buff + strlen(argv[1]), &err_type, &line);
		if (err_type == error) {
			if (find(&errors, line)) continue;
			push_back(&errors, line);
			++err_count;
		}
		if (err_type == warning) {
			if (find(&warnings, line)) continue;
			push_back(&warnings, line);
			++war_count;
		}
	}
	printf("%ld\n%ld\n", err_count, war_count);
	free(buff);
	free(errors.data);
	free(warnings.data);
	fclose(output);
	return 0;
}

