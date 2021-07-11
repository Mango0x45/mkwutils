#ifndef __SRS_H_
#define __SRS_H_

#include <stddef.h>

#define CRC_OFFSET 0x027FFC

struct Region {
	const char *name;
	int id;
};

const struct Region *in_word_set(register const char *str, register size_t len);

#endif /* !__SRS_H_ */
