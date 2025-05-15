# 🇨​​​​​🇴​​​​​🇱​​​​​🇱​​​​​🇮​​​​​🇸​​​​​🇮​​​​​🇴​​​​​🇳​​​​​ 🇨​​​​​🇷​​​​​🇦​​​​​🇫​​​​​🇹​​​​​
<table class="no-border">
  <tr>
    <td><a href="https://github.com/RealDanTheMan/CollisionCraft/actions/workflows/cmake-macos-build.yml" alt="Arch Build"><img src="https://github.com/RealDanTheMan/CollisionCraft/actions/workflows/cmake-archlinux-build.yml/badge.svg" alt="CMake build"/></a></td>
    <td><a href="https://github.com/RealDanTheMan/CollisionCraft/actions/workflows/cmake-archlinux-build.yml" alt="MacOSX Build"><img src="https://github.com/RealDanTheMan/CollisionCraft/actions/workflows/cmake-macos-build.yml/badge.svg" alt="MS Build"/></a></td>
    <td></td>
  </tr>
  <tr>
    <td><img src="https://img.shields.io/badge/Solution-C++17-blue.svg?style=flat&logo=c%2B%2B&logoColor=b0c0c0&labelColor=363D44" alt="C++ solution"/></td>
    <td><img src="https://img.shields.io/badge/OS-Arch%20%7C%20MacOSX-blue??style=flat&logo=Linux&logoColor=b0c0c0&labelColor=363D44" alt="Operating systems"/></td>
    <td colspan="2"><img src="https://img.shields.io/badge/CPU-x86%20%7C%20x86__64%20%7C%20-blue?style=flat&logo=amd&logoColor=b0c0c0&labelColor=363D44" alt="CPU Architect"/></td>
  </tr>
</table>

---

Small front end application for generating collision meshes from 3D models.

<p align="center">
  <img src="demo/img_01.png" alt="App Preview Image" width="320">
  <img src="demo/img_02.png" alt="App Preview Image" width="320">
  <img src="demo/img_03.png" alt="App Preview Image" width="320">
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

#### Run Build Script
```bash
git clone https://github.com/RealDanTheMan/CollisionCraft
cd CollisionCraft
./shell/build.sh
./shell/run.sh
```

---


