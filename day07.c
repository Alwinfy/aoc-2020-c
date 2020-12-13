#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arraylist.h"

#define MAXWORD 32
#define MAXSPEC 1024

int scanword(char out[MAXWORD]) {
	int n;
	if (scanf("%s%n ", out, &n) != 1) return 0;
	out[n++] = ' ';
	return scanf("%s ", out + n);
}

typedef struct __bagspec {
	char	name[MAXWORD];
	int	reach;
	alist	child;  /* type=bagcount  */
	alist	parent; /* type=bagspec*   */
} bagspec;
typedef struct __bagcount {
	int	count;
	bagspec	*other;
} bagcount;

bagspec	specs[MAXSPEC];
size_t	specsize = 0;

void baginit(bagspec *bag, const char name[MAXWORD]) {
	strcpy(bag->name, name);
	bag->reach = 0;
	alist_init(bagcount, &bag->child);
	alist_init(bagspec *, &bag->parent);
}

bagspec *find_or_init(const char name[MAXWORD]) {
	size_t i;
	for (i = 0; i < specsize; i++) {
		if (!strcmp(specs[i].name, name)) {
			return specs + i;
		}
	}
	baginit(specs + specsize, name);
	return specs + specsize++;
}

int p1(bagspec *bag) {
	int count = 0;
	size_t i = 0;
	if (bag->reach & 1) return 0;
	bag->reach |= 1;
	for (i = 0; i < bag->parent.size; i++) {
		bagspec *parent = alist_get(bagspec *, &bag->parent, i);
		count += p1(parent);
	}
	return 1 + count;
}
int p2(bagspec *bag) {
	int count = 0;
	size_t i = 0;
	for (i = 0; i < bag->child.size; i++) {
		bagcount *child = alist_get_ptr(&bag->child, i);
		count += child->count * p2(child->other);
	}
	return 1 + count;
}

int main() {
	char	buffer[MAXWORD];
	int	dump, num;
	size_t	i;
	bagspec	*sgold;

	while (scanword(buffer) == 1) {
		bagspec *bag;
		int	count = 0;

		bag = find_or_init(buffer);
		dump = scanf("%*s %*s ");
		while (scanf("%d ", &num) == 1) {
			bagspec		*other;
			bagcount	bcount;

			bcount.count = num;
			count++;
			dump = scanword(buffer);
			other = find_or_init(buffer);
			bcount.other = other;
			alist_add(&bag->child, &bcount);
			alist_add(&other->parent, &bag);
			dump = scanf("%*s ");
		}
		if (!count) {
			dump = scanf("%*s %*s %*s ");
		}
	}
	sgold = find_or_init("shiny gold");
	printf("%d %d\n", p1(sgold) - 1, p2(sgold) - 1);
	(void) dump;
	for (i = 0; i < specsize; i++) {
		alist_free(&specs[i].child);
		alist_free(&specs[i].parent);
	}
	return 0;
}
