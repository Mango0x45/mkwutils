#!/usr/bin/env sh

trap "rm *.dat" EXIT

[ -f ./rrc ] || make

cp saves/* .
for REGION in americas europe korea taiwan china japan oceania; do
	./rrc -r $REGION $REGION.dat
	if cmp $REGION.dat saves/$REGION.dat; then
		echo [$REGION] Success
	else
		echo [$REGION] Failure
	fi
done
