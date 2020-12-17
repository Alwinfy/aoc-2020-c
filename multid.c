#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

/* Takes a varargs list of dimensions for the array, ending in -1 */
void *alloc_nd_array(size_t size, int first, ...) {
	va_list	count;
	size_t	i, last, next, totalsize = 0, arrsize = 1, dims = 0;
	char	**block, **bptr;


	last = first;
	va_start(count, first);
	while (~(next = va_arg(count, int))) {
		dims++;
		arrsize *= last;
		totalsize += arrsize;
		last = next;
	}
	va_end(count);

	block = bptr = malloc(totalsize * sizeof(void *) + arrsize * last * size);
	last = first;
	arrsize = 1;
	va_start(count, first);
	while (~(next = va_arg(count, int))) {
		size_t offset = next * (--dims ? sizeof(void *) : size);
		arrsize *= last;
		*bptr = (char *)(bptr + arrsize);
		for (i = 1; i < arrsize; i++) {
			bptr[i] = bptr[i - 1] + offset;
		}
		bptr = (char **)(*bptr);
		last = next;
	}
	va_end(count);

	/* uncomment next line to zero alloc'd memory */
	memset(bptr, 0, size * last * arrsize);

	return block;
}
