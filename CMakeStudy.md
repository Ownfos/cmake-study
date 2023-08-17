
## CMake를 처음으로 배우는 경우 읽어볼만한 사이트
- [CMake 공식 튜토리얼](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)
- [CMake 할때 쪼오오금 도움 되는 문서](https://gist.github.com/luncliff/6e2d4eb7ca29a0afd5b592f72b80cb5c)

## CMAKE_SOURCE_DIR vs CMAKE_CURRENT_SOURCE_DIR
- 전자는 cmake를 실행한 최상위 CMakeLists.txt의 경로
- 후자는 add_subdirectory()로 들어간 하위 디렉토리의 경로

## add_subdirectory() vs include()
- 전자는 서브루틴처럼 하위 디렉토리로 들어가서 이뤄짐
- 후자는 #include처럼 현재 처리중인 CMakeLists.txt에 끼어듦
- include()는 현재 CMakeLists.txt에서 선언된 변수를 읽고 수정할 수 있음

## Debug또는 Release로 빌드하기
#### 1. visual studio를 비롯한 multi-configuration generator를 사용하는 경우
```
cmake -S . -B build
cmake --build build --config Release
cmake --build build --config Debug
```
- 한 번 configure 해놓으면 Release와 Debug 모두 빌드 가능
#### 2. makefile을 비롯한 single configuration generator를 사용하는 경우
```
# Release를 위한 configure 및 빌드
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Debug를 위한 configure 및 빌드
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```
- 빌드 타입을 바꾸고 싶으면 위 예시처럼 configure부터 해야 함

## install()과 CMAKE_INSTALL_PREFIX
- install()은 빌드된 target(.lib, .exe, .so 등)을 지정된 폴더로 복사하게 만드는 설정
- 상대 경로로 지정하면 CMAKE_INSTALL_PREFIX를 기준으로 삼음
- CMAKE_INSTALL_PREFIX의 기본 값은 OS에 따라 다르다  
유닉스: /user/local  
윈도우: c:/Program Files/${PROJECT_NAME}
### install하는 방법 (빌드 시스템으로 visual studio를 사용하는 경우)
#### 1. cmake --build에서 옵션 넘겨주기
```
cmake -S . -B build
cmake --build build --target install
```
#### 2. cmake --install을 따로 실행
```
cmake -S . -B build
cmake --build build
cmake --install build
```
### CMAKE_INSTALL_PREFIX에 원하는 값 넘기는 방법
#### 1. configure 단계에서 옵션 넘겨주기
```
cmake -S . -B build -DCMAKE_INSTALL_PREFIX=c:/asdf/haha
```
- CMakeCache.txt를 열어보면 'CMAKE_INSTALL_PREFIX:PATH=xxx'처럼 저장된 것을 확인할 수 있다
- 경로를 바꾸고 싶으면 configure 및 build를 다시 해야한다...
#### 2. cmake --install에 옵션 넘겨주기
```
cmake -S . -B build
cmake --build build --config Release
cmake --install build --config Release --prefix c:/asdf/haha
```
- cmake --install은 자동으로 빌드를 해주는 기능이 없어서 꼭 빌드가 끝난 뒤에 실행해야 한다.
- build와 install의 디폴트 빌드 타입은 빌드 시스템에 따라 달라지는 것 같다.  
window + visual studio: build는 debug, install은 release  
ubuntu + makefile: build는 release, install은 build 따라감 (single configuration generator)
- 경로를 바꾸고 싶으면 --prefix부분만 바꿔서 cmake --install을 해주면 된다 (다시 빌드할 필요 x)
### ```${CMAKE_INSTALL_PREFIX}```의 값은 --prefix의 영향을 받지 않는다
cmake --install에 --prefix 옵션을 다르게 넘겨도 install()안에 적힌 상대경로만 영향을 줄 뿐  
명시적으로 ```${CMAKE_INSTALL_PREFIX}``` 변수를 사용하는 곳에는 아무 영향도 없다.  
이 변수는 configure 시점에 정해진 캐시 값을 참조하는 것으로 알고있다.  
어차피 prefix 기준으로 install 위치를 정해야한다면 편하고 안전한 상대경로를 사용하자.
### install()을 사용하는 CMakeLists.txt 예시
- cmake --build build --target install 하면 install()로 설정한 target이 지정된 경로로 복사됨
```cmake
# implicit linking으로 만든 동적 라이브러리 -> mylib.lib, mylib.dll을 생성
add_library(mylib SHARED mylib.cpp)

# mylib를 사용하는 메인 프로젝트 -> test.exe를 생성
add_executable(test main.cpp)
target_link_libraries(test PUBLIC mylib)

# test.exe, mylib.lib, mylib.dll 세 개의 파일이
# ${CMAKE_INSTALL_PREFIX}/haha 폴더에 복사된다
install(TARGETS test mylib DESTINATION haha)
```
### CMAKE_INSTALL_PREFIX에 상대 경로도 넣어줄 수 있다
```
# ~/myproject에서 아래 명령을 실행하면 ~/myproject/asdf가 CMAKE_INSTALL_PREFIX로 설정된다
cmake -S . -B build -DCMAKE_INSTALL_PREFIX=asdf
```

## Linux에서 .so파일이 같은 경로에 있어도 찾지 못하는 이유와 해결법
- 바이너리 파일에 모든 코드가 포함되는 정적 라이브러리와 다르게 동적 라이브러리는 런타임에 .dll이나 .so파일을 찾아야 한다.
- 환경마다 다르겠지만 기본적으론 바이너리 파일이 있는 폴더나 널리 알려진 시스템 폴더를 탐색할 것이다.  
실제로 윈도우에선 같은 경로에 .dll파일을 넣어주기만 해도 잘 돌아간다.
- Linux에서도 build 폴더 안에 있는 바이너리 파일을 실행하면 정상적으로 링크가 되긴 한다.  
하지만 ```install()```로 생성된 바이너리 파일을 실행하면 .so 파일이 같은 경로에 있어도 다음과 같은 에러가 발생한다.
```
error while loading shared libraries: [라이브러리 이름].so: cannot open shared object file: No such file or directory
```
- 동적 라이브러리를 찾고 연결해주는 역할을 ld.so와 ld-linux.so라는 프로그램이 해준다는데  
이 친구들은 바이너리 파일이 있는 경로를 먼저 찾는게 아니라 다음과 같은 순서로 폴더를 탐색한다  
1. 바이너리 파일의 DT_RPATH section에 정의된 경로들
2. 환경변수 LD_LIBRARY_PATH에 정의된 경로들
3. DT_RUNPATH에 정의된 경로들
4. ldconfig 캐시 파일에 정의된 경로들 (보통 /etc/ld.so.cache에 있다고 함)
- 여기서 우리가 건드려야 하는건 환경변수나 캐시가 아니라 바이너리 파일에 들어갈 RPATH다.  
CMake에서는 ```CMAKE_INSTALL_RPATH```로 install한 파일의 rpath를 설정할 수 있다.  
해당 변수에 값을 넣으면 ```INSTALL_RPATH```가 설정되고 이게 target의 rpath가 되는 것이다.  
왜 ```INSTALL_RPATH```를 직접 변경하지 않고 ```CMAKE_INSTALL_RPATH```를 사용하는지는 모르겠다...
- CMake는 일단 RPATH에 build 폴더를 넣고 ```install()```로 복사할 때 RPATH만 교체하는 것 같다.  
따로 설정하지 않으면 ```INSTALL_RPATH```는 기본적으로 비어있기 때문에 ```install()```한 바이너리 파일만 .so를 찾지 못했을 것이다.
- 여러 경로를 사용하고 싶으면 세미콜론으로 구분하면 된다고 한다.
- 바이너리 파일이 있는 폴더를 의미하는 값은 ```$ORIGIN```이다.  
아래 예시처럼 CMakeLists.txt를 작성해주면 같은 경로에 있는 .so 파일을 제대로 찾아낸다.
```cmake
add_library(mylib SHARED mylib.cpp)

# 반드시 add_executable()이 나오기 전에 적어줘야 한다!
# 위에 선언된 mylib는 RPATH 설정의 영향을 받지 않는다.
# 만약 라이브러리를 다른 경로로 install 해야한다면 $ORIGIN 자리에 해당 경로를 넣어주면 된다.
set(CMAKE_INSTALL_RPATH $ORIGIN)
add_executable(test main.cpp)

target_link_libraries(test PUBLIC mylib)
```
- ```$ORIGIN```대신 ```${CMAKE_INSTALL_PREFIX}```를 사용하는 경우 주의해야 할 상황이 하나 있다
```
# 문제가 발생할 수 있는 예시
set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/../lib)

add_library(mylib SHARED mylib.cpp)
add_executable(test main.cpp)
target_link_libraries(test PUBLIC mylib)

install(TARGETS test mylib
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)

# 터미널에서 아래와 같은 명령을 실행하면 mylib.so는 ~/BBB/lib에 생기는 반면
# RPATH는 ~/AAA/lib으로 설정되어 라이브러리를 찾지 못하게 된다.
# CMakeLists.txt에 사용된 ${CMAKE_INSTALL_PREFIX}에는
# --prefix 옵션과 무관하게 configure 시점의 값이 들어가기 때문이다...!
cmake -S . -B build -DCMAKE_INSTALL_PREFIX=~/AAA
cmake --build build
cmake --install build --prefix ~/BBB
```
##### 해결법 1
- 첫 번째 줄을 set(CMAKE_INSTALL_RPATH $ORIGIN/../lib)로 바꾼다
- $ORIGIN은 항상 바이너리 파일의 경로를 가리키므로 --prefix 옵션에도 제대로 반응한다
##### 해결법 2
- ${CMAKE_INSTALL_PREFIX}를 사용하되 cmake --install에 --prefix를 사용하지 않는다
- CMAKE_INSTALL_PREFIX는 configure시점에 -DCMAKE_INSTALL_PREFIX 옵션으로 넘기기로 합의한 것으로 생각할 수 있다
- 이렇게 해도 돌아가긴 하는데 $ORIGIN을 사용하는 쪽이 더 안전하다고 생각한다

#### 참고자료
- [wikipedia - rpath](https://en.wikipedia.org/wiki/Rpath)
- [stackoverflow - linux executable cant find shared library in same folder](https://stackoverflow.com/questions/39978762/linux-executable-cant-find-shared-library-in-same-folder)
- [stackoverflow - how to set rpath origin in cmake](https://stackoverflow.com/questions/58360502/how-to-set-rpath-origin-in-cmake)

## CMake에서 폴더 = target으로 생각하면 이해하기 쉽다
```cmake
# 단일 프로젝트 구조와 CMakeLists.txt 예시
proj1/
├─ include/
│  ├─ Dog.h
├─ main.cpp
├─ CMakeLists.txt

# proj1/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

project(proj1)

add_executable(proj1 main.cpp)
target_include_directories(proj1 PRIVATE ${CMAKE_SOURCE_DIR}/include)
```
```cmake
# 라이브러리를 사용하는 프로젝트 구조와 CMakeLists.txt 예시
# proj1과 비교했을 때 mylib폴더가 추가되었다는 것만 다르다
# mylib를 사용하는 다른 프로젝트가 생기면 폴더 채로 복붙하고
# add_subdirectory(), target_link_library() 두 줄만 적어주면 끝!
proj2/
├─ mylib/
│  ├─ include/
│  │  ├─ Duck.h
│  ├─ mylib.cpp
│  ├─ CMakeLists.txt
├─ include/
│  ├─ Cat.h
├─ main.cpp
├─ CMakeLists.txt

# proj2/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

project(proj2)

add_subdirectory(mylib)

add_executable(proj2 main.cpp)
target_include_directories(proj2 PRIVATE ${CMAKE_SOURCE_DIR}/include)

# proj2/mylib/CMakeLists.txt

# 여기서 project(mylib)을 작성한 뒤 cmake -S . -B build 명령을 실행하면
# visual studio 기준으로 build/mylib/mylib.sln이 추가로 생긴다
# project()없이 이렇게만 하면 mylib.vcxproj만 생김!
# 정확히 어떻게 다른진 모르겠지만 결과물은 똑같이 나오더라
add_library(mylib mylib.cpp)
target_include_directories(mylib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
```

## Usage Requirement (PUBLIC, INTERFACE, PRIVATE)의 효과
### 관련 함수
- target_link_libraries()
- target_include_directories()
### 용어 정리
- usage requirement: PUBLIC, INTERFACE, PRIVATE으로 지정하는 visibility (OOP의 access specifier랑 비슷함)
- producer: 라이브러리
- consumer: 라이브러리를 사용하는 프로젝트 (executable 또는 상위 라이브러리)
### 키워드별 효과
#### PUBLIC
- producer와 consumer 모두가 필요로 하는 경우 사용
- consumer에서 target_link_libraries()만 해주면 producer에서 사용한 library와 include directory가 자동으로 링크됨
```cmake
add_library(lib lib.cpp)
target_include_directories(lib PUBLIC ${CMAKE_SOURCE_DIR}/lib/include)

# lib에서 PUBLIC으로 include directory를 지정했기 때문에
# 추가적인 target_include_directories() 없이도
# test에서 lib/include에 접근할 수 있다
add_executable(test)
target_include_libraries(test PUBLIC lib)
```
#### INTERFACE
- producer는 필요 없지만 consumer는 필요로 하는 경우 사용
- consumer에게 미치는 영향은 PUBLIC과 동일
#### PRIVATE
- producer는 필요하지만 consumer에게는 숨기고 싶은 경우 사용
- consumer는 producer에서 PRIVATE으로 지정한 라이브러리와 include directory를 알 수 없음