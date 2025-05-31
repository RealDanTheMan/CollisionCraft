echo off
setlocal

set CWD=%CD%
set BUILD_PATH="%CWD%/build"
echo "Build Path: %BUILD_PATH%"

rmdir /s /q %BUILD_PATH%
mkdir %BUILD_PATH%
mkdir %BUILD_PATH%\bin\Release

for /f "usebackq tokens=*" %%i in (`where vcpkg`) do (
    set "VCPKG_EXE=%%i"
    set "VCPKG_PATH=%%~dpi"
)

if "%VCPKG_PATH%" == "" (
    echo "Error: VCPKG is not present in PATH"
    exit /b 1
)

set VCPKG_TOOLCHAIN="%VCPKG_PATH%\scripts\buildsystems\vcpkg.cmake"
echo "Deploying plug-ins -> %BUILD_PATH%\bin\Release"
robocopy "%VCPKG_PATH%\installed\x64-windows\Qt6\plugins\platforms" "%BUILD_PATH%\bin\Release" "qwindows.dll"
robocopy "%VCPKG_PATH%\installed\x64-windows\lib\usd" "%BUILD_PATH%\bin\Release\usd" "plugInfo.json"
robocopy "%VCPKG_PATH%\installed\x64-windows\bin\usd" "%BUILD_PATH%\bin\Release\usd" /E

cmake -B%BUILD_PATH% -S . -DCMAKE_TOOLCHAIN_FILE=%VCPKG_TOOLCHAIN% -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release
cmake --build %BUILD_PATH% --config Release

cd %CWD%
endlocal