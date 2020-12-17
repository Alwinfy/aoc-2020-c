#include <stdio.h>
#include <stdlib.h>

#include "multid.h"

#define CYCLES	7
#define SIZE	8
#define SDIM	(2 * CYCLES + 1)
#define BDIM	(2 * CYCLES + SIZE)

int countAdj(int *offsets, void *loc) {
	char *c = (char *) loc;
	if (~offsets[1]) {
		void **indirect = (void **) loc;
		int p = countAdj(offsets + 1, indirect[*offsets - 1]);
		int q = countAdj(offsets + 1, indirect[*offsets]);
		int r = countAdj(offsets + 1, indirect[*offsets + 1]);
		return p + q + r;
	}
	return c[*offsets] + c[*offsets + 1] + c[*offsets - 1];
}

int update(char cell, int count) {
	return (cell && count == 4) || count == 3;
}

int main() {
	int	c, i, j, k, l, fc1 = 0, fc2 = 0;
	char	***data1 = alloc_nd_array(sizeof(char), SDIM, BDIM, BDIM, -1);
	char	***next1 = alloc_nd_array(sizeof(char), SDIM, BDIM, BDIM, -1);
	char	****data2 = alloc_nd_array(sizeof(char), SDIM, SDIM, BDIM, BDIM, -1);
	char	****next2 = alloc_nd_array(sizeof(char), SDIM, SDIM, BDIM, BDIM, -1);

	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			char c;
			scanf("%c ", &c);
			data2[CYCLES][CYCLES][CYCLES + i][CYCLES + j] = data1[CYCLES][CYCLES + i][CYCLES + j] = (c == '#');
		}
	}

	for (c = 1; c < CYCLES; c++) {
		char ***swap1, ****swap2;
		for (i = 1; i < SDIM-1; i++) {
			for (j = 1; j < BDIM-1; j++) {
				for (k = 1; k < BDIM-1; k++) {
					int offsets[5];
					offsets[1] = i; offsets[2] = j; offsets[3] = k; offsets[4] = -1;
					next1[i][j][k] = update(data1[i][j][k], countAdj(offsets + 1, data1));
					for (l = 1; l < SDIM-1; l++) {
						offsets[0] = l;
						next2[l][i][j][k] = update(data2[l][i][j][k], countAdj(offsets, data2));
					}
				}
			}
		}
		swap1 = data1; data1 = next1; next1 = swap1;
		swap2 = data2; data2 = next2; next2 = swap2;
	}
	for (i = 1; i < SDIM-1; i++) {
		for (j = 1; j < BDIM-1; j++) {
			for (k = 1; k < BDIM-1; k++) {
				fc1 += data1[i][j][k];
				for (l = 1; l < SDIM-1; l++) {
					fc2 += data2[l][i][j][k];
				}
			}
		}
	}
	printf("%d %d\n", fc1, fc2);

	free(data1);
	free(next1);
	free(data2);
	free(next2);
	return 0;
}
