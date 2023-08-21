
## CMake를 처음으로 배우는 경우 읽어볼만한 사이트
- [CMake 공식 튜토리얼](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)
- [CMake 할때 쪼오오금 도움 되는 문서](https://gist.github.com/luncliff/6e2d4eb7ca29a0afd5b592f72b80cb5c)

## CMAKE_SOURCE_DIR vs CMAKE_CURRENT_SOURCE_DIR
- 전자는 cmake를 실행한 최상위 CMakeLists.txt의 경로
- 후자는 add_subdirectory()로 들어간 하위 디렉토리의 경로

## upstream vs downstream
- 공식 튜토리얼을 읽다보면 upstream과 downstream이라는 표현이 자주 나온다
- 산업, 오픈소스 프로젝트 등 다양한 분야에서 사용되는 단어지만 일단 소프트웨어 기준으로 정리해보면 아래와 같다
```
'downstream' depends on 'upstream'
```
- 예를 들어, [cmake-packages](https://cmake.org/cmake/help/latest/manual/cmake-packages.7.html#creating-packages)를 보면 다음과 같은 내용이 나온다.
```
upstream에서 config파일을 제공하는 경우에만 find_package()에 CONFIG 옵션을 사용해야 한다
```
upstream은 어떤 프로젝트가 depend-on 하는 대상이므로 라이브러리일 것이다.  
그러므로 이 문장은 라이브러리에서 xxxConfig.cmake파일을 제공하는지 확인하라는 뜻으로 이해하면 된다.

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

## 라이브러리를 외부에서 사용할 수 있도록 export하는 법
### Step 1) 간단한 정적 라이브러리 프로젝트에서 시작하자
```
mylib/
├─ include/
│  ├─ mylib.h
├─ mylib.cpp
├─ CMakeLists.txt
```
```cmake
# mylib/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

project(mylib)

add_library(mylib STATIC mylib.cpp)
target_include_directories(mylib PUBLIC include)
```
### Step 2) 바이너리와 헤더를 원하는 장소에 설치할 수 있게 install 규칙을 추가하자
```cmake
cmake_minimum_required(VERSION 3.10)

project(mylib)

add_library(mylib STATIC mylib.cpp)
target_include_directories(mylib PUBLIC include)

# 사용자 입장에선 .lib 그리고 .h 파일만 있으면 된다.
install(TARGETS mylib DESTINATION lib)
install(FILES include/mylib.h DESTINATION include)
```
이제 빌드를 한 후 시험삼아 somewhere라는 폴더에 install해보자.
```
cmake -S . -B build
cmake --build build --config Debug
cmake --install build --config Debug --prefix ../somewhere
```
성공했다면 아래와 같이 somewhere 폴더가 생겼을 것이다
```
somewhere/
├─ lib/
│  ├─ mylib.lib
├─ include/
│  ├─ mylib.h
mylib/
├─ include/
│  ├─ mylib.h
├─ mylib.cpp
├─ CMakeLists.txt
```
### Step 3) Debug와 Release 버전의 바이너리를 모두 install할 수 있게 만들자
지금은 install할 때마다 lib/mylib.lib 파일을 덮어쓰기 때문에  
Debug로 빌드한 lib 파일과 Release로 빌드한 lib 파일을 동시에 가질 수가 없다.  
참고로 프로젝트의 빌드 타입과 라이브러리의 빌드 타입이 다르면 에러가 생기므로 어떻게든 이 문제를 해결해야 한다!  
이번엔 ```$<CONFIG>```라는 generator expression을 사용해 복사할 폴더를 빌드 타입에 따라 달라지게 만들 것이다.
```cmake
cmake_minimum_required(VERSION 3.10)

project(mylib)

add_library(mylib STATIC mylib.cpp)
target_include_directories(mylib PUBLIC include)

# $<CONFIG>는 빌드 타입에 따라 "Debug" 또는 "Release"의 값을 갖는다
install(TARGETS mylib DESTINATION lib/$<CONFIG>)
install(FILES include/mylib.h DESTINATION include)
```
이제 Debug와 Release로 각각 빌드 및 install을 수행하면 somewhere 폴더가 아래와 같이 생성된다.
```
somewhere/
├─ lib/
│  ├─ Debug/
│  │  ├─ mylib.lib <-- Debug로 빌드한 바이너리
│  ├─ Release/
│  │  ├─ mylib.lib <-- Release로 빌드한 바이너리
├─ include/
│  ├─ mylib.h
mylib/
├─ include/
│  ├─ mylib.h
├─ mylib.cpp
├─ CMakeLists.txt
```
### Step 4) ```install(EXPORT ...)```를 사용해 imported target을 만들자
```cmake
cmake_minimum_required(VERSION 3.10)

project(mylib)

add_library(mylib STATIC mylib.cpp)

# install(EXPORT ...)를 사용하려면 build와 install의 include directory 설정을 개별적으로 해줘야 한다.
# INSTALL_INTERFACE 뒤에 오는 상대 경로는 CMAKE_INSTALL_PREFIX를 기준으로 계산된다.
#
# 주의: install()과 마찬가지로 명시적으로 ${CMAKE_INSTALL_PREFIX}를 사용하면 cmake --install의 --prefix 옵션을 무시한다!
#       relocatable한 라이브러리를 만들기 위해서는 이런 절대경로를 사용하면 안된다.
#
# BUILD_INTERFACE에 상대 경로를 넣으면 에러가 떠서 ${CMAKE_SOURCE_DIR}을 사용했다.
# 원인은 아직 모르겠지만 지금까지 본 예시에선 다 이렇게 하더라.
#
# 참고:
# 1. BUILD_INTERFACE: 프로젝트를 빌드할 때, install(TARGETS ...)가 실행될 때 사용됨
# 2. INSTALL_INTERFACE: install(EXPORT ...)가 실행될 때 활성화됨
target_include_directories(mylib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

install(TARGETS mylib
    EXPORT mylibTargets # TARGETS로 지정된 모든 target을 포함하는 export set을 생성 (이름은 자유)
    DESTINATION lib/$<CONFIG>

    # mylib를 사용하는 외부 프로젝트의 target에 해당 경로를 include directory로 추가해준다
    # 이번 예시에선 mylib이 PUBLIC으로 target_include_directories()를 실행했으므로 생략해도 차이는 없다.
    # 만약 일부 헤더만 public header로 공개하려 한다면 이 방법이 필요할 것이다.
    INCLUDES DESTINATION include
)
install(FILES include/mylib.h DESTINATION include)

# mylibTargets.cmake라는 파일을 생성하고 lib/cmake/mylib 폴더에 복사한다.
# 거의 대부분 lib/cmake/[패키지 이름] 폴더에 이런 파일을 넣는다고 한다.
# 뒤에서 언급하겠지만, 이렇게 하면 find_package를 위해 CMAKE_PREFIX_PATH 설정이 깔끔하다.
install(EXPORT mylibTargets
    FILE mylibTargets.cmake # 생략하면 export target과 같은 이름으로 생성된다 (ex. mylibTargets.cmake)
    NAMESPACE mylib:: # NAMESPACE를 지정하면 모든 target 앞에 prefix를 붙일 수 있다 (필수 x)
    DESTINATION lib/cmake/mylib
)
```
Debug와 Release로 빌드 및 install을 해보면 somewhere/lib/cmake/mylib 폴더에 세 개의 파일이 생긴다
```
somewhere/
├─ lib/
│  ├─ Debug/
│  │  ├─ mylib.lib
│  ├─ Release/
│  │  ├─ mylib.lib
│  ├─ cmake
│  │  ├─ mylib/
│  │  │  ├─ mylibTargets-debug.cmake
│  │  │  ├─ mylibTargets-release.cmake
│  │  │  ├─ mylibTargets.cmake
├─ include/
│  ├─ mylib.h
mylib/
├─ include/
│  ├─ mylib.h
├─ mylib.cpp
├─ CMakeLists.txt

```
원래라면 mylib를 사용하기 위해서 빌드 타입에 따라 올바른 lib 파일을 선택하고 include 디렉토리를 추가하는 등  
번거로운 작업을 일일이 해줘야하는데 이 부분을 자동으로 생성한 CMakeLists.txt라고 생각하면 된다.  
실제로 somewhere/lib/cmake/mylib/mylibTargets.cmake를 열어보면 흔히 사용하는 ```add_library()```같은 내용이 들어있다.  
NAMESPACE 옵션의 영향으로 앞에 mylib::가 추가된 것도 확인할 수 있다.
```cmake
# Create imported target mylib::mylib
add_library(mylib::mylib STATIC IMPORTED)
```
이제 외부 프로젝트의 CMakeLists.txt에 딱 두 줄만 추가하면 mylib를 사용할 수 있다
```cmake
# test라는 프로젝트에서 mylib를 사용한다고 가정하자
add_executable(test main.cpp)

# 1. 아까 생성한 cmake파일을 #include처럼 복붙해서 mylib라는 imported target을 생성한다
include(엄청복잡한경로/somewhere/lib/cmake/mylib/mylibTargets.cmake)

# 2. 사용하려는 주체인 test에 링크해주면 끝!
# 아까 target_include_directories()에서 PUBLIC으로 지정했기 때문에
# 추가적인 설정 없이 mylib.h도 사용할 수 있다.
target_link_libraries(test PUBLIC mylib::mylib)
```
### Step 5) ```find_package()```를 사용할 수 있도록 config.cmake 파일을 생성하자
find_package()를 사용하면 xxxConfig.cmake 형식의 파일을 자동으로 찾을 수 있다.  
cmake에서 외부 라이브러리를 참조할 때 권장하는 방법이므로 우리도 따라해보자.  
Q) 그냥 mylibTargets의 이름을 mylibConfig로 바꾸면 되는거 아닌가?  
A) 이름이 Config.cmake로 끝나니 될 것 같기도 하지만... 일단 공식 튜토리얼은 다른 방법을 쓰라고 한다
```
# 전체적인 과정
1. config파일의 틀을 작성한다
2. cmake의 helper function으로 부족한 내용을 채운 mylibConfig.cmake를 만든다
3. 외부 프로젝트에서 find_package()를 사용해 mylibConfig.cmake파일을 찾는다
4. mylibConfig.cmake가 mylibTargets.cmake를 include()한다
```
조금 번거롭긴 하지만, 파일 존재 여부 등 안전 체크를 자동으로 해주는 등 몇가지 장점이 있다고 한다.  
일단 config파일의 틀이 될 mylibConfig.cmake.in이라는 파일을 새로 만들자
```cmake
# helper function이 채워넣을 부분.
# 필요한 파일과 컴포넌트가 존재하는지 체크하는 코드가 들어가는 것 같다.
@PACKAGE_INIT@

# find_package()는 이 파일을 include()하는 것과 같은 효과를 내기 때문에 CMAKE_CURRENT_LIST_DIR을 사용해야 한다.
# 참고:
# 1. CMAKE_CURRENT_SOURCE_DIR
# - 현재 처리중인 CMakeLists.txt의 경로.
# - include()한 파일의 경로를 따라가므로 find_package()를 하면 외부 프로젝트의 경로를 가리키게 된다.
# 2. CMAKE_CURRENT_LIST_DIR
# - 실제로 이 파일이 위치한 경로.
# - 이 파일이 include()되어도 같은 값을 유지한다.
#   지금은 find_package()가 include()하는 상황이므로 이 경로가 적합하다.
include(${CMAKE_CURRENT_LIST_DIR}/mylibTargets.cmake)
```
이제 CMakeLists.txt에서 해당 파일을 사용해 config 파일을 생성하도록 해야한다.
```cmake
cmake_minimum_required(VERSION 3.10)

project(mylib)

add_library(mylib STATIC mylib.cpp)
target_include_directories(mylib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

install(TARGETS mylib
    EXPORT mylibTargets
    DESTINATION lib/$<CONFIG>
    INCLUDES DESTINATION include
)
install(FILES include/mylib.h DESTINATION include)

install(EXPORT mylibTargets
    FILE mylibTargets.cmake
    NAMESPACE mylib::
    DESTINATION lib/cmake/mylib
)

# mylibConfig.cmake.in 파일을 읽고 필요한 내용을 채워서
# 빌드 디렉토리의 mylibConfig.cmake 파일을 생성한다
include(CMakePackageConfigHelpers)
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/mylibConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/mylibConfig.cmake
    INSTALL_DESTINATION lib/cmake/mylib
)

# 생성된 mylibconfig.cmake 파일을 install 경로에 복사한다
# 위에서 사용한 INSTALL_DESTINATION과 같은 경로를 사용해야 문제가 안 생긴다고 한다
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/mylibConfig.cmake
    DESTINATION lib/cmake/mylib
)
```
install을 마치면 최종적으로 아래와 같은 구조를 갖게 된다
```
somewhere/ <-- CMAKE_PREFIX_PATH에 추가해야 하는 경로
├─ lib/
│  ├─ Debug/
│  │  ├─ mylib.lib <-- Debug로 빌드한 바이너리
│  ├─ Release/
│  │  ├─ mylib.lib <-- Release로 빌드한 바이너리
│  ├─ cmake/
│  │  ├─ mylib/
│  │  │  ├─ mylibTargets-debug.cmake <-- Debug로 빌드할 때 lib/Debug/mylib.lib을 사용하도록 설정함
│  │  │  ├─ mylibTargets-release.cmake <-- Release로 빌드할 때 lib/Release/mylib.lib을 사용하도록 설정함
│  │  │  ├─ mylibTargets.cmake <-- add_library()등 실질적인 세팅을 하는 파일
│  │  │  ├─ mylibConfig.cmake <-- find_package()가 탐색하는 파일
├─ include/
│  ├─ mylib.h <-- Debug, Release 상관 없이 필요한 public header
mylib/
├─ include/
│  ├─ mylib.h
├─ mylib.cpp
├─ mylibConfig.cmake.in <-- mylibConfig.cmake를 만들기 위한 틀
├─ CMakeLists.txt
```
외부 프로젝트에서 mylib을 사용하려면 다음과 같이 하면 된다.
```cmake
cmake_minimum_required(VERSION 3.10)

project(myexec)

# cmake에서 정해둔 기본 경로에 있는 경우 find_package()만 해도 되지만
# 우리처럼 custom 경로를 사용하려면 CMAKE_INSTALL_PREFIX에 추가해줘야한다.
#
# somewhere 폴더를 넘겨주면 자동으로 somewhere/lib/cmake/mylib 폴더에서
# mylibConfig.cmake 또는 mylib-config.cmake가 존재하는지 확인한다.
#
# somewhere/lib/cmake/mylib까지 넘겨줘도 되지만 귀찮으니 그러지 말자
list(APPEND CMAKE_PREFIX_PATH 엄청복잡한경로/somewhere)

# config 모드로 find_package()를 호출하면 xxxConfig.cmake 형식의 파일을 탐색한다.
# 자매품으로 module 모드도 있다.
# 참고)
# 1. REQUIRED: 못 찾으면 에러 발생
# 2. QUIET: 못 찾아도 에러 없이 진행
find_package(mylib CONFIG REQUIRED)

add_executable(myexec main.cpp)
target_link_libraries(myexec mylib::mylib)
```
이렇게 완성한 install 폴더는 절대경로를 사용하지 않고 relocatable하기 때문에  
somewhere 폴더를 이리저리 옮겨도 CMAKE_PREFIX_PATH만 잘 지정하면 링크에 성공한다!  
만약 캐시 변수를 사용하거나 절대 경로를 넣었다면 불가능했을 것이다.
### 참고자료
- [CMake 공식 export 튜토리얼](https://cmake.org/cmake/help/latest/guide/importing-exporting/index.html)
- [CMake documentation - xxxConfig.cmake를 놓는 일반적인 위치에 대한 내용이 담긴 페이지](https://cmake.org/cmake/help/v3.22/guide/using-dependencies/index.html)
- [CMake documentation - CMakePackageConfigHelpers](https://cmake.org/cmake/help/v3.0/module/CMakePackageConfigHelpers.html)
