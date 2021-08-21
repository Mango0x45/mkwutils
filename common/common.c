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

const char *argv0;

noreturn void
die(const char *s)
{
	fprintf(stderr, "%s: %s", argv0, s);
	if (errno)
		fprintf(stderr, ": %s", strerror(errno));

	write(STDERR_FILENO, "\n", 1);
	exit(EXIT_FAILURE);
}

uint8_t *
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
