#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>

int cmp_dates(const struct tm* lhs, const struct tm* rhs) {
	return (lhs->tm_year > rhs->tm_year ||
		lhs->tm_year == rhs->tm_year &&
		lhs->tm_mon > rhs->tm_mon ||
		lhs->tm_year == rhs->tm_year &&
		lhs->tm_mon == rhs->tm_mon &&
		lhs->tm_mday >= rhs->tm_mday);
}

void print_new_files(char* path, struct tm* time) {
	DIR* dir = opendir(path);
	struct dirent* dp = NULL;
	while ((dp = readdir(dir)) != NULL) {
		struct stat st;
        fstatat(dirfd(dir), dp->d_name, &st, 0);
		if (cmp_dates(localtime(&st.st_mtime), time))
			printf("%s\n", dp->d_name);
	}
	closedir(dir);
}

int main(int argc, char* argv[]) {
	struct tm time;
	time.tm_year = atoi(argv[2]) - 1900;
	time.tm_mon = atoi(argv[3]) - 1;
	time.tm_mday = atoi(argv[4]);
	time.tm_sec = 0;
	time.tm_min = 0;
	time.tm_hour = 0;
	time.tm_isdst = -1;
	print_new_files(argv[1], &time);
	return 0;
}

