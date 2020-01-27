#include <unistd.h>
#include <stdio.h>
#include <linux/limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    int total_size = 0;
	char* path = (char*)malloc(PATH_MAX + 1);
    while (1) {
		char* res = fgets(path, PATH_MAX + 1, stdin);
		if (!res) break;
		path[strlen(path) - 1] = 0;
		struct stat st;
		lstat(path, &st);
		if(S_ISREG(st.st_mode))
			total_size += st.st_size;    
    }
	printf("%d", total_size);
    free(path);
	return 0;
}

