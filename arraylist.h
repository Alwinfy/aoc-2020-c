#ifndef _ARRAYLIST_H
#define _ARRAYLIST_H

#include <stddef.h>

typedef struct __alist {
	size_t size, capacity;
	size_t width;
	void *list;
} alist;

void alist_init_typeless(alist *list, size_t width, size_t initcap);
#define alist_init_size(type, list, size) alist_init_typeless((list), sizeof (type), size)
#define alist_init(type, list) alist_init_typeless((list), sizeof (type), 0)
void alist_free(alist *list);

void alist_set(alist *list, size_t pos, const void *data);
#define alist_set_imm(type, list, pos, val) do { type _data = (val); alist_set((list), (pos), &_data); } while (0)
void *alist_get_typeless(const alist *list, size_t pos);
void *alist_back(const alist *list);
#define alist_get(type, list, pos) (*(type *) alist_get_typeless((list), (pos)))

void alist_resize(alist *list, size_t target);
void alist_add(alist *list, const void *data);
#define alist_add_imm(type, list, val) do { type _data = (val); alist_add((list), &_data); } while (0)
void alist_remove(alist *list);

#endif
