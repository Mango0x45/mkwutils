#ifndef __GHOST_H_
#define __GHOST_H_

#include <stdbool.h>

#define MII_NAME_LEN 10

#define CRC_OFFSET 0x027FFC

#define GHOST_COUNT 64
#define GHOST_MAGIC "RKGD"
#define GHOST_SIZE  0x2800

#define DOWNLOADED_GHOST_OFFSET 0x50000

extern unsigned int target_slot_offset;

struct Time {
	unsigned int minutes;
	unsigned int seconds;
	unsigned int milliseconds;
};

struct GhostData {
	struct Time run_time;
	unsigned int track;
	unsigned int vehicle;
	unsigned int character;
	unsigned int year;
	unsigned int month;
	unsigned int day;
	unsigned int controller;
	/* unsigned int compressed; */
	/* unsigned int ghost_type; */
	bool drift;
	/* unsigned int data_length */
	/* unsigned int lap_count */
	struct Time lap1;
	struct Time lap2;
	struct Time lap3;
	unsigned int country;
	unsigned int state;
	unsigned int location;
	char name[MII_NAME_LEN * 4]; /* x4 because of UTF-8 */
};

void execute_license(const unsigned int license);

#endif /* __GHOST_H_ */
