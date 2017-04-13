/*
 * Brian Chrzanowski
 * Wed Jan 11, 2017 04:15
 *
 * This program creates a UNIX sock, and reads in info in the form of,
 *
 *	DATA::TYPE
 *
 * and simply validates that the DATA will not cause problems for the TYPE.
 * An additional feature is that it will warn the calling process of
 * potential SQL_INJECTION problems.
 *
 * Here are the return values for this program:
 *
 *	0 - good
 *	1 - bad
 *	2 - don't have enough memory, sorry (IE input data too large)
 *	3 - unrecognized data type
 *	... 255
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define RETURN_GOOD 0
#define RETURN_BAD  1
#define RETURN_INPUT_TOO_BIG 2

#define BUF_SIZE 2048

int sanitize(char *data, char *type);

int main(int argc, char **argv)
{
	int sock, sock2;
	struct sockaddr_un local, remote;
	int len;
	char *path;

	if (argc == 1) {
		path = "/tmp/db_sanitize.sock";
	} else {
		path = argv[1];
	}

	/* create a UNIX socket */
	sock = socket(AF_UNIX, SOCK_STREAM, 0);


	/* now, we need to bind it on disk */
	local.sun_family = AF_UNIX;
	strcpy(local.sun_path, path);
	unlink(local.sun_path);

	/* find the length of the path... */
	len = sizeof(struct sockaddr_un); 
	// or this if it makes you happy strlen(local.sun_path) + sizeof(local.sun_family);



	/* then bind it... */
	if (bind(sock, (struct sockaddr *)&local, len) == -1) {
		perror("bind");
		exit(1);
	}

	if (listen(sock, 32) == -1) {
		perror("listen");
		exit(1);
	}

	for(;;) { /* infinite loop, but only one connection at a time */
		int done, n, t;
		char str[BUF_SIZE] = {0};

		printf("Waiting for a connection...\n");
		t = sizeof(remote);

		if ((sock2 = accept(sock, (struct sockaddr *)&remote, (socklen_t *)&t)) == -1) {
			perror("accept");
			exit(1);
		}

		printf("Connected.\n");

		/* here's where you fork() */

		done = 0;
		do {
			n = recv(sock2, str, BUF_SIZE, 0);

			if (n == 0) /* check the return value */
				done = 1;
			else if (n < 0)
				perror("recv");

			if (!done) {

				printf("got : '%s'", str);
				if (send(sock2, (const void *) &str, BUF_SIZE, 0) < 0) {
					perror("send");
					done = 1;
				}
			}

		} while (!done);

		close(sock2);
	}

	return 0;
}

int sanitize(char *data, char *type)
{
	int return_val = 1; /* 1 == true */

	return return_val;
}
