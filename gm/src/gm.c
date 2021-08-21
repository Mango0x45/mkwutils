#define POSIXLY_CORRECT
#define _POSIX_C_SOURCE 2

#include <sys/mman.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <locale.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>
#include <unistd.h>

#include "common.h"
#include "ghost.h"
#include "gm.h"
#include "target.h"

extern const char *argv0;

uint8_t *rksys;
unsigned int flags = 0;

static noreturn void
usage(void)
{
	fprintf(stderr, "%s [-l license number] [-E|-e ghost id] [-i ghost id] rksys.dat\n", argv0);
	exit(EXIT_FAILURE);
}

int
main(int argc, char **argv)
{
	argv0 = argv[0];
	if (setlocale(LC_CTYPE, "") == NULL)
		fprintf(stderr,
		        "%s: Unable to set locale, mii names may be displayed incorrectly\n",
		        argv0);

	int opt;
	unsigned int license = 0;
	/* Outside of `-l` all options are unused *for now* */
	while ((opt = getopt(argc, argv, ":Ee:i:l:r")) != -1) {
		switch (opt) {
		case 'E':
			flags |= FLAG_EXPALL;
			break;
		case 'e':
		case 'i':
			flags |= (opt == 'e') ? FLAG_EXPORT : FLAG_IMPORT;
			switch (set_target()) {
			case BAD_GHOSTID:
				usage();
			case LARGE_GHOSTID:
				die("Invalid ghost ID (given slot is over 32)");
			}
			break;
		case 'l':
			if (optarg[1] || optarg[0] > '4' || optarg[0] < '1') {
				fprintf(stderr, "%s: Invalid license number '%s'\n", argv0, optarg);
				return EXIT_FAILURE;
			}

			license = *optarg - '0';
			break;
		case 'r':
			flags |= FLAG_ROWS;
			break;
		case 's':
			flags |= FLAG_SPLITS;
			break;
		default:
			usage();
		}
	}

	argv += optind;
	argc -= optind;

	if (!argc)
		usage();

	load_rksys(*argv, flags & FLAG_IMPORT);

	if (!license) {
		for (unsigned int i = 1; i <= NUM_LICENSES; i++) {
			execute_license(i);
			if (i != 4 && !((flags & FLAG_EXPALL) || (flags & FLAG_EXPORT)))
				putchar('\n');
		}
	}
	else
		execute_license(license);

	return EXIT_SUCCESS;
}
