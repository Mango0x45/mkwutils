.POSIX:
include ../base.mk

BIN	= gm
OBJS	= ../common/common.o ../common/crc32.o ghost.o gm.o lookup.o target.o utf.o

all: ${BIN}
${BIN}: ${OBJS}
	${CC} -o $@ ${OBJS}

../common/common.o: ../common/common.c
	${CC} ${CFLAGS} -c $< -o $@
../common/crc32.o: ../common/crc32.c
	${CC} ${CFLAGS} -c $< -o $@
ghost.o: src/ghost.c ../common/common.h ../common/crc32.h include/ghost.h include/gm.h include/utf.h include/lookup.h
	${CC} ${CFLAGS} -c $<
gm.o: src/gm.c include/ghost.h include/gm.h include/target.h
	${CC} ${CFLAGS} -c $<
lookup.o: src/lookup.c
	${CC} ${CFLAGS} -c $<
target.o: src/target.c include/ghost.h include/gm.h include/lookup.h include/target.h
	${CC} ${CFLAGS} -c $<
utf.o: src/utf.c include/ghost.h include/gm.h include/utf.h
	${CC} ${CFLAGS} -c $<

install:
	mkdir -p ${PREFIX}/bin
	cp ${BIN} ${PREFIX}/bin

.PHONY: install
