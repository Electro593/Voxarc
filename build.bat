@echo off
setlocal

call "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat" x64
mkdir build
pushd build
set compilerSwitches=/MD /nologo /fp:fast /Gm- /GR- /EHsc /WX /W1 /FC /Z7 /wd4100 /D_CRT_SECURE_NO_WARNINGS /DVOX_PLATFORM_WINDOWS /DGLEW_STATIC
set linkerSwitches=/incremental:no /opt:ref ..\dependencies\lib\glfw\glfw3.lib ..\dependencies\lib\glew\glew32s.lib user32.lib shell32.lib gdi32.lib opengl32.lib
set filesToCompile=..\src\*.cpp ..\src\math\*.cpp
cl %compilerSwitches% /I ..\dependencies\include\ /I ..\src\ %filesToCompile% /link %linkerSwitches% /out:terranova.exe /nodefaultlib:libcmt.lib
popd

endlocal