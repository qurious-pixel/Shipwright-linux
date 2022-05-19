#!/bin/bash

HERE="$(dirname "$(readlink -f "${0}")")"/../..

if [[ ! -e $PWD/oot.otr ]]; then
	export ASSETDIR=$(mktemp -d /tmp/assets-XXXXX)
	ln -s $HERE/usr/bin/{assets,soh.elf} $ASSETDIR
	cp $HERE/usr/bin/OTRGui $ASSETDIR
	export OLDPWD=$PWD
	ln -s $OLDPWD/*.*64 $ASSETDIR
	cd $ASSETDIR
	$ASSETDIR/OTRGui
	cp "$ASSETDIR"/oot.otr $OLDPWD
	sleep 3
	rm -r "$ASSETDIR"
else
	export LD_LIBRARY_PATH="$HERE/usr/lib":$LD_LIBRARY_PATH
	export LD_LIBRARY_PATH="$HERE/usr/lib32":$LD_LIBRARY_PATH
	export LD_LIBRARY_PATH="$HERE/lib":$LD_LIBRARY_PATH
	export LD_LIBRARY_PATH="$HERE/usr/lib32/pulseaudio":$LD_LIBRARY_PATH
	export PATH=$HERE/bin:$HERE/usr/bin:$PATH
	export LIBGL_DRIVERS_PATH="$HERE/usr/lib32/dri":$LIBGL_DRIVERS_PATH
	export MAIN32LDLIBRARY="$HERE/usr/lib32/ld-linux.so.2"
	ln -s "$HERE/usr/bin/gamecontrollerdb.txt" $PWD
	MAIN="$HERE/usr/bin/soh.elf"
	"$MAIN32LDLIBRARY" "$MAIN"
	unlink $PWD/gamecontrollerdb.txt
fi
exit
