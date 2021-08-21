.POSIX:
include ../base.mk

BIN	= rrc
MAN	= rrc.1
OBJS	= ../common/common.o ../common/crc32.o rrc.o regions.o
EXTRAS	= regions.c

# gperf(1) is weird
CFLAGS	+= -Wno-missing-field-initializers

all: ${BIN}
${BIN}: ${OBJS}
	${CC} -o $@ ${OBJS}

../common/common.o: ../common/common.c
	${CC} ${CFLAGS} -c $< -o $@
../common/crc32.o: ../common/crc32.c
	${CC} ${CFLAGS} -c $< -o $@
rrc.o: rrc.c rrc.h ../common/common.h ../common/crc32.h
regions.o: regions.c rrc.h
regions.c: regions.gperf rrc.h

install: install-man
	mkdir -p ${PREFIX}/bin
	cp ${BIN} ${PREFIX}/bin

.PHONY: install
