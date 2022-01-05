@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

if not exist build mkdir build
pushd build

rem 0=Debug, 1=Release
set BuildMode=0

rem 0=MSVC
set Compiler=0

rem 0=win32
set Platform=0

rem 0=x64
set Architecture=0

if %Architecture%==0 set arch=x64
if %Platform%==0 set os=win

set CompilerSwitches=%CompilerSwitches% /nologo /std:c17 /fp:fast /EHa- /FAs /FC /GF /GR- /GS- /Gs0x100000 /J /WX /Wall /X
set CompilerSwitches=%CompilerSwitches% /wd5045 /wd4820 /wd4201 /wd4061 /wd4062 /wd4127 /wd4214 /wd4189
set CompilerSwitches=%CompilerSwitches% /D_BUILD_MODE=%BuildMode% /D_COMPILER=%Compiler% /D_PLATFORM=%Platform% /D_ARCHITECTURE=%Architecture%
set LinkerSwitches=%LinkerSwitches% /wx /incremental:no /opt:ref /opt:icf /nodefaultlib /subsystem:windows /stack:0x100000,0x100000 /machine:%arch%
set DLLSwitches=%DLLSwitches% /noimplib /noentry

if %BuildMode%==0 (
    set CompilerSwitches=%CompilerSwitches% /Od /Z7 /Oi
)
if %BuildMode%==1 (
    set CompilerSwitches=%CompilerSwitches% /O2 /Zo
)

if exist *.pdb del *.pdb > NUL 2> NUL
echo WAITING FOR PDB > lock.tmp

echo Building Module 'Game'
cl %CompilerSwitches% /D_MODULE=1 /I ..\src\ ..\src\game\module.c /LD /link %LinkerSwitches% %DLLSwitches% /pdb:Game_%random%.pdb /out:Game.dll

echo Building Module 'Util'
cl %CompilerSwitches% /D_MODULE=2 /I ..\src\ ..\src\util\module.c /LD /link %LinkerSwitches% %DLLSwitches% /pdb:Util_%random%.pdb /out:Util.dll

del lock.tmp
echo Building Module 'Platform'
cl %CompilerSwitches% /D_MODULE=0 /I ..\src\ ..\src\platform\module.c /link %LinkerSwitches% /entry:Platform_Entry /out:Platform.exe

if not [%~1]==[] (
    cl %CompilerSwitches% /D_MODULE=0 /I ..\src\ "..\src\tools\%~1.c" /link /entry:main %linkerSwitches% /out:"%~1.exe"
)

if exist *.obj del *.obj
if exist *.exp del *.exp
popd
exit /b 0