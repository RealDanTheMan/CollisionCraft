echo off
setlocal

set CWD=%CD%
set BUILD_PATH="%CWD%/build"
echo "Build Path: %BUILD_PATH%"

rmdir /s /q %BUILD_PATH%
mkdir %BUILD_PATH%
mkdir %BUILD_PATH%/bin

cmake -B%BUILD_PATH% -S . -DCMAKE_TOOLCHAIN_FILE=C:\dev\packages\vcpkg\vcpkg\scripts\buildsystems\vcpkg.cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release
cmake --build %BUILD_PATH% --config Release

cd %CWD%
endlocal