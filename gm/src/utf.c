/*
 * The amount of pain and suffering I had to go through to make the conversion from UTF-16BE to
 * UTF-8 work is something I cannot describe. The amount of resources available on this is very low
 * and they are hard to come by. After many nights of painful suffering I think I have finally got a
 * proper solution. None of this would be possible without me finding this amazing website at 01:57:
 *
 *	https://www.lemoda.net/c/iconv-example/iconv-example.html
 */

#include <errno.h>
#include <iconv.h>
#include <stdio.h>
#include <stdlib.h>

#include "ghost.h"
#include "gm.h"
#include "utf.h"

unsigned int
utf8_strlen(const char *s)
{
	unsigned int i = 0;
	while (*s) {
		if ((*s & 0xC0) != 0x80)
			i++;
		s++;
	}
	return i;
}

static iconv_t
conv_init(void)
{
	iconv_t cd = iconv_open("UTF-8", "UTF-16BE");
	if (cd == (iconv_t) -1) {
		if (errno == EINVAL)
			die("Conversion from UTF-16BE to UTF-8 is not supported");
		else
			die("iconv_open");
	}
	return cd;
}

void
utf16_to_utf8(char *utf16, char *utf8)
{
	iconv_t cd = conv_init();

	size_t utf8_len = MII_NAME_LEN * 4;
	size_t utf16_len = MII_NAME_LEN * 2;

	if (iconv(cd, &utf16, &utf16_len, &utf8, &utf8_len) == (size_t) -1)
		die("iconv");

	if (iconv_close(cd) == -1)
		die("iconv_close");
}
