#include <stdio.h>
#include "arraylist.h"

int main() {
	alist list;
	alist_init(int, &list);

	alist_add_imm(int, &list, 10);
	alist_add_imm(int, &list, 20);
	alist_add_imm(int, &list, 30);
	int i = 41;
	alist_add(&list, &i);

	printf("list[1] = %d\n", alist_get(int, &list, 1));
	printf("list[3] = %d\n", alist_get(int, &list, 3));

	alist_free(&list);
}
