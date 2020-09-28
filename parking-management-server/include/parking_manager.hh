#ifndef PARKING_MANAGER_HH
#define PARKING_MANAGER_HH

#include <vector>

#include "parking.hh"
#include "parking_management.grpc.pb.h"
#include "parking_management.pb.h"

namespace services {
class ParkingManagerImpl : public ParkingManager::Service {
private:
  component::ParkingLot m_parking_lot;

  void addParkingSlot(unsigned level, const std::string &vt,
                      const std::string &zone, unsigned id);
  void addParkingSlotsForVehicle(unsigned level, const std::string &vt,
                                 const std::string &zone, unsigned capacity);

public:
  ParkingManagerImpl() = default;
  ::grpc::Status CreateParkingLot(::grpc::ServerContext *context,
                                  const ::ParkingLotDetails *request,
                                  ::Status *response) override;
  virtual ~ParkingManagerImpl() {}
};
} // namespace services

#endif // PARKING_MANAGER_HH
