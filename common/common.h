#ifndef __COMMON_H_
#define __COMMON_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdnoreturn.h>

#define RKSYS_CRC32_OFFSET 0x027FFC
#define RKSYS_SIZE         0x2BC000

extern const char *argv0;

noreturn void die(const char *s);
uint8_t *load_rksys(const char *file, bool rflag);

#endif /* !__COMMON_H_ */
