call "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat" x64

pushd tests
if not exist build mkdir build
pushd build

set compilerSwitches=/Od /MTd /nologo /fp:fast /fp:except- /Gm- /GR- /EHa- /Zo /Oi /WX /W4 /FC /Zi /std:c++17 /D_CRT_SECURE_NO_WARNINGS
set compilerSwitches=/D_VOX_DEBUG=1 %compilerSwitches%
set compilerSwitches=/wd4100 /wd4189 /wd4201 /wd4505 /wd4456 %compilerSwitches%
set linkerSwitches=/incremental:no /opt:ref /debug

@REM cl %compilerSwitches% /I ..\ ..\test_dll.c /LD /link %linkerSwitches% /noentry /out:Test2.dll
cl %compilerSwitches% /FAs /I ..\ ..\test_main.c /link %linkerSwitches% /out:Test.exe

popd
popd

exit /b 0