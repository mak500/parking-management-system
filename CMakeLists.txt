# Please enter description for the project
cmake_minimum_required (VERSION 3.11)

enable_language(CXX)
enable_language(C)

set(THIS parking-management-system)

project(${THIS} VERSION 1.0)

# Including custom cmake rules
include(cmake/clang-cxx-dev-tools.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# googletest setup
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        master
)
set(FETCHCONTENT_QUIET OFF)
FetchContent_MakeAvailable(googletest)

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(parking-management-server)

# Doxygen setup
find_package(Doxygen
             REQUIRED dot
             OPTIONAL_COMPONENTS mscgen dia)

set(DOXYGEN_EXTRACT_ALL YES)
set(DOXYGEN_EXTRACT_PRIVATE YES)
doxygen_add_docs(
    doxygen
    parking-management-server
    README.md
    COMMENT "Generate HTML pages"
)

# gRPC setup
include(FetchContent)
FetchContent_Declare(
  gRPC
  GIT_REPOSITORY https://github.com/grpc/grpc
  GIT_TAG        v1.32.0
)
set(FETCHCONTENT_QUIET OFF)
FetchContent_MakeAvailable(gRPC)
