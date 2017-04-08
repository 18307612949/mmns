/* Define what communication and statistics are within this program */

#include <stdint.h>
#include <sys/time.h>

/* types of communication, max of 8 */
enum {
	COMM_NONE	    = 0x0,
	COMM_SEND_ONLY	= 0x1,

	// These flags will return JSON to the user
	COMM_QUERY_ALL     = 0x30,         // Returns EVERYTHING
	COMM_QUERY_USER    = 0x31,         // Returns ALL info about a  USER
	COMM_QUERY_IP      = 0x32,         // Returns ALL info about an IP
	COMM_QUERY_PORT    = 0x33,         // Returns ALL info about a  PORT
	
};


typedef struct hdr_t {
	uint64_t ts;		// unix timestamp
	uint8_t  type;		// type of communication
	uint32_t len;		// length of the message
} hdr_t;

/* NEXT, WE DEFINE WHAT EVERY ASPECT OF A MACHINE'S STATS */

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
	struct timeval time;
	stat_avg_t avg;
	stat_mem_t mem;
} comm_t;

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

typedef struct node_info_t {

} node_info_t;
