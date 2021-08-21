#define POSIXLY_CORRECT
#define _POSIX_C_SOURCE 2

#include <sys/mman.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>
#include <unistd.h>

#include "common.h"
#include "crc32.h"
#include "rrc.h"

#define REGION_BYTE 0x026B0A

static const char *regions[] = {"Japan",  "Americas",    "Europe", "Oceania",
                                "Taiwan", "South Korea", "China"};

static noreturn void
usage(void)
{
	fprintf(stderr, "Usage: %s [-r region] save ...\n", argv0);
	exit(EXIT_FAILURE);
}

static void
set_region(uint8_t *rksys, const char *region)
{
	static const struct Region *r = NULL;
	if (!r && !(r = in_word_set(region, strlen(region))))
		die("Invalid region");

	rksys[REGION_BYTE] = r->id;
	uint32_t crc = crc32(rksys, RKSYS_CRC32_OFFSET);
	memcpy(rksys + RKSYS_CRC32_OFFSET, &crc, sizeof(uint32_t));
}

int
main(int argc, char **argv)
{
	argv0 = argv[0];
	if (argc == 1)
		usage();

	int opt;
	bool rflag = false;
	const char *region;
	while ((opt = getopt(argc, argv, ":r:")) != -1) {
		if (opt == 'r') {
			rflag = true;
			region = optarg;
		}
		else
			usage();
	}
	argv += optind;

	do {
		uint8_t *rksys = load_rksys(*argv, rflag);

		if (region)
			set_region(rksys, region);
		else {
			int i = (rksys[REGION_BYTE] >> 4);
			puts((i <= 6) ? regions[i] : "Unknown");
		}

		(void) munmap(rksys, RKSYS_SIZE);
	} while (*++argv);

	return EXIT_SUCCESS;
}
