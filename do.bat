@echo off
setlocal

if "%1"=="build" call :build
if "%1"=="assets" call :build_asset_packer
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
set compilerSwitches=/Od /MTd /nologo /fp:fast /fp:except- /Gm- /GR- /EHa- /Zo /Oi /WX /W4 /FC /Zi /GS- /Gs9999999 /std:c++17
set compilerSwitches=/wd4100 /wd4189 /wd4201 /wd4505 %compilerSwitches%
set compilerSwitches=/D_VOX_DEBUG=1 /D_VOX_WIN32=1 %compilerSwitches%
set linkerSwitches=/incremental:no /opt:ref /nodefaultlib /subsystem:windows /stack:0x100000,0x100000 /machine:x64
del *.pdb > NUL 2> NUL
echo WAITING FOR PDB > lock.tmp
rem cl %compilerSwitches% /I ..\dependencies\include\ /D_CRT_SECURE_NO_WARNINGS ..\src\voxarc.cpp /link msvcrt.lib /NODEFAULTLIB:libcmt.lib /NODEFAULTLIB:libcmtd.lib %linkerSwitches% /PDB:voxarc_%random%.pdb /out:Voxarc.exe
del lock.tmp
cl %compilerSwitches% /I ..\src\ ..\src\platform\vox_platform_win32.c /link %linkerSwitches% /out:Voxarc.exe
popd
exit /b 0

:build_asset_packer
call "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat" x64
if not exist build mkdir build
pushd build
set compilerSwitches=/Od /MTd /nologo /fp:fast /fp:except- /Gm- /GR- /EHa- /Zo /Oi /WX /W4 /FC /Zi /std:c++17 /D_CRT_SECURE_NO_WARNINGS
set compilerSwitches=/wd4100 /wd4189 /wd4201 /wd4505 /wd4456 %compilerSwitches%
set linkerSwitches=/incremental:no /opt:ref
cl %compilerSwitches% /I ..\src\ ..\src\tools\vox_asset_builder.c /link %linkerSwitches% /out:AssetBuilder.exe
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