#include <iostream>
#include <memory>

#include "include/parking_manager.hh"
#include <grpcpp/server_builder.h>

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  services::ParkingManagerImpl service;

  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<::grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

auto main(int argc, char **argv) -> int {
  RunServer();
  return 0;
}
