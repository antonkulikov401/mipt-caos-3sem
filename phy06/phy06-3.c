#include <unistd.h>
#include <stdio.h>
#include <linux/limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

void create_symlink(char* path) {
	char prefix[] = "/link_to_";
    char* link_path = (char*)malloc(strlen(path) + strlen(prefix) + 2);
    char* dir_source = strdup(path);
    char* base_source = strdup(path);
    char* dir_name = dirname(dir_source);
    char* base_name = basename(base_source);
    strcpy(link_path, dir_name);
    strcat(link_path, prefix);
    strcat(link_path, base_name);
    symlink(path, link_path);
    free(link_path);
	free(dir_source);
	free(base_source);
}

void handle(char* path) {
	struct stat st;
    lstat(path, &st);
    if(S_ISREG(st.st_mode))
		create_symlink(path);
	else if (S_ISLNK(st.st_mode)) {
		char* real_path = (char*)malloc(PATH_MAX + 1);
		realpath(path, real_path);
		printf("%s\n", real_path);
		free(real_path);
	}
}

int main(int argc, char* argv[]) {
    char* path = (char*)malloc(PATH_MAX + 1);
    while (1) {
        char* res = fgets(path, PATH_MAX + 1, stdin);
        if (!res) break;
        if (path[strlen(path) - 1] == '\n')
			path[strlen(path) - 1] = 0;
		handle(path);
	}
    free(path);
    return 0;
}
