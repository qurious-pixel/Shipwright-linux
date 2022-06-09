#!/bin/bash

if [ -e $PWD/*.*64 ]; then
	mkdir tmp
	mkdir -p Extract/assets
	cp *.*64 tmp/rom.z64
	cp -r assets/game/ship_of_harkinian Extract/assets/
	chmod +x assets/extractor/ZAPD.out
	case $(sha1sum -b tmp/rom.z64 | awk '{ print $1 }') in
	cee6bc3c2a634b41728f2af8da54d9bf8cc14099)
  		ROM=GC_NMQ_D;;
	0227d7c0074f2d0ac935631990da8ec5914597b4)
  		ROM=GC_NMQ_PAL_F;;
	*)
  		echo "rom hash does not match";;
	esac
	echo $ROM
	
	assets/extractor/ZAPD.out ed -eh -i assets/extractor/xmls/${ROM} -b tmp/rom.z64 -fl assets/extractor/filelists -o placeholder -osf placeholder -gsf 1 -rconf assets/extractor/Config_${ROM}.xml -se OTR
else
	echo -e "\nPlace ROM in this folder\n"
	exit
fi
exit
