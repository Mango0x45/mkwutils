#include <sys/stat.h>

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include "crc32.h"
#include "ghost.h"
#include "gm.h"
#include "utf.h"
#include "lookup.h"

unsigned int target_slot_offset;

/* See NOTES, the leading zero is for padding */
static const unsigned int ghost_offset_table[] = {0, 0x28000, 0xCD000, 0x172000, 0x217000};
static const unsigned int profile_offset_table[] = {0, 0x08, 0x8CC8, 0x11988, 0x1A648};

/* These are going to be reused for lap splits, so make them macros */
#define GRAB_MINUTES(x) rkg[x] >> 1
#define GRAB_SECONDS(x) ((rkg[x] & 1) << 6) | (rkg[x + 1] >> 2)
#define GRAB_MILLIS(x)  ((rkg[x + 1] & 3) << 8) | rkg[x + 2]

static void
ghost_init(const uint8_t *rkg, struct GhostData *g)
{
	/* clang-format off */
	*g = (struct GhostData) {
	        .run_time = (struct Time) {
			.minutes = GRAB_MINUTES(0x4),
			.seconds = GRAB_SECONDS(0x4),
			.milliseconds = GRAB_MILLIS(0x4),
		},
	        .track = rkg[0x7] >> 2,
	        .vehicle = rkg[0x8] >> 2,
	        .character = (rkg[0x8] & 3) << 4 | (rkg[0x9] >> 4),
	        .controller = rkg[0xB] & 0xF,
	        .drift = (rkg[0xD] >> 1) & 1,
	        .lap1 = (struct Time) {
			.minutes = GRAB_MINUTES(0x11),
			.seconds = GRAB_SECONDS(0x11),
			.milliseconds = GRAB_MILLIS(0x11)
		},
	        .lap2 = (struct Time) {
			.minutes = GRAB_MINUTES(0x14),
			.seconds = GRAB_SECONDS(0x14),
			.milliseconds = GRAB_MILLIS(0x14),
		},
	        .lap3 = (struct Time) {
			.minutes = GRAB_MINUTES(0x17),
			.seconds = GRAB_SECONDS(0x17),
			.milliseconds = GRAB_MILLIS(0x17),
		},
	        .country = rkg[0x34],
	};
	utf16_to_utf8((char *) rkg + 0x3E, g->name);

	/* clang-format on */
}

#undef GRAB_MINUTES
#undef GRAB_SECONDS
#undef GRAB_MILLIS

static void
create_ghost_file(const uint8_t *rkg)
{
	struct GhostData g;
	ghost_init(rkg, &g);

	char filename[64];
	sprintf(filename, "%s_%s_%u-%02u-%03u.rkg", g.name, track_table[g.track],
	        g.run_time.minutes, g.run_time.seconds, g.run_time.milliseconds);

	int fd = creat(filename, 0666);
	if (fd == -1)
		die("creat");

	ssize_t ret = write(fd, rkg, GHOST_SIZE);
	if (ret == -1)
		die("write");

	(void) close(fd);
}

static void
export_ghost(const unsigned int license)
{
	uint8_t *rkg, *lptr = rksys + ghost_offset_table[license];

	if (flags & FLAG_EXPALL) {
		for (unsigned int i = 0; i < GHOST_COUNT; i++) {
			rkg = lptr + i * GHOST_SIZE;
			if (memcmp(rkg, GHOST_MAGIC, 4) != 0)
				continue;

			create_ghost_file(rkg);
		}
	}
	else if (flags & FLAG_EXPORT) {
		rkg = lptr + target_slot_offset;
		create_ghost_file(rkg);
	}
}

static void
import_ghost(const unsigned int license)
{
	uint8_t *rkg = rksys + ghost_offset_table[license] + target_slot_offset;

	/*
	 * The ghost file isn't validated intentionally. If the user wants to mess around with bad
	 * ghost files and see what happens, then so be it!
	 */
	if (read(STDIN_FILENO, rkg, GHOST_SIZE) == -1)
		die("read");

	/* After importing a ghost make sure to set the ghost flags (see NOTES) */
	uint8_t *fptr = rksys + profile_offset_table[license]
	                + ((target_slot_offset >= DOWNLOADED_GHOST_OFFSET) ? 0x8 : 0x4);
	uint32_t ghost_flags;
	memcpy(&ghost_flags, fptr, sizeof(uint32_t));
	ghost_flags |= swap32((1 << (target_slot_offset / GHOST_SIZE)));
	memcpy(fptr, &ghost_flags, sizeof(uint32_t));

	/* Finally, recalculate the CRC32 hash */
	uint32_t crc = crc32(rksys, CRC_OFFSET);
	memcpy(rksys + CRC_OFFSET, &crc, sizeof(uint32_t));
}

static void
print_license(const unsigned int license)
{
	printf("License %u\n", license);

	/* Pointer to the start of the ghost files */
	uint8_t *lptr = rksys + ghost_offset_table[license];

	for (unsigned int i = 0; i < GHOST_COUNT; i++) {
		/* Pointer to the current ghost file */
		const uint8_t *rkg;
		char type;
		unsigned int index;

		if (i < 32) {
			rkg = lptr + track_offset_table[i] * GHOST_SIZE;
			type = 'r';
			index = i + 1;
		}
		else {
			rkg = lptr + i * GHOST_SIZE;
			type = 'd';
			index = i - 31;
		}

		if (memcmp(rkg, GHOST_MAGIC, 4) != 0)
			continue;

		struct GhostData g;
		ghost_init(rkg, &g);

		/* https://unix.stackexchange.com/a/609135 */
		const unsigned int pad = 10 + strlen(g.name) - utf8_strlen(g.name);

		printf("%c%02u\t%s\t%d'%02d\"%03d\t%-*s\t%-8s\t%-15s\t%s on %s\n", type, index,
		       track_table[g.track], g.run_time.minutes, g.run_time.seconds,
		       g.run_time.milliseconds, pad, g.name, controller_table[g.controller],
		       country_table[g.country], character_table[g.character],
		       vehicle_table[g.vehicle]);
	}
}

void
execute_license(const unsigned int license)
{
	if ((flags & FLAG_EXPALL) || (flags & FLAG_EXPORT))
		export_ghost(license);
	else if (flags & FLAG_IMPORT)
		import_ghost(license);
	else
		print_license(license);
}
