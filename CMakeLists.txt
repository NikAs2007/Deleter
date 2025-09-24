cmake_minimum_required(VERSION 3.30)
project(FileManager)

set(PrName ${PROJECT_NAME})
set(MAKE_BUILD_TYPE Release)

set(CMAKE_CXX_STANDARD 17) #версия с++
set(CMAKE_CXX_STANDARD_REQUIRED ON) #значит обязательно использовать указанный стандарт

add_executable(${PrName} main.cpp FileManager.cpp FileManagerUI.cpp FileManagerBK.cpp)

target_include_directories(${PrName} PRIVATE .)
