ECHO ON
"C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall" x86
ECHO ON
cl /Fo.\ConfigWin.obj /clr /MD .\ConfigWin.cpp
ECHO ON
cl /Fe.\VDJartnet.dll /LD /EHsc /MD .\VDJartnet.cpp .\main.cpp .\ConfigWin.obj
ECHO ON
