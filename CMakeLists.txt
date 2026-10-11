cmake_minimum_required(VERSION 3.25...4.4)

project(CollatzConjecture
    VERSION 1.0.0
    DESCRIPTION "Collatz conjecture iteration-count calculator"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(COLLATZ_WERROR "Treat compiler warnings as errors" OFF)

add_library(collatz_warnings INTERFACE)
target_compile_options(collatz_warnings INTERFACE
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall -Wextra -Wpedantic>
    $<$<AND:$<BOOL:${COLLATZ_WERROR}>,$<CXX_COMPILER_ID:GNU,Clang,AppleClang>>:-Werror>)

add_library(collatz_lib STATIC Collatz.cpp Input.cpp)
target_include_directories(collatz_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(collatz_lib PRIVATE collatz_warnings)

add_executable(collatz main.cpp)
target_link_libraries(collatz PRIVATE collatz_lib collatz_warnings)

include(CTest)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
