/*
 * most of the functions in the library exist because I want nice string
 * checking functions. C doesn't come prepackaged with them, so I'll write my
 * own.
 */

#include <string.h>
#include <stdlib.h>


int isnumeric(char *input, size_t size)
{
	int i;

	for (i = 0; i < size; i++)
		if (!isdigit(input + i))
			return 0;

	return 1;
}
