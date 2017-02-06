/*
 * TODO :: 
 *	Implement Statistics Gathering
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#include "data.h"
#include "list.h"
#include "cmds.h"
#include "funclib.h"

int parse_args(int argc, char **argv, cmds_t *args);
void mode_client(cmds_t *args);
void mode_listener(cmds_t *args);

int main(int argc, char **argv)
{
	/* parse command line */
	cmds_t args = {0};
	if (parse_args(argc, argv, &args)) {
		return 1;
	}

	/* VERBOSITY */

	/* go into listener mode, or stats blaster mode */
	if (args.mode == MODE_CLIENT) {
		mode_client(&args);
	} else {
		mode_listener(&args);
	}

	return 0;
}

/*
 * func : parse_args
 * args : int, char **, cmds_t
 * out  : int
 * use  : changes command line options into cmds_t, which is easier to pass
 */
int parse_args(int argc, char **argv, cmds_t *args)
{
	int i;
	int oc;

	while ((oc = getopt(argc, argv, "lp:t:")) != -1) {
	switch (oc) {
		case 'l':
			args->mode |= MODE_LISTENER;
			break;
		case 'p':
			if (isnumeric(optarg)) {
				args->port = atoi(optarg);
			} else {
				fprintf(stderr, "%s isn't a numeric port!\n",
						optarg);
				return 1;
			}
			break;
		case 't':
			if (isnumeric(optarg)) {
				args->timeout = atoi(optarg);
			} else {
				fprintf(stderr, "%s isn't a numeric time!\n",
						optarg);
				return 1;
			}

		default:
			break;
	}
	}

	return 0;
}



/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/*
 * funcs : mode_{client, listener}
 * args  : cmds_t *
 * out   : void
 * use   : these are both functions that behave as the 'main loop' for both
 *         both client and listener. The client will sleep for however long
 *         the '-t' timeout will allow, and the listener will simply fork
 *         the process with any new communications over the designated port.
 *
 *         mode_client will malloc sizeof(comm_t), and simply fill out that
 *         data as necessary, then send it to the specifed listener
 *
 *         mode_listener will perform multiple mallocs creating a list (using
 *         list functions within "list.h") of lists. List of machine stats,
 *         and each inner list contains, "--limit" nodes.
 */

void mode_client(cmds_t *args) {
	comm_t *client_stats = malloc(sizeof(comm_t));

	if (client_stats) {

		while (1) {
			/* get stats */
		}

	} else {
		fprintf(stderr, "insufficient memory!\n");
	}

}

void mode_listener(cmds_t *args) {
}
