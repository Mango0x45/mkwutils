.POSIX:

include ../base.mk

MAN	= db2rkg.1
SRC	= db2rkg.sh

install: install-man
	mkdir -p ${PREFIX}/bin
	FILE="${SRC}" && cp ${SRC} ${PREFIX}/bin/$${FILE%%.sh}

.PHOHY: install
