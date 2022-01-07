@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

if not exist build mkdir build
pushd build

set CompilerSwitches=%CompilerSwitches% /nologo /std:c17 /fp:fast /EHa- /FAs /FC /GF /GR- /GS- /Gs0x100000 /J /WX /Wall /X
set CompilerSwitches=%CompilerSwitches% /wd5045 /wd4820 /wd4201 /wd4061 /wd4062 /wd4127 /wd4214 /wd4189 /wd4100 /wd4094
set CompilerSwitches=%CompilerSwitches% /Od /Z7 /Oi
set LinkerSwitches=%LinkerSwitches% /wx /incremental:no /opt:ref /opt:icf /nodefaultlib /subsystem:windows /stack:0x100000,0x100000 /machine:x64
set DLLSwitches=%DLLSwitches% /noimplib /noentry

cl %CompilerSwitches% /I ..\src\ ..\src\platform\entry.c /link %LinkerSwitches% /entry:Entry /out:Voxarc.exe

if exist *.obj del *.obj
if exist *.exp del *.exp
popd
exit /b 0