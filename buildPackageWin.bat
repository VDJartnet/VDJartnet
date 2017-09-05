set "VSCMD_START_DIR=%CD%"

::call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall" x86
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat"

cl /Fo.\ConfigWin.obj /c /clr /MD .\ConfigWin.cpp
cl /Fe.\VDJartnet.dll /LD /EHsc /MD .\VDJartnet.cpp .\main.cpp .\ConfigWin.obj /link /LIBPATH:"C:\Program Files\Microsoft Visual Studio 14.0\VC\lib"

echo README > README.rtf
mkdir "%TEMP%\pandoc\"
powershell -command "& { (New-Object Net.WebClient).DownloadFile(\"https://github.com/jgm/pandoc/releases/download/1.19.2.1/pandoc-1.19.2.1-windows.msi\", \"%TEMP%\pandoc.msi\") }"
start /wait msiexec /i "%TEMP%\pandoc.msi" /qn TARGETDIR="%TEMP%\pandoc" /Lvx* "%TEMP%\pandoc_install.log"

"%TEMP%\pandoc\pandoc.exe" -s -S -f markdown_github -t rtf .\README.md -o .\README.rtf
::"%APPDATA%\..\Local\Pandoc\pandoc.exe" -s -S -f markdown_github -t rtf .\README.md -o .\README.rtf

candle winInstaller.wxs ReadmeDlg.wxs
light -ext WixUIExtension -out VDJartnet.msi winInstaller.wixobj ReadmeDlg.wixobj

::rmdir /s /q "%TEMP%\pandoc\"
