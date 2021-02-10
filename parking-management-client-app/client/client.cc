#include "../include/client.hh"

#include <iostream>
#include <string>

namespace client {
void ParkingManagerClient::createParkingLot() {
  ParkingLotDetails details;
  Status result;
  grpc::ClientContext context;

  std::cout << "Provide the name of the Parking Lot" << std::endl;
  std::string parking_name;
  getline(std::cin ,parking_name);
  details.set_name(parking_name);

  std::cout << "Provide the number of parking levels" << std::endl;
  unsigned levels;
  std::cin >> levels;
  details.set_levels(levels);

  for (unsigned i = 0; i < levels; i++) {
    unsigned mv_capacity, ca_capacity, mc_capacity, cy_capacity;
    ParkingLevelCapacity *level = details.add_level_vehicle_capacity();

    std::cout << "Provide the number of minivan parking slot" << std::endl;
    std::cin >> mv_capacity;
    level->set_minivan_capacity(mv_capacity);

    std::cout << "Provide the number of car parking slot" << std::endl;
    std::cin >> ca_capacity;
    level->set_car_capacity(ca_capacity);

    std::cout << "Provide the number of motor cycle parking slot" << std::endl;
    std::cin >> mc_capacity;
    level->set_motocycle_capacity(mc_capacity);

    std::cout << "Provide the number of cycle parking slot" << std::endl;
    std::cin >> cy_capacity;
    level->set_cycle_capacity(cy_capacity);
  }

  /// Do the service call and create the parking slots
  grpc::Status status = m_stub->CreateParkingLot(&context, details, &result);
  if (!status.ok()) {
    std::cout << "RPC issue" << std::endl;
    std::cout << status.error_details() << std::endl;
  }
}
} // namespace client