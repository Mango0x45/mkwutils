#ifndef __RRC_H_
#define __RRC_H_

#include <stddef.h>

struct Region {
	const char *name;
	int id;
};

const struct Region *in_word_set(register const char *str, register size_t len);

#endif /* !__RRC_H_ */
