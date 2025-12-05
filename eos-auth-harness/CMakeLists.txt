cmake_minimum_required(VERSION 3.15)
project(EOSAuthHarness VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# EOS SDK Path
set(EOS_SDK_PATH $ENV{EOS_SDK_PATH})
if(NOT EOS_SDK_PATH)
    message(FATAL_ERROR "EOS_SDK_PATH environment variable not set")
endif()

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${EOS_SDK_PATH}/Include
)

# Link directories
link_directories(
    ${EOS_SDK_PATH}/Lib
)

# Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Subdirectories
add_subdirectory(src)
add_subdirectory(tests)