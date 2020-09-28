# Please enter description for the project
cmake_minimum_required (VERSION 3.11)

enable_language(CXX)
enable_language(C)

set(THIS client)

project(${THIS} VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB CC_SOURCES "*.cc")
file(GLOB HEADERS "*.h")

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Proto file
get_filename_component(PM_PROTO "../../proto/parking_management.proto" ABSOLUTE)
get_filename_component(PM_PROTO_PATH "${PM_PROTO}" PATH)

# Generated sources
set(PM_PROTO_SRCS "${CMAKE_CURRENT_BINARY_DIR}/parking_management.pb.cc")
set(PM_PROTO_HDRS "${CMAKE_CURRENT_BINARY_DIR}/parking_management.pb.h")
set(PM_GRPC_SRCS "${CMAKE_CURRENT_BINARY_DIR}/parking_management.grpc.pb.cc")
set(PM_GRPC_HDRS "${CMAKE_CURRENT_BINARY_DIR}/parking_management.grpc.pb.h")
add_custom_command(
      OUTPUT "${PM_PROTO_SRCS}" "${PM_PROTO_HDRS}" "${PM_GRPC_SRCS}" "${PM_GRPC_HDRS}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${PM_PROTO_PATH}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${PM_PROTO}"
      DEPENDS "${PM_PROTO}")

add_library(${THIS} ${CC_SOURCES} ${HEADERS} ${PM_PROTO_SRCS} ${PM_GRPC_SRCS})
target_link_libraries(${THIS} ${_REFLECTION} ${_GRPC_GRPCPP} ${_PROTOBUF_LIBPROTOBUF})