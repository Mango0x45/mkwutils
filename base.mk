.POSIX:
.SUFFIXES:
CC	= cc
CFLAGS	= -O3 -std=c11 -pipe -pedantic -Wall -Wextra -Wmissing-prototypes -Wstrict-prototypes \
		-Wimplicit-fallthrough -D BYTE_ORDER=$$(../common/check-endianness.sh) -I include \
		-I ../common/

PREFIX	= /usr/local

all:

.SUFFIXES: .gperf .c .o
.gperf.c:
	gperf $< --output-file=$@
.c.o:
	${CC} ${CFLAGS} -c $<

clean:
	rm -f *.gz ${BIN} ${OBJS} ${EXTRAS}

format:
	clang-format -i --style=file $$(find . -name '*.[ch]')

install-man:
	command -v gzip >/dev/null && GZIP=1; \
	for manpage in ${MAN}; do \
		section=$$(echo $$manpage | cut -d '.' -f 2); \
		mkdir -p ${PREFIX}/share/man/man$$section; \
		if [ "$$GZIP" ]; then \
			gzip -c9 <$$manpage >$$manpage.gz; \
			manpage=$$manpage.gz; \
		fi; \
		cp $$manpage ${PREFIX}/share/man/man$$section; \
	done

.PHONY: clean format install-man
