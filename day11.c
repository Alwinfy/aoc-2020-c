#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define xstr(s) str(s)
#define str(s) #s
#define MAXSIZE 128

typedef char (*tdptr)[MAXSIZE];

typedef struct __grid {
	char	data[2][MAXSIZE][MAXSIZE];
	size_t	width, height;
} grid;

int countfill1(tdptr data, size_t i, size_t j) {
	return (data[i + 1][j + 1] == '#')
	     + (data[i    ][j + 1] == '#')
	     + (data[i - 1][j + 1] == '#')
	     + (data[i - 1][j    ] == '#')
	     + (data[i - 1][j - 1] == '#')
	     + (data[i    ][j - 1] == '#')
	     + (data[i + 1][j - 1] == '#')
	     + (data[i + 1][j    ] == '#');
}

int scan(tdptr data, size_t i, size_t j, size_t di, size_t dj) {
	char c;
	do {
		c = data[i += di][j += dj];
	} while (c == '.');
	return (c == '#');
}
int countfill2(tdptr data, size_t i, size_t j) {
	return scan(data, i, j,  1,  1)
	     + scan(data, i, j,  0,  1)
	     + scan(data, i, j, -1,  1)
	     + scan(data, i, j, -1,  0)
	     + scan(data, i, j, -1, -1)
	     + scan(data, i, j,  0, -1)
	     + scan(data, i, j,  1, -1)
	     + scan(data, i, j,  1,  0);
}

void simulate(grid *layout, int (*countfunc)(tdptr, size_t, size_t), int tolerance) {
	int	fixedpt;
	tdptr	this, last;
	size_t	i, j;

	this = layout->data[1];
	last = layout->data[0];
	do {
		tdptr swap;
		fixedpt = 1;
		for (i = 0; i < layout->height; i++) {
			for (j = 0; j < layout->width; j++) {
				int cnt;
				if (last[i][j] != '#' && last[i][j] != 'L') {
					this[i][j] = last[i][j];
					continue;
				}
				cnt = countfunc(last, i, j);
				this[i][j] = (cnt == 0 ? '#' : cnt >= tolerance ? 'L' : last[i][j]);
				if (this[i][j] != last[i][j]) {
					fixedpt = 0;
				}
			}
		}
		swap = last;
		last = this;
		this = swap;
	} while (!fixedpt);
}

int main() {
	grid	layout, layout2;
	char	strin[MAXSIZE];
	size_t	i, j, count1 = 0, count2 = 0;

	memset(&layout, 0, sizeof layout);
	layout.height = 1;
	while (scanf("%126s", strin) == 1) {
		layout.width = strlen(strin) + 2;
		layout.data[0][layout.height][0] = '\0';
		strcpy(1 + layout.data[0][layout.height++], strin);
	}

	memcpy(&layout2, &layout, sizeof layout);

	simulate(&layout, countfill1, 4);
	simulate(&layout2, countfill2, 5);

	for (i = 1; i < layout.height; i++) {
		for (j = 1; j < layout.width; j++) {
			count1 += (layout.data[0][i][j] == '#');
			count2 += (layout2.data[0][i][j] == '#');
		}
	}
	printf("%lu %lu\n", count1, count2);
	return 0;
}
