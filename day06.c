#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	char		yes[26], *line = NULL, *head;
	size_t		count1 = 0, count2 = 0, n = 0, c = 0, all[26];

	for (n = 0; n < 26; n++) {
		yes[n] = all[n] = 0;
	}
	c = 0;
	while (~getline(&line, &n, stdin)) {
		if (*line == '\n') {
			for (n = 0; n < 26; n++) {
				count1 += yes[n];
				count2 += (all[n] == c);
				yes[n] = all[n] = 0;
			}
			c = 0;
			continue;
		}
		for (head = line; *head; head++) {
			if (*head >= 'a' && *head <= 'z') {
				yes[*head - 'a'] = 1;
				all[*head - 'a']++;
			}
		}
		c++;
	}
	for (n = 0; n < 26; n++) {
		count1 += yes[n];
		count2 += (all[n] == c);
	}
	free(line);

	printf("%lu %lu\n", count1, count2);

	return 0;
}
