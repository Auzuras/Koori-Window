cmake_minimum_required(VERSION 3.20)

project(KOORI_WINDOW LANGUAGES CXX)

option(KOORI_WINDOW_TEST_OPT "Build Koori Window tests" ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE KOORI_WINDOW_SOURCES "Source/*.cpp")

add_library(KooriWindow STATIC ${KOORI_WINDOW_SOURCES})

target_include_directories(KooriWindow PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)

if(KOORI_WINDOW_TEST_OPT)
    enable_testing()

    add_subdirectory(Tests)
endif()

if(KOORI_WINDOWS)
    add_compile_definitions(KOORI_WINDOWS)
endif()