curl -sSfLO "https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage"
chmod a+x linuxdeploy*.AppImage
curl -sSfLO "https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage" 
chmod a+x appimagetool*.AppImage
 
mkdir -p AppDir/usr/bin
mkdir -p AppDir/etc/ld.so.conf.d/
echo "/usr/lib32" > AppDir/etc/ld.so.conf.d/lib32-glibc.conf
cp appimage/.github/workflows/scripts/{soh.desktop,soh.png,soh.sh} AppDir/
 
mkdir -p AppDir/usr/share/applications 
mkdir -p AppDir/usr/share/icons/hicolor/scalable/apps
mkdir -p AppDir/usr/{lib,lib32}

mv AppDir/soh.sh AppDir/usr/bin
#cp soh.elf
#cp OTRGui
#cp assets
#cp assets/extractor/ZAPD.out
chmod +x AppDir/usr/bin/{soh.elf,OTRGui,soh.sh}
 
cd AppDir && ln -s ./usr/bin/soh.sh ./AppRun && cd ..
 
ARCH=i386 ./linuxdeploy-x86_64.AppImage --appdir="$GITHUB_WORKSPACE"/AppDir/ -d "$GITHUB_WORKSPACE"/AppDir/soh.desktop -i "$GITHUB_WORKSPACE"/AppDir/soh.png -e "$GITHUB_WORKSPACE"/AppDir/usr/bin/soh.elf
 
cp /lib/i386-linux-gnu/{libGL.so.1,libstdc++.so.6,libm.so.6,libgcc_s.so.1,libc.so.6,libxcb.so.1,libGLdispatch.so.0,libGLX.so.0} AppDir/usr/lib32
cd AppDir && ln -s ./usr/lib32/ld-linux.so.2 ./usr/lib/ && cd ..
cp -r /lib/i386-linux-gnu/pulseaudio AppDir/usr/lib32/
mkdir AppDir/usr/lib32/dri && cp /lib/i386-linux-gnu/dri/swrast_dri.so AppDir/usr/lib32/dri/
 
./appimagetool-x86_64.AppImage "$GITHUB_WORKSPACE"/AppDir "SOH-Linux.AppImage"

mkdir -p "$GITHUB_WORKSPACE"/artifacts/ 
mv "SOH-Linux.AppImage" "$GITHUB_WORKSPACE"/artifacts/
