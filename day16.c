#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_FIELDS 20

typedef struct __field {
	int lmin, lmax;
	int hmin, hmax;
} field;
typedef struct __ticket {
	int data[NUM_FIELDS];
} ticket;

int contains(field *f, int val) {
	return (val >= f->lmin && val <= f->lmax)
	    || (val >= f->hmin && val <= f->hmax);
}
int count(field fields[NUM_FIELDS], int val, int mask) {
	int i, count = 0;

	for (i = 0; i < NUM_FIELDS; i++) {
		if (((mask >> i) & 1) && contains(fields + i, val)) {
			count++;
		}
	}

	return count;
}
int getmask(field fields[NUM_FIELDS], int val) {
	int i, mask = 0;
	for (i = 0; i < NUM_FIELDS; i++) {
		if (contains(fields + i, val)) {
			mask |= (1 << i);
		}
	}
	return mask;
}

int readticket(ticket *out) {
	int i;
	for (i = 0; i < NUM_FIELDS; i++) {
		if (scanf("%d,", &out->data[i]) != 1) {
			return 0;
		}
	}
	return 1;
}

int main() {
	field	fields[NUM_FIELDS];
	ticket	self, buf;
	size_t	i, j, cycle, error = 0;
	long	result;
	int	masks[NUM_FIELDS]; /* tfield -> ordered field */

	for (i = 0; i < NUM_FIELDS; i++) {
		while (getchar() != ':');
		scanf("%d-%d or %d-%d ", &fields[i].lmin, &fields[i].lmax, &fields[i].hmin, &fields[i].hmax);
	}
	i = scanf("your ticket: ");
	readticket(&self);
	for (i = 0; i < NUM_FIELDS; i++) {
		masks[i] = getmask(fields, self.data[i]);
	}
	i = scanf(" nearby tickets: ");
	while (readticket(&buf)) {
		int bad = 0;
		for (i = 0; i < NUM_FIELDS; i++) {
			if (!count(fields, buf.data[i], -1)) {
				error += buf.data[i];
				bad++;
			}
		}
		if (!bad) {
			for (i = 0; i < NUM_FIELDS; i++) {
				masks[i] &= getmask(fields, buf.data[i]);
			}
		}
	}

	for (cycle = 0; cycle < NUM_FIELDS; cycle++) {
		for (i = 0; i < NUM_FIELDS; i++) {
			if (!(masks[i] & (masks[i] - 1))) {
				for (j = 0; j < NUM_FIELDS; j++) {
					if (i != j) {
						masks[j] &= ~masks[i];
					}
				}
			}
		}
	}
	result = 1;
	for (i = 0; i < NUM_FIELDS; i++) {
		if (masks[i] & 0x3f) {
			result *= self.data[i];
		}
	}

	printf("%lu %lu\n", error, result);
	return 0;
}
