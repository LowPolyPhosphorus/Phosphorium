cmake_minimum_required(VERSION 3.21)
project(Phosphorium)

set(CMAKE_CXX_STANDARD 17)

# CEF lives here after fetch_cef.sh runs
set(CEF_VERSION "144.0.6+g5f7e671+chromium-144.0.7559.59")
set(CEF_ROOT "${CMAKE_SOURCE_DIR}/cef")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CEF_ROOT}/cmake")

find_package(CEF REQUIRED)
add_subdirectory(${CEF_LIBCEF_DLL_WRAPPER_PATH} libcef_dll_wrapper)

add_executable(Phosphorium src/main.cpp)
target_include_directories(Phosphorium PRIVATE ${CEF_ROOT})
target_link_libraries(Phosphorium libcef_dll_wrapper ${CEF_LIBRARIES})
