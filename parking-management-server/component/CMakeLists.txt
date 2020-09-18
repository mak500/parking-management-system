# Please enter description for the project
cmake_minimum_required (VERSION 3.6)

enable_language(CXX)
enable_language(C)

set(THIS components)

project(${THIS} VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB CC_SOURCES "*.cc")
file(GLOB HEADERS "*.h")

add_library(${THIS} ${CC_SOURCES} ${HEADERS})

add_subdirectory(tests)