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
call "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat" x64
if not exist build mkdir build
pushd build

rem //IMPORTANT: Statically link Util and Game on release! Inlines don't work with the DLLs

rem BuildMode: 0 = Debug, 1 = Release, 2 = Shipment
set BuildMode=0

rem Compiler: 0 = MSVC
set Compiler=0

rem Platform: 0 = Windows
set Platform=0

rem Architecture: 0 = x64, 1 = ARM
set Architecture=0

set CompilerSwitches=%CompilerSwitches% /nologo /std:c11 /fp:fast /EHa- /FC /GF /GR- /GS- /Oi /J /WX /W4 /Wall /Gs0x100000 /Zc:preprocessor
set CompilerSwitches=%CompilerSwitches% /wd4201 /wd4204 /wd4214 /wd4820 /wd5045 /wd4710 /wd4711 /wd4296 /wd4127 /wd4189
set CompilerSwitches=%CompilerSwitches% /D_VOX_BUILD_MODE=%BuildMode% /D_VOX_COMPILER=%Compiler% /D_VOX_PLATFORM=%Platform% /D_VOX_ARCHITECTURE=%Architecture%
set LinkerSwitches=%LinkerSwitches% /wx /incremental:no /opt:ref /nodefaultlib /subsystem:windows /stack:0x100000,0x100000 /machine:x64
set DllLinkerSwitches=%DllLinkerSwitches% /noimplib /noentry

if %BuildMode%==0 (
    set CompilerSwitches=%CompilerSwitches% /Od /Z7 /FAs /Wv:18
    set LinkerSwitches=%LinkerSwitches% /opt:noicf
)
if %BuildMode%==1 (
    set CompilerSwitches=%CompilerSwitches% /O2 /Zo
    set LinkerSwitches=%LinkerSwitches% /opt:noicf
)
if %BuildMode%==2 (
    set CompilerSwitches=%CompilerSwitches% /O2
    set LinkerSwitches=%LinkerSwitches% /opt:icf
)

rem Module: 0 = Platform, 1 = Game
set BuildPlatform=1
set BuildGame=1
if not %1==-1 (
    if not %1==0 set BuildPlatform=0
    if not %1==1 set BuildGame=0
)

rem Use the compiler swith /P for preprocessor output
if exist *.pdb del *.pdb > NUL 2> NUL
echo WAITING FOR PDB > lock.tmp

if %BuildGame%==1 (
    REM echo. & echo Building Voxarc_Game.dll
    cl %CompilerSwitches% /D_VOX_MODULE=1 /I ..\src\ ..\src\game\vox_game.c /LD /link %LinkerSwitches% %DllLinkerSwitches% /pdb:Voxarc_Game_%random%.pdb /out:Voxarc_Game.dll
)
del lock.tmp
if %BuildPlatform%==1 (
    REM echo. & echo Building Voxarc_Win32.exe
    cl %CompilerSwitches% /D_VOX_MODULE=0 /I ..\src\ ..\src\platform\vox_platform.c /D_VOX_EXPORT /link %LinkerSwitches% /out:Voxarc_Win32.exe
)

if not %2==0 (
    cl %CompilerSwitches% /D_VOX_MODULE=0 /I ..\src\ ..\src\tools\%2 /link /entry:main %linkerSwitches% /out:%3
)

if exist *.obj del *.obj
if exist *.exp del *.exp
popd
exit /b 0

REM :build_tool
REM call "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat" x64
REM if not exist build mkdir build
REM pushd build
REM set CompilerSwitches=%CompilerSwitches% /Od /MTd /nologo /fp:fast /fp:except- /Gm- /GR- /EHa- /Zo /Oi /WX /W4 /FC /Zi /std:c++17 /D_CRT_SECURE_NO_WARNINGS
REM set CompilerSwitches=%CompilerSwitches% /D_VOX_DEBUG=1
REM set CompilerSwitches=%CompilerSwitches% /wd4100 /wd4189 /wd4201 /wd4505 /wd4456
REM set CompilerSwitches=%CompilerSwitches% /D_VOX_BUILD_MODE=0 /D_VOX_COMPILER=0 /D_VOX_PLATFORM=0 /D_VOX_ARCHITECTURE=0
REM set linkerSwitches=/incremental:no /opt:ref

REM popd
REM exit /b 0

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