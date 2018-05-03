set "VSCMD_START_DIR=%CD%"

call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat"

msbuild .\VDJartnet.vcxproj /p:configuration=Release /p:platform=Win32

::candle winInstaller.wxs ReadmeDlg.wxs
::light -ext WixUIExtension -out VDJartnet.msi winInstaller.wixobj ReadmeDlg.wixobj
