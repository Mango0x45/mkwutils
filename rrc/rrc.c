#define POSIXLY_CORRECT
#define _POSIX_C_SOURCE 2

#include <sys/mman.h>
#include <sys/stat.h>

#include <errno.h>
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
#define RKSYS_SIZE  0x2BC000

static char *argv0;
static const char *regions[] = {"Japan",  "Americas",    "Europe", "Oceania",
                                "Taiwan", "South Korea", "China"};

static noreturn void
usage(void)
{
	fprintf(stderr, "Usage: %s [-r region] save ...\n", argv0);
	exit(EXIT_FAILURE);
}

static noreturn void
die(const char *s)
{
	fprintf(stderr, "%s: %s", argv0, s);
	if (errno)
		fprintf(stderr, ": %s", strerror(errno));

	fputc('\n', stderr);
	exit(EXIT_FAILURE);
}

static uint8_t *
load_rksys(const char *file, bool rflag)
{
	int open_flags, mmap_flags, prot_flags = PROT_READ;
	if (rflag) {
		open_flags = O_RDWR;
		mmap_flags = MAP_SHARED;
		prot_flags |= PROT_WRITE;
	}
	else {
		open_flags = O_RDONLY;
		mmap_flags = MAP_PRIVATE;
	}

	int fd = open(file, open_flags);
	if (fd == -1)
		die("open");

	struct stat sb;
	(void) fstat(fd, &sb);
	if (sb.st_size != RKSYS_SIZE)
		die("Invalid rksys (invalid file size)");

	uint8_t *rksys = mmap(NULL, RKSYS_SIZE, prot_flags, mmap_flags, fd, 0);
	if (rksys == MAP_FAILED)
		die("mmap");

	(void) close(fd);

	if (memcmp(rksys, "RKSD", 4) != 0)
		die("Invalid rksys.dat (incorrect magic value)");

	return rksys;
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
