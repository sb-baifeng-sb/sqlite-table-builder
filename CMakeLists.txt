#1.cmake verson，指定cmake版本 
cmake_minimum_required(VERSION 3.0)

#2.project name，指定项目的名称，一般和项目的文件夹名称对应
PROJECT(stb)

#指定c++版本
#add_definitions(-std=c++17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#3.head file path，头文件目录
INCLUDE_DIRECTORIES(
    CppSQLite
    sqlite3
)

#4.source directory，源文件目录
file(GLOB SRC_FILES
    "./*.c*"
    "stb/*.c*"
    "CppSQLite/*.c*"
    "sqlite3/*.c*")

#add_library(${SRC_FILES})

#5.set environment variable，设置环境变量，编译用到的源文件全部都要放到这里，否则编译能够通过，但是执行的时候会出现各种问题，比如"symbol lookup error xxxxx , undefined symbol"
#SET(TEST_MATH ${SRC_FILES})

#6.add executable file，添加要编译的可执行文件
ADD_EXECUTABLE(${PROJECT_NAME} ${SRC_FILES})

#7.add link library，添加可执行文件所需要的库，比如我们用到了libm.so（命名规则：lib+name+.so），就添加该库的名称
TARGET_LINK_LIBRARIES(${PROJECT_NAME})