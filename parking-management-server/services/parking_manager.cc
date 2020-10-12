#include "../include/parking_manager.hh"

namespace services {

void ParkingManagerImpl::addParkingSlot(unsigned level, const std::string &vt,
                                        const std::string &zone, unsigned id) {
  m_parking_lot.addParking(std::to_string(level) + "_" + vt + "_" + zone + "_" +
                           std::to_string(id));
}

void ParkingManagerImpl::addParkingSlotsForVehicle(unsigned level,
                                                   const std::string &vt,
                                                   const std::string &zone,
                                                   unsigned capacity) {
  for (unsigned i = 0; i < capacity; i++) {
    addParkingSlot(level, vt, zone, i);
  }
}

::grpc::Status
ParkingManagerImpl::CreateParkingLot(::grpc::ServerContext *context,
                                     const ::ParkingLotDetails *request,
                                     ::Status *response) {
  m_parking_lot.setName(request->name());
  m_parking_lot.setParkingLevelCount(request->levels());

  for (unsigned level = 0; level < request->levels(); level++) {
    const auto &capacity = request->level_vehicle_capacity(level);
    addParkingSlotsForVehicle(level, "MV", "A", capacity.minivan_capacity());
    addParkingSlotsForVehicle(level, "CA", "B", capacity.car_capacity());
    addParkingSlotsForVehicle(level, "MC", "C", capacity.motocycle_capacity());
    addParkingSlotsForVehicle(level, "CY", "D", capacity.cycle_capacity());
  }

  return ::grpc::Status::OK;
}
} // namespace services