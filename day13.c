#include <stdio.h>
#include <stdlib.h>

#include "arraylist.h"

typedef struct __bus {
	long cycle;
	long offset;
} bus;

long nexttime(long time, const bus *b) {
	return (1 + (time - 1) / b->cycle) * b->cycle;
}

void crt(bus *dest, const bus *add) {
	while ((dest->offset + add->offset) % add->cycle) {
		dest->offset += dest->cycle;
	}
	dest->cycle *= add->cycle;
}

long part1(long arrive, const alist *busses) {
	size_t	i;
	bus	*best = alist_get_ptr(busses, 0);
	long	btime = nexttime(arrive, best);

	for (i = 1; i < busses->size; i++) {
		bus *next = alist_get_ptr(busses, i);
		long ntime = nexttime(arrive, next);
		if (ntime < btime) {
			best = next;
			btime = ntime;
		}
	}

	return best->cycle * (btime - arrive);
}
long part2(const alist *busses) {
	bus	agg;
	size_t	i;

	agg.cycle = 1;
	agg.offset = 0;

	for (i = 0; i < busses->size; i++) {
		crt(&agg, alist_get_ptr(busses, i));
	}
	printf("%ld %ld\n", agg.cycle, agg.offset);
	return agg.offset;
}

int main() {
	int	i;
	long	arrive;
	alist	busses;

	alist_init(bus, &busses);
	scanf("%ld ", &arrive);
	for (i = 0;; i++) {
		bus next;
		if (scanf("%ld,", &next.cycle) != 1) {
			if (scanf("x,") != 0) {
				break;
			}
			continue;
		}
		next.offset = i;
		printf("%ld %ld\n", next.cycle, next.offset);
		alist_add(&busses, &next);
	}
	printf("%ld %ld\n", part1(arrive, &busses), part2(&busses));

	alist_free(&busses);
	return 0;
}
