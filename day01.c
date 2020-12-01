#include <stdio.h>
#include <stdlib.h>
#include "arraylist.h"

int sub(const void *l, const void *r) {
	return *(int *) l - *(int *) r;
}

static int twentytwenty = 2020;
static int part2 = 1;

int main(void) {
	alist	list, noop, *action;
	int	val, count = 0, *head, *tail;
	size_t	lpos;

	alist_init(int, &noop);
	alist_add_imm(int, &noop, 0);
	alist_init(int, &list);

	while (scanf("%d ", &val) == 1) {
		alist_add(&list, &val);
	}

	qsort(list.list, list.size, list.width, sub);
	action = part2 ? &list : &noop;
	for (lpos = 0; lpos < action->size; lpos++) {
		int target;

		target = twentytwenty - alist_get(int, action, lpos);
		head = list.list;
		tail = head + (list.size - 1);
		head += lpos;
		while (*head < *tail) {
			int value = *head + *tail - target;
			if (value > 0) {
				tail--;
			} else if (value < 0) {
				head++;
			} else {
				break;
			}
		}

		if (*head < *tail) {
			int first = twentytwenty - target;
			printf("%d + %d + %d = %d\n", first, *head, *tail, twentytwenty);
			printf("%d * %d * %d = %d\n", first ? first : 1, *head, *tail, first * *head * *tail);
			count++;
		}
	}
	if (!count) {
		puts("No matches found");
	}

	alist_free(&list);
	alist_free(&noop);
	return 0;
}
