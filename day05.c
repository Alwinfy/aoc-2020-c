#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>

#include "arraylist.h"

int cmp(const void *a, const void *b) { return *(int*)a - *(int*)b; }

int main() {
	char *line = NULL;
	size_t n = 0, i;
	int best = 0;
	alist list;
	alist_init(int, &list);
	while (~getline(&line, &n, stdin)) {
		int v = 0;
		for (i = 0; i < 10; i++) {
			v |= (line[i] == 'B' || line[i] == 'R') << (9 - i);
		}
		best = best > v ? best : v;
		alist_add_imm(int, &list, v);
	}
	free(line);
	qsort(list.list, list.size, list.width, cmp);
	printf("%d\n", best);
	for (i = 1; i < list.size; i++) {
		if (alist_get(int, &list, i - 1) + 2 == alist_get(int, &list, i)) {
			printf("%d\n", alist_get(int, &list, i) - 1);
		}
	}
	alist_free(&list);
	return 0;
}
