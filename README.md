# Welcome to the Ship of Harkinian 
## Linux Twisted build !
This a PC port of OOT bla bla you know it if you are there.
Here some usefull information : 
```
Build team: `zelda@srd022j`
Build date: `03-02-21 00:49:18` (year-month-day)
sha1: cee6bc3c2a634b41728f2af8da54d9bf8cc14099
```
This build feature all my mods pre-installed, they can be toggle on and off with the DEV menu.
DEV Menu can be open with F1.
This build is still not playable there is some crash and bug there and there.
I will manually add PR from the main repo -> https://github.com/HarbourMasters/Shipwright

Before starting be sure to open a terminal or what ever you use in a folder that has no space in it's path.
How to build (All thanks to Random :) )
```
git clone https://github.com/Baoulettes/Shipwright-linux.git
cd Shipwright-linux
```

Add your rom in soh folder baserom_non_mq.z64 sha1:cee6bc3c2a634b41728f2af8da54d9bf8cc14099

## Docker Procedure
Now do the docker build (be sure to have docker :P)
```
sudo docker build . -t soh
sudo docker run --rm -it -v $(pwd):/soh soh /bin/bash
```

now you will be in "docker mode"
Do these one by one and it should be fine 

```
git clone https://github.com/ladislav-zezula/StormLib external/StormLib
cmake -B external/StormLib/build -S external/StormLib
cmake --build external/StormLib/build
cp external/StormLib/build/libstorm.a external
cp /usr/local/lib/libGLEW.a external

cd soh
make setup -j$(nproc)
make -j $(nproc)
```

# Original Readme in case :)!
# Welcome to the Ship of Harkinian!

A PC port of OoT allowing you to enjoy the game with modern controls, widescreen, high-resolution, gyroscopy and other great features! Setup is simple, let's get started!

The Ship does not include assets and as such requires a prior copy of the game to play.

## Quick Start

1) Download The Ship of Harkinian from Discord.
2) Requires an `oot debug` rom (not Master Quest).
```
Build team: `zelda@srd022j`
Build date: `03-02-21 00:49:18` (year-month-day)
sha1: cee6bc3c2a634b41728f2af8da54d9bf8cc14099
```
3) Use the OTRGui to generate an `oot.otr` archive file.
4) Launch `soh.exe`

Congratulations, you are now sailing with the Ship of Harkinian! Have fun!

## Additional Setup Information

The Ship of Harkinian uses a proprietary versioning system consisting of a sci-fi film character followed by a phonetic alphabet code word. The film character represents a major release version which increments with the addition of many new features and bug fixes. The code word represents a minor release version which increments with small updates mainly comprised of bug fixes. For example, `DECKARD ALFA`.

### The Extraction Tool

* Open a rom to initiate generating the `oot.otr` archive file.
* If a second button exists then `oot.otr` already exists. To prevent overwriting the old `oot.otr` use this button to choose a new game directory. The new directory must not already contain an `oot.otr` to prevent an error.
* When the process completes, place `oot.otr` beside `soh.exe` if it is not already.

This packaging process can take up to **5 minutes**.

Close the OTRGui when the `Done!` message appears.
If you get another message, then you might have selected the wrong rom. Make sure to use a rom consistent with the above checksum.

If you still cannot get the tool to work, join our [Discord Server](https://discord.com/invite/BtBmd55HVH) and ask for help in the `#support` text channel. Keep-in-mind that we do not condone piracy in any way.

### Running The Ship of Harkinian

Launch the game. If the window immediately closes, or if there are visual artifacts, you may have selected the wrong rom in the OTRGui tool.

Currently, DirectX 11 and OpenGL is supported. Change the renderer by opening the `shipofharkinian.ini` configuration file in notepad and add `sdl` to `gfx backend` for OpenGL or leave blank for DirectX.

## Take The Survey
Want to use cartridge readers in tandem with the OTRGui?
Take [this survey](https://retroarchopenhardware.com/survey.php) to increase chances of this becoming reality.

## Discord

Official Discord: https://discord.com/invite/BtBmd55HVH

## Building The Ship of Harkinian

### Windows

 1. Install [Python](https://www.python.org/ftp/python/3.10.2/python-3.10.2-amd64.exe)
 2. Install [Visual Studio 2022 Community Edition](https://visualstudio.microsoft.com/vs/community/)
 2b. In the Visual Studio Installer, install `MSVC v142 - VS 2019 C++`.
 4. Clone the Ship of Harkinian repository.
 5. Place `oot debug` rom (not Master Quest) in the `soh` folder named `baserom_original_non_mq`.
 6. Launch `soh/fixbaserom.py`.
 7. Launch `soh/extract_baserom.py`.
 8. Copy the `baserom` folder from the `soh` folder into the `OTRExporter` folder.
 9. Run `OTRExporter/OTRExporter.sln`.
 10. Switch the solution to `Release x64`.
 11. Build the solution.
 12. Launching `OTRExporter/extract_assets.py` will generate an `oot.otr` archive file in `OTRExporter/oot.otr`.
 13. Run `soh/soh.sln`
 14. Switch the solution to `Release x86`.
 15. Build the solution.
 16. Copy the `OTRExporter/oot.otr` archive file to `soh/Release`.
 17. Launch `soh.exe`.

### Linux

```bash
# Clone the repo
git clone git@github.com:HarbourMasters/ShipWright.git
cd ShipWright
# Copy the baserom to the soh folder
cp .../baserom_non_mq.z64 soh
# Build the docker image
sudo docker build . -t soh
# Run the docker image with the working directory mounted to /soh
sudo docker run --rm -it -v $(pwd):/soh soh /bin/bash
```
Inside the Docker container:
```bash
# Clone and build StormLib
git clone https://github.com/ladislav-zezula/StormLib external/StormLib
cmake -B external/StormLib/build -S external/StormLib
cmake --build external/StormLib/build
cp external/StormLib/build/libstorm.a external
cp /usr/local/lib/libGLEW.a external

cd soh
# Extract the assets/Compile the exporter/Run the exporter
make setup -j$(nproc)
# Compile the code
make -j $(nproc)
```

## Troubleshooting The Exporter
- Affirm that you have an `/assets` folder filled with XMLs in the same directory as OTRGui.exe
- Affirm that `zapd.exe` exists in the `/assets/extractor` folder


## The Harbour Masters Are...

    Kenix 			| Lead Developer/Public Relations - Resource Management Programmer, Audio System Programmer, and General Programmer
    Jack Walker 		| Lead Developer - OTR Format Programmer, Resource Load Programmer, and General Programmer
    Louist103 		| Developer - Save System Programmer and General Programmer
    Emil 			| Developer - Fast3D Programmer
    m4xw 			| Developer - Shipwright, Throwing Baguettes, and General Programmer
    MelonSpeedruns 		| Developer - General Programmer
    Rozlette 		| Developer - General Programmer
    JoshDuMan 		| Developer - General Programmer
    KiritoDev/Lywx 		| Developer - General Programmer
    Theo3 			| Developer - General Programmer
	Random06457  		| Developer - Linux Build

## Special Thanks

    Decomp & ZAPD 		| Made this project even possible in the first place!
    MNGoldenEagle 		| Patiently explained audio data formats, encouragement, and founding ZSO which was the first source of the game's code and resource format documentation.
    Rrrrry123 		| Speedbunner, encouragement, and community moderation
    Fierce deity 		| Encouragement and community moderation
    mzxrules		| For his contributions to decomp
    zel.		        | For his contributions to decomp
    Aloxado 		| Developer - General Programmer
    MegaMech 		| Developer - General Programmer
	Revo 			| Tester - GCC support and General Testing
	zfg 			| Tester - General Testing
	Horseless Headman 	| Tester - General Testing
    Steven Pritchett        | Tester - General Testing
	Trenton May 		| Tester - General Testing
	Zeldaboy14 		| Tester - General Testing, encouragement, and community moderation
	Koby Howell 		| Tester - General Testing
	Logg 			| Tester - General Testing
	Taylor Daley		| Graphic Design
	Can't Sleep 		| Graphic Design
    MicTheMicrophone 	| Voice actor for the King
    Amphibibro		| Voice actor for Link

Lemons
