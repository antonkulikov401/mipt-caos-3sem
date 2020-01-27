#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct tm tm;

uint32_t read_dates(tm* dates) {
	const char date_fmt[] = "%d-%d-%d %d:%d";
	int32_t year = 0, month = 0, day = 0, hour = 0, minute = 0;
	uint32_t i = 0;
	while (scanf(date_fmt, &year, &month, &day, &hour, &minute) != EOF) {
		dates[i].tm_year = year - 1900;
		dates[i].tm_mon = month - 1;
		dates[i].tm_mday = day;
		dates[i].tm_hour = hour;
		dates[i].tm_min = minute;
		dates[i].tm_isdst = -1;
		++i;
	}
	return i;
}

int32_t minute_difference(tm* first, tm* second) {
	time_t start_time = mktime(first), end_time = mktime(second);
	return difftime(end_time, start_time) / 60;
}

int main(int argc, char* argv[]) {
	const uint32_t max_num = 4096;
	tm* dates = (tm*)malloc(max_num * sizeof(tm));
	uint32_t num_of_dates = read_dates(dates);
	for (uint32_t i = 1; i < num_of_dates; ++i)
		printf("%d\n", minute_difference(&dates[i - 1], &dates[i]));
	free(dates);
	return 0;
}

