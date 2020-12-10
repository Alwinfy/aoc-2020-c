#include <stdio.h>
#include <stdlib.h>

#include "arraylist.h"

int cmp(const void *x, const void *y) {
	return *(int *)x - *(int *)y;
}

int main() {
	int	scan, one = 0, three = 1;
	size_t	i;
	long	*dp;
	alist	list;

	alist_init(int, &list);
	alist_add_imm(int, &list, 0);
	while (scanf("%d", &scan) == 1) {
		alist_add(&list, &scan);
	}
	dp = malloc(list.size * sizeof *dp);
	dp[0] = 1;
	qsort(list.list, list.size, list.width, cmp);

	for (i = 1; i < list.size; i++) {
		int j = i, next;
		int this = alist_get(int, &list, i);
		switch(this - alist_get(int, &list, i - 1)) {
		case 1: one++; break;
		case 3: three++; break;
		}
		dp[i] = 0;
		do {
			next = alist_get(int, &list, --j);
			if (this - next > 3) break;
			dp[i] += dp[j];
		} while (j > 0);
	}
	printf("%d %ld\n", one * three, dp[list.size - 1]);

	alist_free(&list);
	free(dp);
	return 0;
}
