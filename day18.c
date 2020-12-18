#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>

#include "arraylist.h" /* for robust stack ops */

typedef long (*binop)(long l, long r);

typedef struct __operation {
	char	name;
	binop	eval;
	int	prec;
} operation;

long add(long l, long r) { return l + r; }
long mul(long l, long r) { return l * r; }

const operation PART1[] = {
	{'+', add, 1},
	{'*', mul, 1},
	{0,  NULL, 0},
};
const operation PART2[] = {
	{'+', add, 2},
	{'*', mul, 1},
	{0,  NULL, 0},
};

const operation *findop(char name, const operation ops[]) {
	for (; ops->eval; ops++) {
		if (ops->name == name) {
			return ops;
		}
	}
	return NULL;
}

void unwind(alist *ops, alist *data, int prec) {
	operation *stacktop;
	while (ops->size && (stacktop = alist_back(operation *, ops))->prec >= prec) {
		long l, r;
		alist_remove(ops);
		l = alist_back(long, data);
		alist_remove(data);
		r = alist_back(long, data);
		alist_remove(data);
		alist_add_imm(long, data, stacktop->eval(l, r));
	}
}

long eval(char **state, const operation ops[]) {
	alist	opst, data;
	char	c;
	long	result;
	alist_init(operation *, &opst);
	alist_init(long, &data);
	while ((c = **state)) {
		const operation *op;

		++*state;
		if (c == ' ') {
			continue;
		}
		if (c == '(') {
			alist_add_imm(long, &data, eval(state, ops));
			continue;
		}
		if (c >= '0' && c <= '9') {
			alist_add_imm(long, &data, c - '0');
			continue;
		}
		if ((op = findop(c, ops))) {
			unwind(&opst, &data, op->prec);
			alist_add(&opst, &op);
			continue;
		}
		break;
	}
	unwind(&opst, &data, -99);
	result = alist_back(long, &data);
	alist_free(&opst);
	alist_free(&data);
	return result;
}

int main() {
	char	*line = NULL;
	size_t	n = 0;
	long	total1 = 0, total2 = 0;

	while (~getline(&line, &n, stdin)) {
		char	*buf1 = line, *buf2 = line;
		long	res1 = eval(&buf1, PART1), res2 = eval(&buf2, PART2);
		if (*buf1) {
			printf("Failed to parse line: rest = %s", buf1);
		} else if (*buf2) {
			printf("Failed to parse line: rest = %s", buf1);
		} else {
			total1 += res1;
			total2 += res2;
		}
	}
	free(line);
	printf("%ld %ld\n", total1, total2);
	return 0;
}
