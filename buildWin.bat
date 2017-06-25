"C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall" x86
cl /Fo.\ConfigWin.obj /clr /MD .\ConfigWin.cpp
cl /Fe.\VDJartnet.dll /LD /EHsc /MD .\VDJartnet.cpp .\main.cpp .\ConfigWin2.obj