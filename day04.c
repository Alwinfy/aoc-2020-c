#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arraylist.h"

#define COMMA ,
#define NOOP
#define PASSPORT_FIELDS(pport, delim, op) op pport->byr \
	delim op pport->iyr \
	delim op pport->eyr \
	delim op pport->hgt \
	delim op pport->hcl \
	delim op pport->ecl \
	delim op pport->pid \
	delim op pport->cid

typedef enum __ppt_types {
	BYR = 0,
	IYR,
	EYR,
	HGT,
	HCL,
	ECL,
	PID,
	CID
} ppt_type;

const char *KEYS[] = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid", "cid", NULL};

typedef struct __passport {
	int		byr, iyr, eyr;
	int		hgt;
	int		hcl, ecl;
	unsigned int	pid, cid;
} passport;

void passport_init(passport *pport) {
	PASSPORT_FIELDS(pport, =, NOOP) = -1;
	pport->cid = 0; /* for part 1 */
}

int passport_validate(const passport *pport) {
	return PASSPORT_FIELDS(pport, &&, ~);
}
void passport_prn(const passport *pport, FILE *out) {
	fprintf(out, "byr=%d iyr=%d eyr=%d hgt=%dun hcl=#%06x ecl=#%06x pid=%d cid=%d\n",
		PASSPORT_FIELDS(pport, COMMA, NOOP));
}

int get_key_type(const char key[4]) {
	int offset = 0;
	while (KEYS[offset]) {
		if (!strcmp(KEYS[offset], key)) {
			return offset;
		}
		offset++;
	}
	return -1;
}

void parse_structures(const char *line, passport *dest) {
	char	key[4], val[16]; /* please don't buffer overrun please don't buffer overrun */
	int	offset, fail = 0;
	while (!fail && sscanf(line, "%3s:%15s %n", key, val, &offset) == 2) {
		int	type;
		if ((type = get_key_type(key)) < 0) {
			fail = 1;
			continue;
		}
		switch (type) {
		case BYR:
			sscanf(val, "%d", &dest->byr) == 1 || (fail = 1);
			break;
		case IYR:
			sscanf(val, "%d", &dest->iyr) == 1 || (fail = 1);
			break;
		case EYR:
			sscanf(val, "%d", &dest->eyr) == 1 || (fail = 1);
			break;
		case HGT: {
			char *penu = val + strlen(val) - 2;
			if (*penu >= 'a' && *penu <= 'z') {
				*penu = '\0';
			}
			sscanf(val, "%d", &dest->hgt) == 1 || (fail = 1);
			dest->hgt = 1;
			break;
		}
		case HCL:
			/* sscanf(val, "#%x", &dest->hcl) == 1 || (fail = 1); */
			dest->hcl = 1;
			break;
		case ECL:
			/* strncpy(dest->ecl, val, 3); */
			dest->ecl = 1;
			break;
		case PID:
			sscanf(val, "%u", &dest->pid) == 1 || sscanf(val, "#%x", &dest->pid) == 1 || (fail = 1);
			break;
		case CID:
			sscanf(val, "%u", &dest->cid) == 1 || (fail = 1);
			break;
		default:
			fail = 1;
		}
		line += offset;
	}
}

int main() {
	alist		passports;
	passport	copy, *head;
	char		*line = NULL;
	size_t		count = 0, n = 0;

	passport_init(&copy);
	alist_init(passport, &passports);

	alist_add(&passports, &copy);
	head = alist_back(&passports);
	while (~getline(&line, &n, stdin)) {
		if (*line == '\n') {
			alist_add(&passports, &copy);
			head = alist_back(&passports);
			continue;
		}
		parse_structures(line, head);
	}
	free(line);

	for (n = 0; n < passports.size; n++) {
		/* passport_prn(&alist_get(passport, &passports, n), stderr); */
		count += passport_validate(&alist_get(passport, &passports, n));
	}

	printf("%lu\n", count);

	alist_free(&passports);
	return 0;
}
