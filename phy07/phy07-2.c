#include <stdio.h>
#include <stdint.h>
#include <ftw.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>

void dir_size(const char* path, int32_t* size) {
	DIR* dir = opendir(path);
	struct dirent* dp = NULL;
	while ((dp = readdir(dir)) != NULL) {
		struct stat st;
		char filename[PATH_MAX + 1];
		sprintf(filename, "%s/%s", path, dp->d_name);
		lstat(filename, &st);
		if (S_ISREG(st.st_mode)) *size += st.st_size;
		if (S_ISDIR(st.st_mode) && strcmp(dp->d_name, ".") &&
			strcmp(dp->d_name, "..")) dir_size(filename, size);
	}
	closedir(dir);
}

int main(int argc, char* argv[]) {
	int32_t size = 0;
	dir_size(argv[1], &size);
	printf("%d", size);
	return 0;
}

