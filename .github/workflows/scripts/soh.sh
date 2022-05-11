#!/bin/bash
HERE="$(dirname "$(readlink -f "${0}")")"/../..

export LD_LIBRARY_PATH="$HERE/usr/lib":$LD_LIBRARY_PATH
export LD_LIBRARY_PATH="$HERE/usr/lib32":$LD_LIBRARY_PATH
export LD_LIBRARY_PATH="$HERE/lib":$LD_LIBRARY_PATH

#Sound Library
export LD_LIBRARY_PATH="$HERE/usr/lib32/pulseaudio":$LD_LIBRARY_PATH
export LD_LIBRARY_PATH="$HERE/usr/lib32/alsa-lib":$LD_LIBRARY_PATH
export LADSPA_PATH="$HERE/usr/lib32/ladspa":$LADSPA_PATH
export AO_PLUGIN_PATH="$HERE/usr/lib32/ao/plugins-4":$AO_PLUGIN_PATH

export PATH=$HERE/bin:$HERE/usr/bin:$PATH
export LIBGL_DRIVERS_PATH="$HERE/usr/lib32/:"$HERE/usr/lib32/dri":$LIBGL_DRIVERS_PATH
export MAIN32LDLIBRARY="$HERE/usr/lib32/ld-linux.so.2"

if [[ ! -e $PWD/oot.otr ]]; then 
	export ASSETDIR=$(mktemp -d /tmp/assets-XXXXX)
	ln -s $HERE/usr/bin/{assets,soh.elf,OTRGui} $ASSETDIR
	export OLDPWD=$PWD
	ln -s $OLDPWD/*.*64 $ASSETDIR
	cd $ASSETDIR
	MAIN="$ASSETDIR/OTRGui"
	"$MAIN32LDLIBRARY" "$MAIN"
	cp "$ASSETDIR"/oot.otr $OLDPWD
	sleep 3
	rm -r "$ASSETDIR"
else
	ln -s "$HERE/usr/bin/gamecontrollerdb.txt" $PWD
	MAIN="$HERE/usr/bin/soh.elf"
	"$MAIN32LDLIBRARY" "$MAIN"
	unlink $PWD/gamecontrollerdb.txt
fi
exit
