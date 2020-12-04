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
	PID
} ppt_type;

const char *KEYS[] = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid", "cid", NULL};

typedef struct __passport {
	char    byr[16], iyr[16], eyr[16];
	char    hgt[16];
	char    hcl[16], ecl[16];
	char	pid[16], cid[16];
} passport;

void passport_init(passport *pport) {
	PASSPORT_FIELDS(pport, =, *) = '\0';
	*pport->cid = 48; /* for part 1 */
}

int p1_validate(const passport *pport) {
	return PASSPORT_FIELDS(pport, &&, *);
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

void read_structures(const char *line, passport *dest) {
	char	key[4], val[16]; /* please don't buffer overrun please don't buffer overrun */
	int	offset, fail = 0;
	while (!fail && sscanf(line, "%3s:%15s %n", key, val, &offset) == 2) {
		int	type;
		if ((type = get_key_type(key)) < 0) {
			continue;
		}
		strcpy(dest->byr + 16 * type, val);
		line += offset;
	}
}
int p2_validate(passport *ppt) {
	unsigned int	value;
	char		unit[3] = {0};
	int		count = 0;

	if (sscanf(ppt->byr, "%u%n", &value, &count) != 1 || ppt->byr[count] || value < 1920 || value > 2002)
		return 0;
	if (sscanf(ppt->iyr, "%u%n", &value, &count) != 1 || ppt->iyr[count] || value < 2010 || value > 2020)
		return 0;
	if (sscanf(ppt->eyr, "%u%n", &value, &count) != 1 || ppt->eyr[count] || value < 2020 || value > 2030)
		return 0;
	if (sscanf(ppt->hcl, "#%6x%n", &value, &count) != 1 || count != 7 || ppt->hcl[count])
		return 0;
	if (strlen(ppt->ecl) != 3 || !strstr("amb blu brn gry grn hzl oth", ppt->ecl))
		return 0;
	if (sscanf(ppt->pid, "%9u%n", &value, &count) != 1 || count != 9 || ppt->pid[count])
		return 0;

	count = strlen(ppt->hgt);
	if (count < 2)
		return 0;
	unit[0] = ppt->hgt[count - 2];
	unit[1] = ppt->hgt[count - 1];
	ppt->hgt[count - 2] = 0;
	if (sscanf(ppt->hgt, "%u%n", &value, &count) != 1 || ppt->hgt[count])
		return 0;
	return (!strcmp(unit, "in") && value >= 59 && value <= 76) || 
	       (!strcmp(unit, "cm") && value >= 150 && value <= 193);
}

int main() {
	alist		passports;
	passport	copy, *head;
	char		*line = NULL;
	size_t		count1 = 0, count2 = 0, n = 0;

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
		read_structures(line, head);
	}
	free(line);

	for (n = 0; n < passports.size; n++) {
		/* passport_prn(&alist_get(passport, &passports, n), stderr); */
		count1 += p1_validate(&alist_get(passport, &passports, n));
		count2 += p2_validate(&alist_get(passport, &passports, n));
	}

	printf("%lu %lu\n", count1, count2);

	alist_free(&passports);
	return 0;
}
