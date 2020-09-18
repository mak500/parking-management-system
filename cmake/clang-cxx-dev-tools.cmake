# Additional targets to perform clang-format/clang-tidy
# Get all project files
file(GLOB_RECURSE
  ALL_CXX_SOURCE_FILES
  *.[chi]pp *.[chi]xx *.cc *.hh *.ii *.[CHI]
  )

# Remove unecessary files from the build directory.
# TODO(madhukar) : Find a clever to avoid it in the first place itself
list(FILTER ALL_CXX_SOURCE_FILES EXCLUDE REGEX ".*build.*[chi]pp")
list(FILTER ALL_CXX_SOURCE_FILES EXCLUDE REGEX ".*build.*[chi]xx")
list(FILTER ALL_CXX_SOURCE_FILES EXCLUDE REGEX ".*build.*cc")
list(FILTER ALL_CXX_SOURCE_FILES EXCLUDE REGEX ".*build.*hh")
list(FILTER ALL_CXX_SOURCE_FILES EXCLUDE REGEX ".*build.*ii")

# Adding clang-format target if executable is found
find_program(CLANG_FORMAT "clang-format")
if(CLANG_FORMAT)
  add_custom_target(
    clang-format
    COMMAND /usr/bin/clang-format
    -i
    -style=file
    ${ALL_CXX_SOURCE_FILES}
    )
endif()

# Adding clang-tidy target if executable is found
find_program(CLANG_TIDY "clang-tidy")
if(CLANG_TIDY)
  add_custom_target(
    clang-tidy
    COMMAND /usr/bin/clang-tidy
    ${ALL_CXX_SOURCE_FILES}
    -config=''
    --
    -std=c++17
    ${INCLUDE_DIRECTORIES}
    )
endif()
