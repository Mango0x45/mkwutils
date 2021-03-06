#ifndef __GM_H_
#define __GM_H_

#include <stdint.h>
#include <stdnoreturn.h>

#define NUM_LICENSES 4

#define RKSYS_MAGIC "RKSD"

extern uint8_t *rksys;
extern unsigned int flags;

enum program_flags {
	FLAG_EXPALL = 0x1,
	FLAG_EXPORT = 0x2,
	FLAG_IMPORT = 0x4,
	FLAG_ROWS = 0x8,
	FLAG_SPLITS = 0x10
};

#endif /* !__GM_H_ */
