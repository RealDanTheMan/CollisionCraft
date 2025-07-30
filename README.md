# 🇨​​​​​🇴​​​​​🇱​​​​​🇱​​​​​🇮​​​​​🇸​​​​​🇮​​​​​🇴​​​​​🇳​​​​​ 🇨​​​​​🇷​​​​​🇦​​​​​🇫​​​​​🇹​​​​​
<table class="no-border">
  <tr>
    <td><a href="https://github.com/RealDanTheMan/CollisionCraft/actions/workflows/test-build-macos.yml" alt="Arch Build"><img src="https://github.com/RealDanTheMan/CollisionCraft/actions/workflows/test-build-macos.yml/badge.svg" alt="CMake build"/></a></td>
    <td><a href="https://github.com/RealDanTheMan/CollisionCraft/actions/workflows/test-build-archlinux.yml" alt="MacOSX Build"><img src="https://github.com/RealDanTheMan/CollisionCraft/actions/workflows/test-build-archlinux.yml/badge.svg" alt="MS Build"/></a></td>
    <td><a href="https://github.com/RealDanTheMan/CollisionCraft/actions/workflows/test-build-windows.yml" alt="MacOSX Build"><img src="https://github.com/RealDanTheMan/CollisionCraft/actions/workflows/test-build-windows.yml/badge.svg" alt="MS Build"/></a></td>
   
  </tr>
  <tr>
    <td><img src="https://img.shields.io/badge/Solution-C++17-blue.svg?style=flat&logo=c%2B%2B&logoColor=b0c0c0&labelColor=363D44" alt="C++ solution"/></td>
    <td><img src="https://img.shields.io/badge/OS-Arch%20%7C%20MacOSX%20%7C%20Windows-blue??style=flat&logo=Linux&logoColor=b0c0c0&labelColor=363D44" alt="Operating systems"/></td>
    <td colspan="2"><img src="https://img.shields.io/badge/CPU-x64%20%7C%20Arm64%20%7C%20-blue?style=flat&logo=amd&logoColor=b0c0c0&labelColor=363D44" alt="CPU Architect"/></td>
  </tr>
</table>

---

Small front end application for generating collision meshes from 3D models.

<p align="center">
  <img src="demo/img_01.png" alt="App Preview Image" width="320">
  <img src="demo/img_02.png" alt="App Preview Image" width="320">
  <img src="demo/img_03.png" alt="App Preview Image" width="320">
  <img src="demo/img_04.png" alt="App Preview Image" width="320">
</p>


---

### General Requirements
Ensure your system includes the following:
- **Git** for repository cloning.
- **Compatible Compilers**: *GNU*, *LLVM* supporting **C++17** or newer.
- **CMake** (version 3.16+).
- **CGAL**
- **QT6**
  
---

### Build From Source

To compile the **Collision Craft** locally from latest source, follow these steps:

#### Installing Dependencies | Arch Linux
```bash
pacman -S cmake git qt6-base eigen tbb boost cgal

```

#### Installing Dependencies | MacOSX
```bash
brew install cmake git qt6 eigen tbb boost cgal 

```

#### Installing Dependencies | Windows
> [!NOTE]
> Ensure you have **VCPKG** installed and added to the *PATH* to manage dependencies. VCPKG packages sources and build them locally, this can take some time but it only needs to happen once.
```bash
git clone https://github.com/microsoft/vcpkg
.\vcpkg\bootstrap-vcpkg.bat
.\vcpkg\vcpkg.exe update
set PATH=%PATH%;%CD%\vcpkg

```

```powershell
vcpkg install boost:x64-windows cgal:x64-windows eigen3:x64-windows python3:x64-windows qtbase:x64-windows python3:x64-windows usd:x64-windows

```

#### Run Build Script | Linux & MacOS
```bash
git clone https://github.com/RealDanTheMan/CollisionCraft
cd CollisionCraft
./shell/build.sh
./shell/run.sh
```

#### Run Build Script | Windows

```bash
git clone https://github.com/RealDanTheMan/CollisionCraft
cd CollisionCraft
./shell/build-product.bat
./shell/run.bat
```

---


