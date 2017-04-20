#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*
 * ---------- A-NOTE-TO-PEOPLE-FOLLOWING-THE-LOGIC-OF-THIS-PROGRAM -----------
 *
 * Note that the needs of interactive load-balancing require more information
 * acquired from individual nodes as more users connect to the system. This
 * is to ensure that people are assigned a capable machine for WHATEVER task is
 * required of them.
 *
 *		more users -> more data / minute
 *
 * The curve of data flow can be described by the following function:
 *
 *		(((double)Number Of Nodes/Number Of Users)) * 60
 *
 *		Where
 *			1 <= Number Of Nodes <= inf
 *			1 <= Number Of Users <= inf
 *
 * This interior function is of the form 1/X, which has an asymptotic nature.
 * Altering the constant with the number of available nodes simply changes the
 * stretch in the function. When multiplying by 60, this obtains a number of
 * seconds each mmns client needs to sleep between blasting updates.
 *
 * Examples using a "small" 6 node system:
 *
 *		Work Day:
 *			60 Employees using 6 nodes
 *				(6 / 60) * 60 = Update every 6 seconds
 *
 *		Night Shift:
 *			4 Employees using 6 nodes
 *				(6 / 4) * 60  = Update every 90 seconds
 *
 *		Saturday:
 *			18 Employees using 6 nodes
 *				(6 / 18) * 60 = Update every 20 seconds
 *
 * Examples using a "massive" 300 nodes system:
 *
 *		Work Day:
 *			8000 Employees using 300 nodes
 *				(300 / 8000) * 60 = Update every 2.25 seconds
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
