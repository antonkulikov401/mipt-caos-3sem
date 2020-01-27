#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
	const int buffer_size = 8192;
	const char* source_name = "source_code.c";
	const char* exec_name = "./calculate";
	char expr[buffer_size];
	fgets(expr, buffer_size, stdin);
	if (!strcmp(expr, "") || !strcmp(expr, "\n")) return 0;
	char cmd[buffer_size];
	sprintf(cmd, 
		"#include <stdio.h>\n"
		"int main() { "
		"int x = (%s); "
		"printf(\"%%d\", x); "
		"return 0; }", expr);
	int file = open(source_name, O_RDWR | O_CREAT | O_TRUNC, 0666);
	write(file, cmd, strlen(cmd));
	close(file);
	pid_t pid = fork();
	if (pid == 0)
		execlp("gcc", "gcc", source_name, "-o", exec_name,  NULL);
	wait(NULL);
	pid = fork();
	if (pid == 0)
		execlp(exec_name, exec_name, NULL);
	wait(NULL);
	remove(source_name);
	remove(exec_name);
	return 0;
}

