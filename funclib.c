
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* 
 * these functions are integral to the operation of the program, as they
 * help determine the delay in which nodes are allowed to request information,
 */

double compute_time_delay(int users, int nodes)
{
	// we have to check function's inputs. this is mostly when there are 0
	// active people online
	if (users < 1) {
		users = 1;
	}

	if (nodes < 1) {
		nodes = 1;
	}

    return (((double)nodes / users)) * 60;
}

/* the functions here exist because I wanted them to exist. As simple as that */
int isnumeric(char *input)
{
	int i;

	for (i = 0; i < strlen(input); i++)
		if (!isdigit(*input + i))
			return 0;

	return 1;
}

int get_digit_index(char *input)
{
	int i;
	
	for (i = 0; i < strlen(input); i++)
		if (isdigit(*(input + i)))
			break;

	return i;
}
