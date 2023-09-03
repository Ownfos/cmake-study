# CMake Project Example
## This example was intended to show the following things
1. Directory structure for a CMake project
2. Using vcpkg in manifest mode to manage external library dependencies
3. Using GoogleTest to make use of unit tests
## How to build
### 1. Clone vcpkg
```
git clone https://github.com/microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg.bat
```
### 2. Build and run tests
#### 2-1) vscode cmake tools extension
- Execute "CMake: Run Tests" from command palette.  
This will configure, build, and run tests sequentially.
#### 2-2) command line (requires CMake 3.20 or higher)
```
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE="vcpkg/scripts/buildsystems/vcpkg.cmake"
cmake --build build
ctest -C Debug --test-dir build
```