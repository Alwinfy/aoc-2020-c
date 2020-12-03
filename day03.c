#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "arraylist.h"

unsigned int count(const alist *grid, size_t width, unsigned int x, unsigned int y) {
	unsigned int	count;
	size_t		n, idx;

	for (n = 0, idx = 0, count = 0; n < grid->size; idx++, n += y) {
		count += (alist_get(char *, grid, n)[idx * x % width] == '#');
	}
	return count;
}

int main() {
	char	*line = NULL;
	size_t	n = 0, width = 0, size;
	alist	grid;

	alist_init(char *, &grid);

	while (~(size = getline(&line, &n, stdin))) {
		if (n <= 1) {
			continue;
		}
		line = realloc(line, size + 1);
		width = size - 1;
		alist_add(&grid, &line);

		line = NULL;
		n = 0;
	}
	assert(grid.size);
	free(line);

	size = count(&grid, width, 3, 1);
	printf("%lu\n", size);
	size *= count(&grid, width, 1, 1);
	size *= count(&grid, width, 5, 1);
	size *= count(&grid, width, 7, 1);
	size *= count(&grid, width, 1, 2);
	printf("%lu\n", size);

	for (n = 0; n < grid.size; n++) {
		free(alist_get(char *, &grid, n));
	}
	alist_free(&grid);
	return 0;
}
