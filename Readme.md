
## CMake를 처음으로 배우는 경우 읽어볼만한 사이트
- [CMake 공식 튜토리얼](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)
- [CMake 할때 쪼오오금 도움 되는 문서](https://gist.github.com/luncliff/6e2d4eb7ca29a0afd5b592f72b80cb5c)

## CMAKE_SOURCE_DIR vs CMAKE_CURRENT_SOURCE_DIR
- 전자는 cmake를 실행한 최상위 CMakeLists.txt의 경로
- 후자는 add_subdirectory()로 들어간 하위 디렉토리의 경로

## CMAKE_BINARY_DIR vs CMAKE_CURRENT_BINARY_DIR
- 하나의 cmake 프로젝트 디렉토리에 여러 CMakeLists.txt가 있어서 CMAKE_CURRENT_SOURCE_DIR이 필요해진 것처럼  
add_subdirectory()를 할 때마다 빌드 디렉토리에서 하위 폴더가 생기기 때문에 이들을 구분할 방법이 있어야 한다.
```cmake
# 폴더 구조
aaa/
├─ bbb/
│  ├─ CMakeLists.txt
│  ├─ bbb.cpp
│  ├─ ccc/
│  │  ├─ CMakeLists.txt
│  │  ├─ ccc.cpp
├─ CMakeLists.txt
├─ aaa.cpp

# aaa/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(test)
add_subdirectory(bbb)
add_library(aaa aaa.cpp)

# aaa/bbb/CmakeLists.txt
add_subdirectory(ccc)
add_library(bbb bbb.cpp)

# aaa/bbb/ccc/CmakeLists.txt
add_library(ccc ccc.cpp)

# 빌드 디렉토리 (Debug로 빌드한 상태의 핵심 내용만 묘사함)
build/ <-- aaa/CMakeLists.txt의 CMAKE_CURRENT_BINARY_DIR
├─ Debug/
│  ├─ aaa.lib
├─ bbb/ <-- aaa/bbb/CMakeLists.txt의 CMAKE_CURRENT_BINARY_DIR
│  ├─ Debug/
│  │  ├─ bbb.lib
│  ├─ ccc/ <-- aaa/bbb/ccc/CmakeLists.txt의 CMAKE_CURRENT_BINARY_DIR
│  │  ├─ Debug/
│  │  │  ├─ ccc.lib
```

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
#### 가능한 빌드 타입으로는 Debug, Release, RelWithDebInfo, MinSizeRel 네 가지가 있다
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
앞으로 일어날 일을 요약해보면 다음과 같다
1. install() 함수로 .lib과 .h파일들을 외부에 독립적인 폴더 구조로 복사할 수 있게 설정한다.
2. install(TARGETS ... EXPORT ...)로 xxxTargets라는 export set을 만든다.
3. install(EXPORT ...)로 export set을 불러오는 코드를 담은 xxxTargets.cmake 파일을 만든다
4. 3번 단계에서 만든 .cmake파일을 찾아 불러오고 부가적인 안전 체크를 해주는 xxxConfig.cmake 파일을 만든다.
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
#### 다른 방법: ```<CONFIG>_PREFIX``` 프로퍼티 설정하기
```cmake
# <CONFIG>_PREFIX라는 프로퍼티를 설정하면 결과물의 파일 이름에 지정한 prefix를 붙일 수 있다.
# fmt라는 유명한 라이브러리에서도 이 방법을 사용해 Release와 Debug 바이너리를 이름으로 구분한다.
# ex) fmt.lib <-- Release
#     fmtd.lib <-- Debug
# 
# 이렇게 하면 위에서처럼 $<CONFIG>를 사용해 다른 폴더에 바이너리를 넣도록 할 필요가 없어진다.
# 과정이 어떻든 Release, Debug 등 다른 설정으로 빌드한 바이너리가 덮어씌워지는 상황만 막으면 되기 때문!
set_target_property(mylib PROPERTIES DEBUG_PREFIX d)
```
### Step 4) ```install(EXPORT ...)```를 사용해 imported target을 만들자
```cmake
cmake_minimum_required(VERSION 3.10)

project(mylib)

add_library(mylib STATIC mylib.cpp)

# install(EXPORT ...)를 사용하려면 build와 install의 include directory 설정을 개별적으로 해줘야 한다.
# 자세한 이유는 조금 길어져서 밑에 문단을 하나 더 만들 예정...
#
# 주의: install()과 마찬가지로 명시적으로 ${CMAKE_INSTALL_PREFIX}를 사용하면 cmake --install의 --prefix 옵션을 무시한다!
#       relocatable한 라이브러리를 만들기 위해서는 이런 절대경로를 사용하면 안된다.
#
# 참고:
# 1. BUILD_INTERFACE: 프로젝트를 빌드할 때, install(TARGETS ...)가 실행될 때 사용됨. 절대 경로가 들어가야 한다.
# 2. INSTALL_INTERFACE: install(EXPORT ...)가 실행될 때 활성화됨. 절대 경로는 넣지 말자.
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
#### 외전: target_include_directories()에서 build interface와 install interface를 각각 설정해야 하는 이유
```cmake
# 실제로 파일을 빌드하려면 언젠간 절대경로가 필요해진다.
# 그래서 target_include_directories()는 상대경로를 넘겨주면 call site에서 바로 절대경로로 변환해버린다.
# 그러므로 아래 두 줄은 결과적으로 같아진다
target_include_directories(test PUBLIC include)
target_include_directories(test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 진짜 같은지 궁금하면 아래 코드를 추가해 설정된 값을 확인해보길 바란다.
get_target_property(dir test INTERFACE_INCLUDE_DIRECTORIES)
message(${dir})

# 그런데 절대 경로를 사용해버리면 relocatability에 문제가 생긴다.
# 위에서 지정한 include directory는 무조건 현재 빌드 트리를 가리키기때문에
# install로 헤더를 복사해봤자 사용할 수가 없다.
# 그렇기에 install(EXPORT ...)로 생성한 타겟에는 해당 위치의 헤더를 사용하라고 지시할 방법이 필요한 것이다.
# 그렇게 나온 해법이 BUILD_INTERFACE와 INSTALL_INTERFACE라는 generator expression을 사용하는 것!
target_include_directories(test PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# 실제로 install(EXPORT ...)로 생성된 .cmake 파일을 열어보면 아래와 같은 코드를 찾을 수 있다.
# 물론 여기에도 install prefix를 직접 박아버리면 relocatability를 잃어버리므로
# 이 .cmake 파일을 include()할 upstream 프로젝트에서 처리할 수 있게 ${_IMPORT_PREFIX}라는 변수를 활용한다.
add_library(test STATIC IMPORTED)
set_target_properties(test PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/include"
)

# 근데 왜 갑자기 BUILD_INTERFACE에 없던 ${CMAKE_CURRENT_SOURCE_DIR}을 추가해 절대 경로를 넘겨줬을까?
# CMake 개발자인 Brad King에 의하면 generator expression을 사용하는 순간
# 결과물이 절대 경로로 나올지, 상대 경로로 나올지 알 수 없어서 일단 절대 경로로 간주한다고 한다.
# 즉, 자동으로 prefix를 붙여주지 않기 때문에 우리가 직접 절대 경로를 넘겨줘야 하는 것이다.
# 어찌되었건 빌드할 때 필요한 건 절대 경로니까!
# 아래처럼 사용하면 include 경로에 상대 경로를 줬다며 에러가 발생한다.
target_include_directories(test PUBLIC
    $<BUILD_INTERFACE:include>
    $<INSTALL_INTERFACE:include>
)
```
- [Brad King이 관련 issue에 남긴 댓글](https://gitlab.kitware.com/cmake/cmake/-/issues/19802)

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
버전 정보를 담은 ```<PackageName>ConfigVersion.cmake```파일도 만들어주면 좋지만 없어도 링크는 가능하다.
```cmake
write_basic_package_version_file(mylibConfigVersion
  VERSION 1.0.0
  COMPATIBILITY AnyNewerVersion
)
```
### 참고자료
- [CMake 공식 export 튜토리얼](https://cmake.org/cmake/help/latest/guide/importing-exporting/index.html)
- [CMake documentation - xxxConfig.cmake를 놓는 일반적인 위치에 대한 내용이 담긴 페이지](https://cmake.org/cmake/help/v3.22/guide/using-dependencies/index.html)
- [CMake documentation - CMakePackageConfigHelpers](https://cmake.org/cmake/help/v3.0/module/CMakePackageConfigHelpers.html)


## vscode에서 cmake tools extension 사용하기
### 기본적인 정보
- 프로젝트 폴더에서 ctrl+p로 커맨드 팔레트를 열면 CMake: Configure라는 명령이 보인다.  
  만약 오른쪽 아래에 뜨는 팝업 메시지를 놓쳤다면 이렇게 수동으로 configure를 해줄 수 있다.  
  한 번 configure 해주면 cmakelists.txt가 수정될때마다 자동으로 configure를 다시 해준다!
- 화면 하단 바에서 빌드 타입, 컴파일러, 빌드 버튼, 디버그 버튼, 실행 버튼을 찾을 수 있다.  
  굳이 명령어를 치지 않아도 이 버튼들을 통해 같은 작업을 할 수 있어서 매우 편리하다.  
  단, install에 --prefix옵션을 넘기는건 없는 듯 하니 터미널 켜서 직접 해주자.
### 디버그를 시작했는데 std::cout 등 출력이 아무 곳에도 보이지 않는 문제 해결
- 일반 실행과 다르게 디버그는 설정이 다르게 되어있는지 .vscode/settings.json을 수정해야했다.
- 파일 안에 아래와 같은 내용을 끼워넣으면 terminal 탭에 정상적으로 출력된다.
```json
"cmake.debugConfig": {
    "console": "integratedTerminal"
}
```

## vscode에서 vcpkg 사용하기
#### 1. 일단 cmake 프로젝트 폴더에서 vcpkg를 clone한다
```
git clone https://github.com/microsoft/vcpkg
.\vcpkg\bootstrap-vcpkg.bat
```
#### 2. .vscode/settings.json에 아래와 같은 내용을 끼워넣는다.
```json
"cmake.configureArgs": ["-DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake"]
```
- vcpkg를 프로젝트 밑에 독립적으로 설치했기 때문에 vcpkg가 사용하는 라이브러리설치 경로를 알려줄 필요가 있다.
#### 3. 필요한 라이브러리를 설치한다.
- 두 가지 방법이 있음:  
  a) classic mode - 직접 설치  
  b) manifest mode - 의존성만 명시, 설치는 vcpkg가 해줌
#### 3-1. [classic mode] vcpkg install ... 명령을 직접 실행한다.
- 설치할 때 --triplet 옵션을 눈여겨봐야한다.  
  x64-windows, x86-windows 등 다양한 값이 가능한데 이게 빌드 시스템이랑 일치해야만 find_package()가 성공한다.
- 설치가 끝나면 어떻게 사용하는지 친절하게 알려준다.  
  그대로 복사해서 우리 프로젝트의 CMakeLists.txt에 붙여넣으면 된다.
- 설치 완료 후 나오는 메시지 예시 (fmt)
```
The package fmt provides CMake targets:

    find_package(fmt CONFIG REQUIRED)
    target_link_libraries(main PRIVATE fmt::fmt)

    # Or use the header-only version
    find_package(fmt CONFIG REQUIRED)
    target_link_libraries(main PRIVATE fmt::fmt-header-only)
```
#### 3-2. [manifest mode] 루트 디렉토리에 vcpkg.json 파일을 만든다.
- 마이크로소프트에서 권장하는 사용 방식이다.
- json 파일 안에 필요한 라이브러리의 이름과 버전을 적어두면 configure 시점에 자동으로 install을 해준다!
```json
{
    "dependencies": [
        "gtest",
        "fmt"
    ]
}
```

## vscode에서 Google Test로 unit test 사용하기
#### 1. Google Test 가져오기
- vcpkg 등으로 직접 다운로드해도 되고 gtest 튜토리얼에 나온것처럼 fetch_content를 해도 된다.
```cmake
# 출처: http://google.github.io/googletest/quickstart-cmake.html
# URL 중 archive/XXX.zip에서 XXX는 깃허브 리포지토리의 commit hash이며 최대한 자주 업데이트해달라고 나와있다.
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
```
#### 2. [!!!매우 중요!!!] '최상위' CMakeLists.txt에서 enable_test() 하기
- add_subdirectory()로 들어간 곳에서 해도 영향이 없으니 꼭 루트에 있는 CMakeLists.txt에 넣어줘야한다!!!
```cmake
cmake_minimum_required(VERSION 3.12)

# 설정 등등

include(CTest) # <-- DartConfigure.tlc 없다며 ctest가 제대로 작동하지 않을 경우 추가
enable_testing()
add_subdirectory(test)
```
- 참고자료: [stackoverflow - ctest add tests in subdirectories](https://stackoverflow.com/questions/54550939/ctest-add-tests-in-subdirectories)
#### 3. 테스트 코드 담은 executable 만들고 gtest_discover_tests()로 테스트 등록
- gtest를 사용해서 유닛 테스트를 이것저것 담은 test.cpp를 만들었다고 가정하자.  
  하나의 파일에 여러 TEST()가 있어도 되고, 여러 개의 테스트용 executable을 만들어도 된다.
```c++
#include <gtest/gtest.h>

TEST(TestSuiteName, Test1)
{
    ASSERT_EQ(7 * 6, 42);
}

TEST(TestSuiteName, Test2)
{
    ASSERT_TRUE(true);
}
```
- 이제 CMakeLists.txt에서 test.cpp에 담긴 TEST()를 읽고 테스트로 등록하도록 만들어보자.  
  테스트 executable이 여러 개라면 같은 작업을 반복해주면 된다.
```cmake
add_executable(mytest test.cpp)

# GTest 찾아서 링크
find_package(GTest CONFIG REQUIRED)
target_link_libraries(mytest PRIVATE GTest::gtest_main) # <-- 사용한 GTest 기능에 따라 GTest::gmock처럼 다른 라이브러리가 필요할 수도 있음

# 테스트 등록
include(GoogleTest)
gtest_discover_tests(mytest)
```
#### 4. vscode에서 테스트 돌리고 결과 확인하기 
- 커맨드 팔레드에서 CMake: Run Tests를 실행하거나 vscode 왼쪽 Testing 탭에서 UI로 테스트 실행 버튼을 눌러보자
- 아래에 TERMINAL 탭 옆에 TEST RESULTS 탭이 생기며 실행한 테스트를 시간대별로 확인할 수 있다.
- 왼쪽 Testing 탭에서는 마지막 테스트 결과를 빨간 체크와 초록 체크로 시각적으로 확인할 수 있고  
  여러 테스트 중 하나만 골라서 실행하는 것도 가능하다.

## 유닛테스트 target을 위한 꼼수: main.cpp를 제외한 모든 코드를 모아서 라이브러리로 만들기
#### 문제 상황
- .cpp 파일을 백 개 정도 사용하는 calculator 프로젝트를 만들었다고 가정하자.
- 당신은 유닛 테스트를 추가하려고 ```add_executable(test```를 타이핑하는 순간  
  그 뒤에 add_executable(calculator ...)에 들어간 모든 cpp 파일을 다시 적어줘야 한다는 것을 깨닫는다!
- 귀찮음도 귀찮음이지만 두 개의 CMakeLists.txt에서 중복된 내용을 적는다는 것은 그 자체로 위험성이 있다
#### 해결법
- calculator에 사용된 소스 파일의 이름이 source1.cpp ~ source100.cpp 그리고 main.cpp라고 가정한다면 아래와 같이 작성하면 된다
```cmake
# 기존 calculator를 calculator_lib과 calculator로 분리
add_library(calculator_lib source1.cpp source2.cpp (중간 생략) source100.cpp)
add_executable(calculator main.cpp)
target_link_libraries(calculator PRIVATE calculator_lib)

# 유닛 테스트 target은 calculator_lib를 가져와서 사용함
add_executable(test test.cpp)
target_link_libraries(test PRIVATE calculator_lib)
```
## Case Study: fmt
### Install Tree 구조
```
include/
├─ fmt/
│  ├─ format.h <-- public headers
│  ├─ ...
lib/
├─ cmake/
│  ├─ fmt/ <-- cmake 튜토리얼에서 나온대로 "[설치경로]/lib/cmake/[라이브러리 이름]" 패턴을 따르고 있음
│  │  ├─ fmt-config-version.cmake
│  │  ├─ fmt-config.cmake
│  │  ├─ fmt-targets-debug.cmake
│  │  ├─ fmt-targets-release.cmake
│  │  ├─ fmt-targets.cmake
├─ pkgconfig/
│  ├─ fmt.pc <-- 뭔지 모르겠음
├─ fmt.lib <-- Release 바이너리
├─ fmtd.lib <-- Debug 바이너리
```
### install()
- 딱히 shared library나 executable은 없어보이는데도 install destination 네 개를 전부 명시했다
- public header는 라이브러리 이름과 동일한 폴더 아래에 놓는게 일반적인 것 같다.  
C++의 유명인사 boost 라이브러리들도 헤더가 모두 boost라는 폴더 안에 있다.  
install tree뿐만 아니라 소스 자체의 include폴더에도 include/fmt 밑에 헤더가 들어있다.
```cmake
# Install the library and headers.
install(TARGETS ${INSTALL_TARGETS} EXPORT ${targets_export_name}
        LIBRARY DESTINATION ${FMT_LIB_DIR}
        ARCHIVE DESTINATION ${FMT_LIB_DIR}
        PUBLIC_HEADER DESTINATION "${FMT_INC_DIR}/fmt"
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
```
### fmt-config.cmake.in
- ```if (NOT TARGET <target>)```은 cmake에 ```<target>```이라는 target이 없는 경우에 실행되는 조건문으로  
헤더 가드에 나오는 ```#ifndef ... #define ...```과 비슷한 역할을 하는 것으로 보임.  
다른 라이브러리에서도 종종 사용하는 흔한 구문이라고 한다.
- ```${targets_export_name}```은 ```install(TARGETS ... EXPORT ...)```에 등장하는 export set의 이름.
```cmake
@PACKAGE_INIT@

if (NOT TARGET fmt::fmt)
  include(${CMAKE_CURRENT_LIST_DIR}/@targets_export_name@.cmake)
endif ()

check_required_components(fmt)

```
### 유닛 테스트
- test 폴더에 상당히 많은 테스트가 들어있음
- gtest를 사용하고 있음에도 gtest_discover_tests() 대신 기본 명령인 add_test()를 사용한다.
- 루트 CMakeLists.txt에서 ```add_subdirectory(test)```를 하기 전에 ```enable_testing()```만 호출함.  
### export()에 namespace를 사용하는 이유
- 주석을 보면 namespace를 붙일 경우 cmake에서 더 나은 diagnostic를 제공하기 때문이라고 나와있다.
### 언어 표준 설정
- ```set(CMAKE_CXX_STANDARD ...)``` 대신 ```target_compile_features(fmt PUBLIC cxx_std_11)```을 사용한다
- 일부 add_test()의 경우 ```-DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}```를 옵션으로 넘겨준다
- ```set(CMAKE_CXX_STANDARD_REQUIRED True)```는 찾을 수 없었다
- 검색해보니 CMAKE_CXX_STANDARD 대신 target_compile_features를 사용하는 것이 좋다고 한다.  
참고자료: [stackoverflow - cmake cxx standard vs target compile features](https://stackoverflow.com/questions/70667513/cmake-cxx-standard-vs-target-compile-features)