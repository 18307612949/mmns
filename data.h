/* Define what communication and statistics are within this program */

#include <stdint.h>
#include <sys/time.h>

/* types of communication, max of 8 */
enum {
	COMM_NONE	    = 0x0,
	COMM_SEND_ONLY	= 0x1,

	// These flags will return JSON to the user
	COMM_QUERY      = 0x30			// the user sends text to be treated
									// as prepared SQL, with the results
									// being returned to the user
};


typedef struct hdr_t {
	uint64_t ts;		// unix timestamp
	uint8_t  type;		// type of communication
	uint32_t len;		// length of the message
} hdr_t;


typedef struct stat_avg_t {
	double avg[3];		// 1, 5, 15 minutes, the avg
} stat_avg_t;

typedef struct stat_mem_t {
	uint64_t mem_total;	// these define values located within
	uint64_t mem_free;	// proc/meminfo, and the memory values are
	                    // assumed to be in kB. If they're not and you
						// have issues, TELL ME IMMEDIATELY!
} stat_mem_t;

/* 
 * COMMUNICATION STRUCT
 *
 *	Please note, that this can be changed on many-a-platform. Please
 *	make sure you know what your compiler will spit out before you
 *	try to copy this to multiple machines.
 */

typedef struct comm_t {
	hdr_t header;
	stat_avg_t avg;
	stat_mem_t mem;
} comm_t;

typedef struct node_info_t {
	uint32_t  ip;							// network byte order
	uint16_t  port;							// 0 - 65535
	char      hostname[128];				// stores the machine's hostname

	char      username[32];
	char      password[128];

	uint64_t  start_ts;
} node_info_t;

/*
 * The following can be stored within each client node to maintain information
 * about the entire system.
 */

typedef struct system_info_t {
	int8_t    flag;
	uint64_t  num_nodes;
	uint64_t  num_clients;
} system_info_t;

/* This is sent by the client, and it denotes */

/* And finally, the piece that brings everything together */
typedef struct server_hdr_t {
} server_hdr_t;
