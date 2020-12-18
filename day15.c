#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arraylist.h>

typedef struct __mement {
	long	key;
	long	value;
} mement;

#define SHITTY_HASH_MAP_SIZE 16384

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

mement *findalloc(shittyhashmap *map, long key, long dvalue) {
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
	create.value = dvalue;
	alist_add(list, &create);
	return alist_back_ptr(list);
}

long historysay(shittyhashmap *history, long next, long time) {
	long	lastval;
	mement	*entry = findalloc(history, next, time);

	lastval = entry->value;
	entry->value = time;
	return time - lastval;
}

int main() {
	long		time = 0, next;
	shittyhashmap	history;

	shittyhashmap_init(&history);
	while (scanf("%ld,", &next) == 1) {
		next = historysay(&history, next, ++time);
	}
	while (time < 2019) {
		next = historysay(&history, next, ++time);
	}
	printf("%ld\n", next);
	while (time < 29999999) {
		next = historysay(&history, next, ++time);
	}
	printf("%ld\n", next);
	shittyhashmap_free(&history);

	return 0;
}
