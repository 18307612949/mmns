/* Define what communication and statistics are within this program */

#include <stdint.h>

/* types of communication, max of 255 */
enum {
	COMM_NONE	= 0x1,
	COMM_SEND_ONLY	= 0x2,
	COMM_QUERY	= 0x4
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
	uint64_t mem_used;	// these define values located within
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
