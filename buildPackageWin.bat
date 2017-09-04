set "VSCMD_START_DIR=%CD%"

::call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall" x86
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat"

cl /Fo.\ConfigWin.obj /c /clr /MD .\ConfigWin.cpp

cl /Fe.\VDJartnet.dll /LD /EHsc /MD .\VDJartnet.cpp .\main.cpp .\ConfigWin.obj /link /LIBPATH:"C:\Program Files\Microsoft Visual Studio 14.0\VC\lib"

candle winInstaller.wxs

light -ext WixUIExtension winInstaller.wixobj
