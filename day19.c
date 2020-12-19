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
	int i, j, c = 0;
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

rule p2_8 = {2, {2, 1}, 0, {{42, 8, 0}, {42, 0, 0}}};
rule p2_11 = {2, {3, 2}, 0, {{42, 11, 31}, {42, 31, 0}}};

int main() {
	rule rules[MAX_RULES], rules2[MAX_RULES];
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
	memcpy(rules2, rules, MAX_RULES * sizeof(rule));
	rules2[8] = p2_8;
	rules2[11] = p2_11;
	while (~getline(&line, &n, stdin)) {
		const char *res1 = test(line, rules, 0);
		const char *res2 = test(line, rules2, 0);
		if (res1 && *res1 == '\n') {
			count1++;
		}
		printf("rest=%s", res2 ? res2 : "NULL\n");
		if (res2) {
			count2++;
		}
	}
	free(line);
	printf("%lu %lu\n", count1, count2);
	return 0;
}
