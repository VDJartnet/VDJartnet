"C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall" x86
cl /Fo.\ConfigWin.obj /c /clr /MD .\ConfigWin.cpp
cl /Fe.\VDJartnet.dll /LD /EHsc /MD .\VDJartnet.cpp .\main.cpp .\ConfigWin.obj /link /LIBPATH:"C:\Program Files\Microsoft Visual Studio 14.0\VC\lib"
