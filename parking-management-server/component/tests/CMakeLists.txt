# Please enter description for the project
cmake_minimum_required (VERSION 3.11)

enable_language(CXX)
enable_language(C)

set(THIS components_test)

project(${THIS} VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB CC_SOURCES "*.cc")
file(GLOB HEADERS "*.h")

add_executable(${THIS} ${CC_SOURCES} ${HEADERS})
target_link_libraries(${THIS} gtest_main components)
add_test(NAME ${THIS} COMMAND ${THIS})