#ifndef PARKING_MANAGER_HH
#define PARKING_MANAGER_HH

#include <vector>

#include "parking.hh"

namespace services {
class ParkingManager {
private:
  component::ParkingLot m_parking_lot;

public:
  ParkingManager() = default;
};
} // namespace services

#endif // PARKING_MANAGER_HH
