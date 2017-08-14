#! /bin/sh

buildDir=/tmp/VDJartnet

rm -rf $buildDir
mkdir $buildDir
mkdir $buildDir/dist
xcodebuild clean
xcodebuild build -project VDJartnet.xcodeproj -scheme VDJartnet -configuration Release SYMROOT=$buildDir/

pkgbuild --identifier uk.kepler22b.vdjartnet.vdjartnet --component $buildDir/Release/VDJartnet.bundle --install-location /Documents/VirtualDJ/Plugins64/AutoStart/ $buildDir/component-vdjartnet.pkg

pkgbuild --identifier uk.kepler22b.vdjartnet.config --nopayload --scripts ./updateConfigMac $buildDir/component-config.pkg

pkgbuild --identifier uk.kepler22b.vdjartnet.presets --nopayload --scripts ./updatePresetsMac $buildDir/component-presets.pkg

productbuild --synthesize --package $buildDir/component-vdjartnet.pkg --package $buildDir/component-config.pkg --package $buildDir/component-presets.pkg $buildDir/distribution.xml

xsltproc -o $buildDir/distribution.xml ./distributionTransformMac.xsl $buildDir/distribution.xml

if [[ `command -v pandoc` ]]
then
  echo "Using existing pandoc"
else
  pandocURL="$(curl -s https://api.github.com/repos/jgm/pandoc/releases/latest | grep '"browser_download_url": ".*\.pkg"' | sed -E 's/"browser_download_url": "(.*\.pkg)"/\1/g')"
  curl -o /tmp/pandoc.pkg -L $pandocURL
  sud0 installer -package /tmp/pandoc.pkg -target /
fi

pandoc -s -S -f markdown_github -t html ./README.md -o $buildDir/README.html
cp ./COPYING $buildDir/COPYING

productbuild --distribution $buildDir/distribution.xml --resources $buildDir  --package-path $buildDir $buildDir/dist/VDJartnet.pkg

hdiutil create -volname "VDJartnet" -srcfolder $buildDir/dist -ov $buildDir/VDJartnet.dmg
