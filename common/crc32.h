#ifndef __CRC32_H_
#define __CRC32_H_

#include <stddef.h>
#include <stdint.h>

inline uint32_t
swap32(uint32_t x)
{
#if defined(__GNUC__) || defined(__clang__)
	return __builtin_bswap32(x);
#else
	return (x >> 24) | ((x >> 8) & 0x0000FF00) | ((x << 8) & 0x00FF0000) | (x << 24);
#endif
}

uint32_t crc32(const void *p, size_t length);

#endif /* !__CRC32_H_ */
