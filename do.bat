@echo off
setlocal
if "%1"=="build" call :build %2 %3 %4
if "%1"=="tools" call :build_tool %2 %3
if "%1"=="shell" call :shell
if "%1"=="debug" call :debug
if "%1"=="kwd" call :check_keyword %~2
if "%1"=="cmt" call :check_comment %~2
if "%1"=="lines" call :line_count %~2
endlocal
exit /b 0



:build
rem call "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat" x64
call "E:\\Andrew\\Programs\\VisualStudio\\BuildTools\\VC\\Auxiliary\\Build\\vcvarsall.bat" x64
if not exist build mkdir build
pushd build

rem //IMPORTANT: Statically link all dlls on release!

rem BuildMode: 0 = Debug, 1 = Release, 2 = Shipment
set BuildMode=0
rem Compiler: 0 = MSVC
set Compiler=0
rem Platform: 0 = Windows
set Platform=0
rem Loader: 0 = Win32
set Loader=0
rem Architecture: 0 = x64, 1 = x86
set Architecture=0

if %Architecture%==0 set arch=x64
if %Platform%==0 set os=win

set CompilerSwitches=%CompilerSwitches% /nologo /std:c11 /fp:fast /EHa- /FC /GF /GR- /GS- /Oi /J /WX /W4 /Wall /Gs0x100000 /Zc:preprocessor
set CompilerSwitches=%CompilerSwitches% /wd4201 /wd4204 /wd4214 /wd4820 /wd5045 /wd4710 /wd4711 /wd4296 /wd4127 /wd4189 /wd4668
set CompilerSwitches=%CompilerSwitches% /D_MODE=%BuildMode% /D_COMPILER=%Compiler% /D_PLATFORM=%Platform% /D_LOADER=%Loader% /D_ARCH=%Architecture%
set LinkerSwitches=%LinkerSwitches% /wx /incremental:no /opt:ref /opt:icf /nodefaultlib /subsystem:windows /stack:0x100000,0x100000 /machine:%arch%
set DllLinkerSwitches=%DllLinkerSwitches% /noimplib /noentry

if %BuildMode%==0 (
    set CompilerSwitches=%CompilerSwitches% /Od /Z7 /FAs /Wv:18
    set LinkerSwitches=%LinkerSwitches%
)
if %BuildMode%==1 (
    set CompilerSwitches=%CompilerSwitches% /O2 /Zo
    set LinkerSwitches=%LinkerSwitches%
)
if %BuildMode%==2 (
    set CompilerSwitches=%CompilerSwitches% /O2 /wd4100 /wd4061 /wd4062 /FAs
    set LinkerSwitches=%LinkerSwitches%
)

rem Module: 0 = Platform, 1 = Base, 2 = Util
set BuildPlatform=1
set BuildBase=1
set BuildUtil=1
if not %1==-1 (
    if not %1==0 set BuildPlatform=0
    if not %1==1 set BuildBase=0
    if not %1==2 set BuildUtil=0
)

rem Use the compiler swith /P for preprocessor output
if exist *.pdb del *.pdb > NUL 2> NUL
echo WAITING FOR PDB > lock.tmp

if %BuildUtil%==1 (
    echo. & echo Building Voxarc_Util.dll
    cl %CompilerSwitches% /D_MODULE=2 /I ..\src\ ..\src\util\module.c /LD /link %LinkerSwitches% %DllLinkerSwitches% /pdb:Voxarc_Util_%random%.pdb /out:Voxarc_Util.dll
)
if %BuildBase%==1 (
    echo. & echo Building Voxarc_Game.dll
    cl %CompilerSwitches% /D_MODULE=1 /I ..\src\ ..\src\game\game.c /LD /link %LinkerSwitches% %DllLinkerSwitches% /pdb:Voxarc_Game_%random%.pdb /out:Voxarc_Game.dll
)
del lock.tmp
if %BuildPlatform%==1 (
    echo. & echo Building Voxarc_%os%_%arch%.exe
    @REM cl %CompilerSwitches% /D_MODULE=0 /I ..\src\ ..\src\platform\platform.c /link %LinkerSwitches% /out:Voxarc_Win32.exe
    cl %CompilerSwitches% /D_MODULE=0 /I ..\src\ ..\src\platform\module.c /link %LinkerSwitches% /entry:Platform_Entry /out:Voxarc_%os%_%arch%.exe
)

if not %2==0 (
    cl %CompilerSwitches% /D_MODULE=0 /I ..\src\ ..\src\tools\%2 /link /entry:main %linkerSwitches% /out:%3
)

if exist *.obj del *.obj
if exist *.exp del *.exp
popd
exit /b 0



:shell
start cmd.exe /k call "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat" x64
exit 0



:debug
devenv main.exe
exit 0



:check_keyword
cls
pushd code
set Wildcard=*.h *.cpp *.inl *.c
findstr -s -n -i -l %1 %Wildcard%
popd
exit /b 0



:check_comment
cls
setlocal enabledelayedexpansion
pushd code
if "%1"=="" (
    set wildcard=*.h *.cpp *.inl *.c
) else (
    set wildcard=%1
)
set color=90
for %%G in (%wildcard%) do (
    echo [0m -------- & echo %%G
    for /f "tokens=*" %%H in (%%G) do (
        set x=%%H
        if defined x (
            if not "!x:IMPORTANT=!"=="%%H" (
                set color=93
            ) else (
                if not "!x:TODO=!"=="%%H" (
                    set color=91
                ) else (
                    if not "!x:NOTE=!"=="%%H" (
                        set color=92
                    ) else (
                        if "!x://=!"=="%%H" (
                            set color=90
                        )
                    )
                )
            )
            if not "!x://=!"=="%%H" (
                echo [1;!color!m %%H
            )
        )
    )
)
echo [0m
popd
exit /b 0



:line_count
for /r %%f in (%*) do (
    find /c /v "" %%f
)
echo TOTAL:
(for /r %%f in (%*) do @type "%%f") | find /c /v ""
exit /b 0