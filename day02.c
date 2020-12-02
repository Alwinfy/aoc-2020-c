#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>

int validate_1(size_t min, size_t max, char target, const char *rest) {
	size_t	number = 0;

	while (*rest) {
		if (*rest++ == target) {
			number++;
		}
	}
	return number >= min && number <= max;
}

int validate_2(size_t min, size_t max, char target, const char *rest) {
	return (rest[min - 1] == target) ^ (rest[max - 1] == target);
}

int main() {
	char	*line = NULL;
	size_t	n = 0, count1 = 0, count2 = 0;

	while (~getline(&line, &n, stdin)) {
		int	offset;
		size_t	min, max;
		char	target;

		sscanf(line, "%lu-%lu %c: %n", &min, &max, &target, &offset);
		count1 += validate_1(min, max, target, line + offset);
		count2 += validate_2(min, max, target, line + offset);
	}

	printf("count1 = %lu, count2 = %lu\n", count1, count2);

	free(line);
	return 0;
}
