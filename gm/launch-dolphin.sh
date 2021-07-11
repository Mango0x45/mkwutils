#!/usr/bin/env sh

# Launch dolphin with the given savefile. The old save will be restored.

[ $# -ne 1 ] && {
	>&2 echo "Usage: $0 rksys.dat"
	exit 1
}

DOLPHIN_DIR="$HOME/.local/share/dolphin-emu/Wii/title/00010004/524d4345/data"

trap "mv /tmp/old_save.dat $DOLPHIN_DIR/rksys.dat" EXIT

cp $DOLPHIN_DIR/rksys.dat /tmp/old_save.dat
cp "$1" $DOLPHIN_DIR/rksys.dat

dolphin-emu
