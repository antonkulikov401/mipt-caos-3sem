#include <stdio.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <limits.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <errno.h>
#include <sys/types.h>

typedef struct file_info {
	char* permissions;
	int links;
	char* owner;
	char* group;
	int size;
	char* name;
} file_info;

void destroy_file_info(file_info* fi) {
	free(fi->permissions);
	free(fi->name);
}

void get_file_info(struct stat* st, int dirfd,
	const char* name, file_info* fi) {
	
	char* per = (char*)malloc(11);
	per[0] = (S_ISDIR(st->st_mode) ? 'd' :
		S_ISLNK(st->st_mode) ? 'l' :
		S_ISBLK(st->st_mode) ? 'b' :
		S_ISCHR(st->st_mode) ? 'c' :
		S_ISFIFO(st->st_mode) ? 'p' :
		S_ISSOCK(st->st_mode) ? 's' : '-');
	per[1] = (st->st_mode & S_IRUSR ? 'r' : '-');
	per[2] = (st->st_mode & S_IWUSR ? 'w' : '-');
	per[3] = (st->st_mode & S_IXUSR ? 'x' : 
		st->st_mode & S_ISUID ? 's' : '-');
	per[4] = (st->st_mode & S_IRGRP ? 'r' : '-');
	per[5] = (st->st_mode & S_IWGRP ? 'w' : '-');
	per[6] = (st->st_mode & S_IXGRP ? 'x' :
		st->st_mode & S_ISGID ? 's' : '-');
	per[7] = (st->st_mode & S_IROTH ? 'r' : '-');
	per[8] = (st->st_mode & S_IWOTH ? 'w' : '-');
	per[9] = (st->st_mode & S_IXOTH ? 'x' :
		st->st_mode & S_ISVTX ? 't' : '-');
	per[10] = 0;
	fi->permissions = per;
	fi->links = st->st_nlink;
	fi->owner = getpwuid(st->st_uid)->pw_name;
	fi->group = getgrgid(st->st_gid)->gr_name;
	fi->size = st->st_size;
	char* file_name = (char*)malloc(PATH_MAX + 1);
    strcpy(file_name, name);
	if (S_ISLNK(st->st_mode)) {
		char* suffix = (char*)malloc(PATH_MAX + 1);
		char* buff = (char*)malloc(PATH_MAX + 1);
		memset(buff, 0, PATH_MAX + 1);
		readlinkat(dirfd, name, buff, PATH_MAX + 1); 
		sprintf(suffix, " -> %s", buff);
		strcat(file_name, suffix);
		free(suffix);
		free(buff);
	}
	fi->name = file_name;
}

void print_file_info(file_info* fi, int links_align, int owner_align,
	int group_align, int size_align) {

	printf("%s ", fi->permissions);
	printf("%*d ", links_align, fi->links);
	printf("%*s ", owner_align, fi->owner);
	printf("%*s ", group_align, fi->group);
	printf("%*d ", size_align, fi->size);
	printf("%s\n", fi->name);
}

int cmp_file_info(const file_info* lhs, const file_info* rhs) {
	return strcasecmp(lhs->name, rhs->name);
}

int get_max_size(file_info* files, int len) {
	int max_size = -1;
	for (int i = 0; i < len; ++i)
		if (files[i].size > max_size) max_size = files[i].size;
	return max_size;
}

int get_max_links(file_info* files, int len) {
    int max_links = -1;
    for (int i = 0; i < len; ++i)
        if (files[i].links > max_links) max_links = files[i].links;
    return max_links;
}

int get_max_owner_len(file_info* files, int len) {
	int max_len = -1;
	for (int i = 0; i < len; ++i)
		if (strlen(files[i].owner) > max_len)
			max_len = strlen(files[i].owner);
	return max_len;
}

int get_max_group_len(file_info* files, int len) {
    int max_len = -1;
    for (int i = 0; i < len; ++i)
        if (strlen(files[i].group) > max_len)
			max_len = strlen(files[i].group);
    return max_len;
}

int ls(char* path) {
	struct stat pst;
	int err = lstat(path, &pst);
	if (err == -1) {
		fprintf(stderr, "%s", strerror(errno));
		return 1;
	}
	if (!S_ISDIR(pst.st_mode)) {
		file_info fi;
		char* path_buff = strdup(path);
		DIR* dir = opendir(dirname(path_buff));
		get_file_info(&pst, dirfd(dir), basename(path), &fi);
		print_file_info(&fi, 0, 0, 0, 0);
		closedir(dir);
		free(path_buff);
		destroy_file_info(&fi);
		return 0;
	}

	DIR* dir = opendir(path);
	if (dir == NULL) {
		fprintf(stderr, "%s", strerror(errno));
        return 1;
	}
    struct dirent* dp = NULL;
	const int max_files = 4096;
	file_info* files = (file_info*)malloc(max_files * sizeof(file_info));
	int curr_file = 0;
    while ((dp = readdir(dir)) != NULL) {
        struct stat st;
        char filename[PATH_MAX + 1];
        sprintf(filename, "%s/%s", path, dp->d_name);
		err = lstat(filename, &st);
		if (err == -1) {
        	fprintf(stderr, "%s", strerror(errno));
        	return 1;
    	}
		if (dp->d_name[0] != '.') {
			get_file_info(&st, dirfd(dir), dp->d_name, &files[curr_file]);
			++curr_file;
		}
    }

	qsort(files, curr_file, sizeof(file_info),
		(int(*)(const void*, const void*))cmp_file_info);
	char size_align[256];
	sprintf(size_align, "%d", get_max_size(files, curr_file));
	char links_align[256];
	sprintf(links_align, "%d", get_max_links(files, curr_file));
	for (int i = 0; i < curr_file; ++i) {
		print_file_info(&files[i], strlen(links_align),
			get_max_owner_len(files, curr_file),
			get_max_group_len(files, curr_file),
			strlen(size_align));
		destroy_file_info(&files[i]);
    }
	free(files);
	closedir(dir);
	return 0;
}

int main(int argc, char* argv[]) {
	return ls(argv[1]);
}

