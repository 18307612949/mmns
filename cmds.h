/*
 * This headerfile defines every single possible command that could exist
 * within one struct. The goal is to simply switch on settings. 
 */

#include <stdint.h>

enum {
	MODE_CLIENT	= 0x00,
	MODE_LISTENER	= 0x01
};

typedef struct cmds_t {
	uint8_t  mode;
	uint16_t port;
	uint16_t timeout;
} cmds_t;
