cmake_minimum_required(VERSION 3.23.2)

project(DigitalImageProcessing)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

set(PROJECT_PATH ${PROJECT_SOURCE_DIR})
configure_file(Config.h.in ${PROJECT_SOURCE_DIR}/include/Config.h @ONLY)

set(BMPImageLib "BMPImageLib")
file(GLOB_RECURSE LIB_SRCS Common "*.cpp" "*.cc" "*.c")
add_library(${BMPImageLib} ${LIB_SRCS})
target_compile_options(${BMPImageLib} PUBLIC -Wall -Werror -Wextra)
target_include_directories(${BMPImageLib} PUBLIC include)

include_directories(include)

foreach(index RANGE 1 4)
  add_subdirectory("Lab${index}")
endforeach(index RANGE 1 4)

