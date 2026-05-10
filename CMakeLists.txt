cmake_minimum_required(VERSION 3.21)
project(Phosphorium)

set(CMAKE_CXX_STANDARD 17)

# Let CEF's own build system fetch the right version automatically
set(CEF_VERSION "133.4.4+g4feec62+chromium-133.0.6943.142")

include(FetchContent)
FetchContent_Declare(
  cef_cmake
  GIT_REPOSITORY https://github.com/chromiumembedded/cef-project.git
  GIT_TAG        master
)
FetchContent_MakeAvailable(cef_cmake)
