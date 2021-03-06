#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "arraylist.h"

#define _ALIST_BOUNDS_CHECK(list, pos) assert((pos) < (list)->size)

void alist_init_ptr(alist *list, size_t width, size_t capacity) {
	list->size = list->capacity = 0;
	list->list = NULL;
	list->width = width;
	alist_resize(list, capacity);
}
void alist_free(alist *list) {
	free(list->list);
	list->size = list->capacity = 0;
	list->list = NULL;
}

void alist_set(alist *list, size_t pos, const void *data) {
	_ALIST_BOUNDS_CHECK(list, pos);
	memcpy((char *) list->list + pos * list->width, data, list->width);
}
void *alist_get_ptr(const alist *list, size_t pos) {
	_ALIST_BOUNDS_CHECK(list, pos);
	return (char *) list->list + pos * list->width;
}
void *alist_back_ptr(const alist *list) {
	return list->size ? (char *) list->list + list->width * (list->size - 1) : NULL;
}

void alist_add(alist *list, const void *data) {
	if (list->capacity >= list->size) {
		alist_resize(list, list->size ? (list->size << 1) : 1);
	}
	alist_set(list, list->size++, data);
}

void alist_remove(alist *list) {
	list->size--;
}

void alist_resize(alist *list, size_t target) {
	if (list->capacity < target) {
		list->list = realloc(list->list, target * list->width);
		list->capacity = target;
	}
}

