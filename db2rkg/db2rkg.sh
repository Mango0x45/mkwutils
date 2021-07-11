#!/usr/bin/env sh

parse () {
	sed -n '
	/\(ninrankings\|maschell\)/ {
		s/ghostviewer/download/
		p
	}

	/chadsoft/ {
		s/\.html/.rkg/
		p
	}
	' <"$FILE"
}

if [ "$#" -eq 0 ]; then
	parse /dev/stdin
else
	trap "rm -f /tmp/db2rkg_$$" EXIT
	for ARG in "$@"; do
		if [ -f "$ARG" ]; then
			FILE="$ARG"
		elif [ "$ARG" = "-" ]; then
			FILE=/dev/stdin
		else
			FILE=/tmp/db2rkg_$$
			echo "$ARG" >$FILE
		fi

		parse "$FILE"
	done
fi
