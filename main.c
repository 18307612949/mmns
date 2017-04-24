/*
 * Brian Chrzanowski
 *
 * mmns, Multi-Machine-Network-System
 *  
 * TODO :: Client
 *			Help Text
 *		:: Server
 *			Everything
 *
 */

#include <netdb.h> 
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "cmds.h"
#include "data.h"
#include "funclib.h"
#include "list.h"
#include "sqlite3.h"

#define DEFAULT_TIMEOUT 60
#define SOCKET_BUFFER   16 * 1024 * 1024
#define HOSTLEN         256
#define INITIAL_SIZE    32
#define NODE_TIMEOUT    1800			// time before we remove node data
#define PARSE_BUF_SIZE  128
#define TIMEOUT         1000 * 1000

#define MEM_FILE "/proc/meminfo"
#define MEM_TOTAL "MemTotal"
#define MEM_FREE  "MemFree"

int parse_args(int argc, char **argv, cmds_t *args);
void print_help_text();

void getmemstats(stat_mem_t *input);
int  mode_client_collectstats(comm_t *client_stats);
void mode_client(cmds_t *args);
void mode_client_verbose(cmds_t *args, comm_t *input);

void mode_listener(cmds_t *args);
void mode_listener_loop(cmds_t *args, node_info_t **data);
int createSocket(int portnum, int maxClient, int mode);

int main(int argc, char **argv)
{
	/* parse command line */
	cmds_t args = {0};
	if (parse_args(argc, argv, &args)) {
		return 1;
	}

	/* go into listener mode, or stats blaster mode */
	if (args.mode == MODE_CLIENT) {
		mode_client(&args);
	} else {
		mode_listener(&args);
	}

	printf("leaving\n");

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
	int oc;

	while ((oc = getopt(argc, argv, "shlp:t:v")) != -1) {
		switch (oc) {
			case 'h':
				print_help_text();
				return 1;

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

			case 'v':
				args->verbose = 1;
				break;
				
			case 's':
				args->mode = MODE_LISTENER;
				break;

			default:
				break;
		}
	}

	/* set defaults if no value exists aren't already set */
	if (args->timeout == 0)
		args->timeout = DEFAULT_TIMEOUT;

	/* set the ip address to the last arg */
	strcpy(args->ip_addr, argv[argc - 1]);

	return 0;
}



/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/*
 * funcs : mode_client
 * args  : cmds_t *
 * out   : void
 * use   : The client will sleep for however long the '-t' timeout will allow,
 *         then it will send communications mode_client will malloc
 *         sizeof(comm_t), and simply fill out that data as necessary, then
 *         send it to the specifed listener.
 *
 */

void mode_client(cmds_t *args) {

	struct sockaddr_in serv_addr;
	int sockfd;
	int num_bytes = 0;
	comm_t *client_stats = malloc(sizeof(comm_t));

	if (client_stats) {
		/* make a sock */
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd == -1) {
			fprintf(stderr, "Error opening socket\n");
			break;
		}

		/* set up connection */
		memset(&serv_addr, 0, sizeof(struct sockaddr_in));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(args->port);
		inet_aton(args->ip_addr, &(serv_addr.sin_addr));

		if (connect(sockfd, (struct sockaddr *)&serv_addr,
					sizeof(serv_addr)) == -1) {
			fprintf(stderr, "error connecting\n");
			break;
		}

		while (1) {
			/* get stats */
			mode_client_collectstats(client_stats);

			/* verbosity */
			if (args->verbose) {
				mode_client_verbose(args, client_stats);
			}

			/* send the data over that sock */
			num_bytes = send(sockfd, &client_stats, sizeof(struct comm_t), 0);
			printf("we wrote %d / %ld bytes\n", num_bytes, sizeof(struct comm_t));

			if (num_bytes == -1) {
				perror("sockets ");
				break;
			}

			printf("cleaning up\n");

			/* cleanup */
			close(sockfd);
			memset(client_stats, 0, sizeof(comm_t));

			/* sleep */
			usleep(args->timeout * TIMEOUT);
		}
	} else {
		fprintf(stderr, "insufficient memory!\n");
	}

	free(client_stats);
}

/*
 * func : mode_client_verbose
 * args : cmds_t *, comm_t *
 * out  : void
 * use  : prints out system information in a log style
 */
void mode_client_verbose(cmds_t *args, comm_t *input)
{
	/* print out timestamp */
	printf("TIME\n");
	printf("\tsec   : %ld\n", input->header.ts);
	printf("\n");

	/* print out cpu info */
	printf("Load AVG\n");
	printf("1  min  : %.2f\n", input->avg.avg[0]);
	printf("5  mins : %.2f\n", input->avg.avg[1]);
	printf("15 mins : %.2f\n", input->avg.avg[2]);
	printf("\n");

	/* print out meminfo */
	printf("Memory Info\n");
	printf("Total : %ld\n", input->mem.mem_total);
	printf("Free  : %ld\n", input->mem.mem_free);
	printf("\n");

	/* print out sending information */
	printf("Sending to -> %s\n", args->ip_addr);
	printf("\n");
}


/*
 * func : mode_client_collectstats
 * args : comm_t *, int
 * out  : int
 * use  : this function will simply fill out the comm_t struct, to be sent
 *        to the listener
 */
int mode_client_collectstats (comm_t *client_stats)
{
	int return_val = 0;
	getloadavg(client_stats->avg.avg, 3);

	/* get timestamp */
	client_stats->header.ts = (unsigned long)time(NULL);

	/* getting memory statistics */
	getmemstats(&(client_stats->mem));

	return return_val;
}

/*
 * funcs : getmemstats
 * args  : stat_mem_t *
 * out   : void
 * use   : fills out the relevant memory information into the passed in struct
 */
void getmemstats(stat_mem_t *input)
{
	long i;
	char buf[PARSE_BUF_SIZE] = {0};
	FILE *fp = fopen(MEM_FILE, "r");

	if (fp) {
		/* spin through the file, looking for the various targets */
		while (fgets(buf, PARSE_BUF_SIZE, fp)) { // fgets stops on '\n'
			/* remove the 'kb' from the end of the line */
			for (i = 0; i < 3; i++)
				buf[strlen(buf) - i] = 0;

			/* string comparisons */
			if (strncmp(buf, MEM_TOTAL, strlen(MEM_TOTAL) - 1) == 0) {
				input->mem_total = atol(buf + get_digit_index(buf));
			}

			if (strncmp(buf, MEM_FREE, strlen(MEM_FREE) - 1) == 0) {
				input->mem_free = atol(buf + get_digit_index(buf));
			}

		}

	} else {
		fprintf(stderr, "ERROR :: couldn't open %s!", MEM_FILE);
	}

	fclose(fp);
}






/*
 * func : mode_listener
 * args : cmds_t *
 * out  : void
 * use  : mode_listener will perform multiple mallocs creating a list (using
 *        list functions within "list.h") of lists. List of machine stats,
 *        and each inner list contains, "--limit" nodes.
 *
 *        The function itself will simply make additions to the list on
 *        calls to the accept() function. The process will fork(), and
 *        will create new entries into the list, using the function, 
 *        mode_listener_manage_list.
 */

void mode_listener(cmds_t *args)
{
	int max_size;
	int loop;
	int fd;
	int total_data;
	int recieved;

	char *buffer;
	char *ptr;

	pid_t pid;

	struct LL *node_info     = NULL;
	struct LL *user_commands = NULL;
	struct LL *control_data  = NULL;

	max_size = INITIAL_SIZE;

	loop = 1;
	total_data = 0;
	recieved   = 0;

	/* get a socket */
	fd = createSocket(args->port, MAX_CLIENTS, 0);

	if (fd == -1) {
		perror(NULL);
	}

	buffer = malloc(SOCKET_BUFFER);

	if (buffer) {
		memset(buffer, 0, SOCKET_BUFFER);
	}

	while (loop) {
		while (total_data < SOCKET_BUFFER) {
			recieved = recv(fd, buffer, strlen("helloworld"), 0);

			if (recieved != 0) {
				if (strncmp(buffer, "helloworld", recieved) == 0) {
					pid = fork();

					switch (pid) {
					case -1:
						fprintf(stderr, "FAILED TO FORK!\n");
						loop = 0;
						break;

					case 0:  /* child quits the program on return */
						do_child();
						exit(0);

					default: /* parent */

				}
			}
		}
	}

	/* close the socket */
	close(fd);

	/* free all of the things */
	free_whole_list(control_data);
	free_whole_list(node_info);
	free_whole_list(user_commands);
}

int createSocket(int portnum, int maxClient, int mode)
{
	struct  sockaddr_in   saddr;        /* build our address here */
	struct  hostent     *hostpointer;   /* this is part of our    */
	char    hostname[HOSTLEN];          /* address            */
	int sock_id;                        /* the socket which will be returned*/
	
	sock_id = socket(PF_INET, SOCK_STREAM, 0);

	if ( sock_id == -1 ) 
		return -1;
	
	bzero((void *)&saddr, sizeof(saddr));//null contensts of struct

	if(mode != 1)
	{
		gethostname(hostname, HOSTLEN);
		hostpointer = gethostbyname(hostname);
		bcopy( (void *)hostpointer->h_addr,
				(void *)&saddr.sin_addr, hostpointer->h_length);
	} else {
		saddr.sin_addr.s_addr = INADDR_ANY;
	}
	//give the socket good stuff
	saddr.sin_port = htons(portnum);
	saddr.sin_family = AF_INET;
	
	//verify that everything worked and we are all good.
	if ( bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)) != 0 )
		return -1;

	if ( listen(sock_id, maxClient) != 0 ) 
		return -1;

	return sock_id;
}

void print_help_text()
{
	char *help =
		"USAGE: \"./mmns {-p port, -l, -v verbose, -s}";
	printf("%s\n", help);
}
