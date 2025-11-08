@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x64
cd /d "%~dp0build"
"%~dp0vcpkg\downloads\tools\cmake-3.30.1-windows\cmake-3.30.1-windows-i386\bin\cmake.exe" .. -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_MAKE_PROGRAM="%~dp0vcpkg\downloads\tools\ninja\1.13.1-windows\ninja.exe"
"%~dp0vcpkg\downloads\tools\ninja\1.13.1-windows\ninja.exe"