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

REM Run CMake build using VS2017 flavour
set VCPKG_TOOLCHAIN="%VCPKG_PATH%\scripts\buildsystems\vcpkg.cmake"
cmake -B%BUILD_PATH% -S . -DCMAKE_TOOLCHAIN_FILE=%VCPKG_TOOLCHAIN% -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release
cmake --build %BUILD_PATH% --config Release

set QT_PLUG_SRC="%VCPKG_PATH%\installed\x64-windows\Qt6\plugins"
set QT_PLUG_DST="%BUILD_PATH%\bin\Release\qt\plugins"
echo "Deploying QT plug-ins -> %QT_PLUG_DST%"



xcopy /E /I /Y "%QT_PLUG_SRC%\imageformats" "%QT_PLUG_DST%\imageformats"
xcopy /E /I /Y "%QT_PLUG_SRC%\platforms" "%QT_PLUG_DST%\platforms"
xcopy /E /I /Y "%QT_PLUG_SRC%\styles" "%QT_PLUG_DST%\styles"

set USD_PLUG_SRC="%VCPKG_PATH%\installed\x64-windows\bin\usd"
set USD_PLUG_DST="%BUILD_PATH%\bin\Release\usd"
echo "Deploying USD plug-ins -> %USD_PLUG_DST%"
xcopy /E /I /Y "%USD_PLUG_SRC%" "%USD_PLUG_DST%"
xcopy /Y "%VCPKG_PATH%\installed\x64-windows\lib\usd\plugInfo.json" "%USD_PLUG_DST%"

cmake -B%BUILD_PATH% -S . -DCMAKE_TOOLCHAIN_FILE=%VCPKG_TOOLCHAIN% -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release
cmake --build %BUILD_PATH% --config Release

cd %CWD%
endlocal