#include "include/client.hh"
#include "parking_management.grpc.pb.h"
#include "parking_management.pb.h"

auto main(int argc, char **argv) -> int {
  client::ParkingManagerClient pm(grpc::CreateChannel(
      "localhost:50051", grpc::InsecureChannelCredentials()));
  pm.createParkingLot();
  return 0;
}