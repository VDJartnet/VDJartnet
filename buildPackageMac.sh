#! /bin/sh

buildDir=/tmp/VDJartnet

rm -rf $buildDir
mkdir $buildDir
mkdir $buildDir/dist
xcodebuild clean
xcodebuild build -project VDJartnet.xcodeproj -scheme VDJartnet -configuration Release SYMROOT=$buildDir/

pkgbuild --identifier uk.kepler22b.vdjartnet.vdjartnet --component $buildDir/Release/VDJartnet.bundle --install-location /Documents/VirtualDJ/Plugins64/AutoStart/ $buildDir/component-vdjartnet.pkg

productbuild --synthesize --package $buildDir/component-vdjartnet.pkg $buildDir/distribution.xml

xsltproc -o $buildDir/distribution.xml ./distributionTransformMac.xsl $buildDir/distribution.xml

cp ./about.html $buildDir/about.html
cp ./COPYING $buildDir/COPYING

productbuild --distribution $buildDir/distribution.xml --resources $buildDir  --package-path $buildDir $buildDir/dist/VDJartnet.pkg

hdiutil create -volname "VDJartnet" -srcfolder $buildDir/dist -ov $buildDir/VDJartnet.dmg
