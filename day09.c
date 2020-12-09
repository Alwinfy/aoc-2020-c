#include <stdio.h>
#include <stdlib.h>

#include "arraylist.h"

#define TWENTY_FIVE 25

int main() {
	size_t	i, j, pos = 0;
	long	cache[TWENTY_FIVE], next;
	alist	list;

	alist_init(long, &list);
	for (i = 0; i < TWENTY_FIVE; i++) {
		scanf("%ld", &cache[i]);
		alist_add(&list, &cache[i]);
	}
	while (scanf("%ld", &next)) {
		alist_add(&list, &next);
		for (i = 0; i < TWENTY_FIVE; i++) {
			for (j = i + 1; j < TWENTY_FIVE; j++) {
				if (next == cache[i] + cache[j]) {
					goto loopend;
				}
			}
		}
		printf("%ld\n", next);
		break;
		loopend:
		cache[pos++ % TWENTY_FIVE] = next;
	}

	for (i = 0; i < pos; i++) {
		long acc = 0;
		for (j = i; j < pos; j++) {
			long last = alist_get(long, &list, j);
			acc += last;
			if (acc == next) {
				long min = last, max = last;
				for (; i < j; i++) {
					long iter = alist_get(long, &list, i);
					min = min > iter ? iter : min;
					max = max < iter ? iter : max;
				}
				printf("%ld + %ld = %ld\n", min, max, min + max);
				goto out;
			}
		}
	}
	out:

	alist_free(&list);
	return 0;
}
