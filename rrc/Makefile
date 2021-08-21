.POSIX:
include ../base.mk

BIN	= rrc
MAN	= rrc.1
OBJS	= rrc.o regions.o
EXTRAS	= regions.c

# gperf(1) is weird
CFLAGS	+= -Wno-missing-field-initializers

all: ${BIN}
${BIN}: ${OBJS}
	${CC} -o $@ ${OBJS}

rrc.o: rrc.c rrc.h ../common/common.h ../common/crc32.h
regions.o: regions.c rrc.h
regions.c: regions.gperf rrc.h

install: install-man
	mkdir -p ${PREFIX}/bin
	cp ${BIN} ${PREFIX}/bin

.PHONY: install
