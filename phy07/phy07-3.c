#include <stdio.h>
#include <string.h>

int string_len(char* s) {
	int len = 0;
	for (char* c = s; *c != 0; ++c) ++len;
	return len;
}

void normalize_path(char* path) {
	int is_absolute = *path == '/';
	char* it1 = path;
	for (char* it2 = path; *it2 != 0; ++it2) {
		if (*it2 == '.') {
			if (it2 == path) {
				++it1;
				continue;
			}
			++it2;
			if (*it2 == 0) break;
			if (*it2 == '.') {
				while (*it1 != '/') --it1;
				if (it1 == path) continue;
				else {
					--it1;
					while (it1 != path && *it1 != '/') --it1;
				}
				continue;
			}
			if (*it2 == '/') continue;
			--it2;
			*it1 = *it2;
			++it1;
			continue;
		}
		if (*it2 == '/') {
			while (*it2 == '/') ++it2;
			--it2;
		}
		*it1 = *it2;
		++it1;
	}
	while (*it1 != 0) {
		*it1 = 0;
		++it1;
	}
	if (*path == 0) {
		if (is_absolute) *path = '/';
		else *path = '.';
	}
	if (*path == '/' && string_len(path) == 1 && !is_absolute) *path = '.';
}

