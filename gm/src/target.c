#include <ctype.h>
#include <stdbool.h>

#include "lookup.h"
#include "ghost.h"
#include "gm.h"
#include "target.h"

/*
 * Function used to get the number out of target_ghost, i.e.:
 * 	"r28" -> 28
 *	"d19" -> 19
 * The string is passed in at an offset of 1 so you can ignore the leading char.
 */
static inline unsigned int
atou(const char *s)
{
	return ((*s - '0') * 10) + *(s + 1) - '0';
}

static bool
validate_target(void)
{
	if ((optarg[0] != 'r' && optarg[0] != 'd') || !isdigit(optarg[1]) || !isdigit(optarg[2]))
		return false;
	return true;
}

enum target_error
set_target(void)
{
	if (!validate_target())
		return BAD_GHOSTID;

	char type = LOWER(optarg[0]);
	unsigned int index = atou(optarg + 1);
	if (index > 32)
		return LARGE_GHOSTID;

	if (type == 'r')
		target_slot_offset = track_offset_table[index - 1] * GHOST_SIZE;
	else
		target_slot_offset = (index + 31) * GHOST_SIZE;

	return 0;
}
