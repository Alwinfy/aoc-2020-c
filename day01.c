#include <stdio.h>
#include <stdlib.h>
#include "arraylist.h"

int sub(const void *l, const void *r) {
	return *(int *) l - *(int *) r;
}

static int twentytwenty = 2020;

void process(alist *action, alist *list) {
	size_t	lpos, count = 0;
	int	*head, *tail;

	for (lpos = 0; lpos < action->size; lpos++) {
		int target;

		target = twentytwenty - alist_get(int, action, lpos);
		head = list->list;
		tail = head + (list->size - 1);
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
			int mfirst = first ? first : 1;
			printf("%d + %d + %d = %d\n", first, *head, *tail, twentytwenty);
			printf("%d * %d * %d = %d\n", mfirst, *head, *tail, mfirst * *head * *tail);
			count++;
		}
	}
	if (!count) {
		puts("No matches found");
	}
}

int main(void) {
	alist	list, noop;
	int	val;

	alist_init(int, &noop);
	alist_add_imm(int, &noop, 0);
	alist_init(int, &list);

	while (scanf("%d ", &val) == 1) {
		alist_add(&list, &val);
	}

	qsort(list.list, list.size, list.width, sub);
	process(&noop, &list);
	process(&list, &list);

	alist_free(&list);
	alist_free(&noop);
	return 0;
}
