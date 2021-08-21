.POSIX:

include ../base.mk

MAN	= db2rkg.1

install: install-man
	mkdir -p ${PREFIX}/bin
	FILE="$<" && cp $< ${PREFIX}/bin/$${FILE%%.sh}

.PHOHY: install
