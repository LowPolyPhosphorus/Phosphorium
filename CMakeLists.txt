cmake_minimum_required(VERSION 3.20)
project(Phosphorium)

set(CMAKE_CXX_STANDARD 17)

# Where CEF lives (downloaded by fetch_cef.sh)
set(CEF_ROOT "${CMAKE_SOURCE_DIR}/cef")

# Pull in CEF's own build helpers
include("${CEF_ROOT}/cmake/FindCEF.cmake")
add_subdirectory("${CEF_ROOT}/libcef_dll" libcef_dll)

# Your source files
add_executable(Phosphorium src/main.cpp)

# Link CEF into your app
target_include_directories(Phosphorium PRIVATE "${CEF_ROOT}")
target_link_libraries(Phosphorium libcef_dll_wrapper libcef)
