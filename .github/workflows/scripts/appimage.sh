curl -sSfLO "https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage"
chmod a+x linuxdeploy*.AppImage
curl -sSfLO "https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage" 
chmod a+x appimagetool*.AppImage
 
mkdir -p AppDir/usr/bin
mkdir -p AppDir/etc/ld.so.conf.d/
echo "/usr/lib32" > AppDir/etc/ld.so.conf.d/lib32-glibc.conf
cp appimage/.github/workflows/scripts/{soh.desktop,soh.png,soh.sh} AppDir/
curl -sSfL https://raw.githubusercontent.com/gabomdq/SDL_GameControllerDB/master/gamecontrollerdb.txt -o AppDir/usr/bin/gamecontrollerdb.txt

mkdir -p AppDir/usr/share/applications 
mkdir -p AppDir/usr/share/icons/hicolor/scalable/apps
mkdir -p AppDir/usr/{lib,lib32}

#cp -r /usr/lib/i386-linux-gnu/alsa-lib AppDir/usr/lib32/
#mkdir -p AppDir/usr/share/alsa
#cp /usr/share/alsa/alsa.conf AppDir/usr/share/alsa/

mv AppDir/soh.sh AppDir/usr/bin
cp -r Release/* AppDir/usr/bin

chmod +x AppDir/usr/bin/{soh.elf,OTRGui,soh.sh}
 
cd AppDir && ln -s ./usr/bin/soh.sh ./AppRun && cd ..

export UPD_INFO="gh-releases-zsync|qurious-pixel|Shipwright-linux|continuous|SOH-Linux.AppImage.zsync"
ARCH=i386 ./linuxdeploy-x86_64.AppImage         \
  --appdir="$GITHUB_WORKSPACE"/AppDir/          \
  -d "$GITHUB_WORKSPACE"/AppDir/soh.desktop     \
  -i "$GITHUB_WORKSPACE"/AppDir/soh.png         \
  -e "$GITHUB_WORKSPACE"/AppDir/usr/bin/soh.elf \
  -e "$GITHUB_WORKSPACE"/AppDir/usr/bin/assets/extractor/ZAPD.out
 
cp /lib/i386-linux-gnu/{ld-linux.so.2,libstdc++.so.6,libm.so.6,libgcc_s.so.1,libc.so.6,libxcb.so.1,libGLdispatch.so.0,libGLX.so.0,libGL.so.1} AppDir/usr/lib32
cp /lib/i386-linux-gnu/{libpthread.so.0,libdl.so.2,libz.so.1,libXau.so.6,libXdmcp.so.6,libXext.so.6,librt.so.1,libbsd.so.0,libresolv.so.2,libgpg-error.so.0,libX11.so.6} AppDir/usr/lib32
cd AppDir && ln -s ./usr/lib32/ld-linux.so.2 ./usr/lib/ && cd ..
cp -r /lib/i386-linux-gnu/pulseaudio AppDir/usr/lib32/
mkdir AppDir/usr/lib32/dri && cp /lib/i386-linux-gnu/dri/swrast_dri.so AppDir/usr/lib32/dri/

mv "$GITHUB_WORKSPACE"/AppDir/usr/bin/ZAPD.out "$GITHUB_WORKSPACE"/AppDir/usr/bin/assets/extractor/ZAPD.out
cd "$GITHUB_WORKSPACE"/AppDir/usr/bin/assets/extractor/
patchelf --set-rpath "\$ORIGIN/../../../lib32" ZAPD.out
cd "$GITHUB_WORKSPACE"
 
./appimagetool-x86_64.AppImage "$GITHUB_WORKSPACE"/AppDir "SOH-Linux.AppImage" -u "gh-releases-zsync|qurious-pixel|Shipwright-linux|continuous|SOH-Linux.AppImage.zsync"

mkdir -p "$GITHUB_WORKSPACE"/artifacts/ 
mv SOH-Linux.AppImage* "$GITHUB_WORKSPACE"/artifacts/
ls -al artifacts
