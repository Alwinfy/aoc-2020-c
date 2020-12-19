#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arraylist.h"

#define MAX_RULES 200

typedef struct __rule {
	int alts, rules[2], literal;
	int ptrs[2][3];
} rule;

const char *test(const char *pattern, rule *rules, int num) {
	int i, j;
	rule *rule = rules + num;
	if (rule->literal) {
		return rule->literal == *pattern ? pattern + 1 : NULL;
	}
	for (i = 0; i < rule->alts; i++) {
		const char *pos = pattern;
		for (j = 0; j < rule->rules[i]; j++) {
			pos = test(pos, rules, rule->ptrs[i][j]);
			if (!pos) break;
		}
		if (pos) return pos;
	}
	return NULL;
}
int test2(const char *pattern, rule *rules) {
	int i, j;
	pattern = test(pattern, rules, 42);
	for (i = 0; pattern; i++, pattern = test(pattern, rules, 42)) {
		const char *rest = test(pattern, rules, 31);
		for (j = 0; rest && j < i; j++, rest = test(rest, rules, 31)) {
			if (*rest == '\n') {
				return 1;
			}
		}
				
	}
	return 0;
}

int main() {
	rule rules[MAX_RULES];
	char *line = NULL;
	size_t n = 0, count1 = 0, count2 = 0;
	while (~getline(&line, &n, stdin) && *line != '\n') {
		int key, off;
		rule *r;
		sscanf(line, "%d: %n", &key, &off);
		r = rules + key;
		if (line[off] == '"') {
			r->literal = line[off + 1];
			continue;
		}
		r->alts = 0;
		r->literal = r->rules[0] = 0;
		while (line[off]) {
			int poff;
			if (line[off] == '|') {
				r->rules[++r->alts] = 0;
				off += 2;
			}
			sscanf(line + off, "%d %n", &r->ptrs[r->alts][r->rules[r->alts]++], &poff);
			off += poff;
		}
		r->alts++;
	}
	while (~getline(&line, &n, stdin)) {
		const char *res1 = test(line, rules, 0);
		if (res1 && *res1 == '\n') {
			count1++;
		}
		if (test2(line, rules)) {
			count2++;
		}
	}
	free(line);
	printf("%lu %lu\n", count1, count2);
	return 0;
}
