include(FetchContent)

FetchContent_Declare(Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG bcfb10e498df3e2ed8f814b3e4b689b9a85608ab # v3.15.1
    SYSTEM)
FetchContent_MakeAvailable(Catch2)

add_executable(collatz_tests collatz_tests.cpp)
target_link_libraries(collatz_tests PRIVATE collatz_lib collatz_warnings Catch2::Catch2WithMain)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(collatz_tests)
