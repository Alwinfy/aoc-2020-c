#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arraylist.h"

#define MAX_LONGS (1 << 12)

typedef struct __mement {
	long	key;
	long	value;
} mement;

#define SHITTY_HASH_MAP_SIZE 4096

typedef struct __shittyhashmap {
	alist	lists[SHITTY_HASH_MAP_SIZE];
	char	init[SHITTY_HASH_MAP_SIZE];
} shittyhashmap;

void shittyhashmap_init(shittyhashmap *map) {
	memset(&map->init, 0, SHITTY_HASH_MAP_SIZE);
}
void shittyhashmap_free(shittyhashmap *map) {
	size_t i;

	for (i = 0; i < SHITTY_HASH_MAP_SIZE; i++) {
		if (map->init[i]) {
			alist_free(map->lists + i);
		}
	}
}

mement *findalloc(shittyhashmap *map, long key) {
	size_t	i, bucket;
	mement	create;
	alist	*list;

	bucket = key & (SHITTY_HASH_MAP_SIZE - 1);
	list = map->lists + bucket;
	if (!map->init[bucket]) {
		map->init[bucket] = 1;
		alist_init(mement, list);
	}
	for (i = 0; i < list->size; i++) {
		mement *next = alist_get_ptr(list, i);
		if (key == next->key) {
			return next;
		}
	}
	create.key = key;
	alist_add(list, &create);
	return alist_back(list);
}

size_t genfloatset(long mask, long *floatset, int offset) {
	if (offset < 0) {
		floatset[0] = mask;
		return 1;
	}
	if ((mask >> offset) & 1) {
		floatset += genfloatset(mask & ~(1L << offset), floatset, offset - 1);
		return 2 * genfloatset(mask, floatset, offset - 1);
	}
	return genfloatset(mask, floatset, offset - 1);
}

int main() {
	shittyhashmap	entries, entries2;
	long		omask = 0, zmask = 0, xmask = 0, address, data;
	long		floatset[MAX_LONGS];
	size_t		i, j, xlen = 0;

	shittyhashmap_init(&entries);
	shittyhashmap_init(&entries2);
	while (1) {
		if (scanf("mem[%ld] = %ld ", &address, &data) == 2) {
			mement *entry = findalloc(&entries, address);
			entry->value = ((data & ~zmask) | omask);
			for (i = 0; i < xlen; i++) {
				long newadd = (address & ~xmask) | omask | floatset[i];
				findalloc(&entries2, newadd)->value = data;
			}
			continue;
		} else if (scanf("mask = ") == 0) {
			int next, count;
			omask = zmask = xmask = 0;
			while ((next = getchar()) != '\n') {
				count++;
				omask <<= 1;
				zmask <<= 1;
				xmask <<= 1;
				if (next == '1') {
					omask |= 1;
				}
				if (next == '0') {
					zmask |= 1;
				}
				if (next == 'X') {
					xmask |= 1;	
				}
			}
			xlen = genfloatset(xmask, floatset, 36);
			continue;
		}
		break;
	}

	address = data = 0;
	for (j = 0; j < SHITTY_HASH_MAP_SIZE; j++) {
		alist *one = entries.lists + j;
		alist *two = entries2.lists + j;
		if (entries.init[j]) {
			for (i = 0; i < one->size; i++) {
				data += alist_get(mement, one, i).value;
			}
		}
		if (entries2.init[j]) {
			for (i = 0; i < two->size; i++) {
				address += alist_get(mement, two, i).value;
			}
		}
	}
	printf("%ld %ld\n", data, address);

	shittyhashmap_free(&entries);
	shittyhashmap_free(&entries2);
	return 0;
}
