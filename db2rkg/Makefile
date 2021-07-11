.POSIX:

include ../base.mk

MAN	= db2rkg.1

install: db2rkg.sh install-man
	mkdir -p ${PREFIX}/bin
	FILE="$<" && cp $< ${PREFIX}/bin/$${FILE%%.sh}

.PHOHY: install
