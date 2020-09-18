#include "../include/parking.hh"

namespace component {

void ParkingLevel::addParkingSlot(const ParkingSlot &ps) {
  if (ps.isOccupied()) {
    auto &vt_occupied_parking = m_occupied_parking.at(ps.getVehicleType());
    if (vt_occupied_parking.find(ps.getParkingSlotId()) ==
        vt_occupied_parking.end()) {
      vt_occupied_parking[ps.getParkingSlotId()] = ps;
      m_occupied_parking_count++;
    }
  } else {
    auto &vt_available_parking = m_available_parking.at(ps.getVehicleType());
    if (vt_available_parking.find(ps.getParkingSlotId()) ==
        vt_available_parking.end()) {
      vt_available_parking[ps.getParkingSlotId()] = ps;
      m_available_parking_count++;
    }
  }
}

[[nodiscard]] auto ParkingLevel::getParkingSlot(const VehicleType &vt)
    -> utils::StatusOr<ParkingSlot> {
  auto &vehicle_parking_slots = m_available_parking.at(vt);
  if (!vehicle_parking_slots.empty()) {
    auto parking_slot =
        vehicle_parking_slots.extract(vehicle_parking_slots.begin()).mapped();
    parking_slot.setOccupied(true);
    m_occupied_parking.at(vt)[parking_slot.getParkingSlotId()] = parking_slot;
    m_available_parking_count--;
    m_occupied_parking_count++;
    return utils::StatusOr<ParkingSlot>(parking_slot);
  } else {
    return utils::StatusOr<ParkingSlot>(utils::Status::UNAVAILABLE);
  }
}

void ParkingLevel::returnParkingSlot(const ParkingSlot &ps) {
  assert(ps.isOccupied());
  auto &occupied_vt_map = m_occupied_parking.at(ps.getVehicleType());
  auto &available_vt_map = m_available_parking.at(ps.getVehicleType());
  auto it = occupied_vt_map.find(ps.getParkingSlotId());
  assert(it != occupied_vt_map.end());

  occupied_vt_map.extract(it);
  m_occupied_parking_count--;

  available_vt_map[it->first] = it->second;
  m_available_parking_count++;
}

[[nodiscard]] auto ParkingLot::getTotalAvailableParking() const -> int {
  int total_available_parking = 0;
  for (const auto &level : m_parking_level) {
    total_available_parking += level.getAvailableParking();
  }
  return total_available_parking;
}

[[nodiscard]] auto ParkingLot::getTotalOccupiedParking() const -> int {
  int total_occupied_parking = 0;
  for (const auto &level : m_parking_level) {
    total_occupied_parking += level.getOccupiedParking();
  }
  return total_occupied_parking;
}

[[nodiscard]] auto
ParkingLot::getAvailableParkingForVehicleType(const VehicleType &vt) const
    -> int {
  int total_available_parking = 0;
  for (const auto &level : m_parking_level) {
    total_available_parking += level.getVehicleTypeAvailableParking(vt);
  }
  return total_available_parking;
}

[[nodiscard]] auto
ParkingLot::getOccupiedParkingForVehicleType(const VehicleType &vt) const
    -> int {
  int total_occupied_parking = 0;
  for (const auto &level : m_parking_level) {
    total_occupied_parking += level.getVehicleTypeOccupiedParking(vt);
  }
  return total_occupied_parking;
}

[[nodiscard]] auto ParkingLot::getAvailableParkingForVehicleTypeAtLevel(
    int level, const VehicleType &vt) const -> int {
  assert(level < m_parking_level_count);
  return m_parking_level.at(level).getVehicleTypeAvailableParking(vt);
}

[[nodiscard]] auto ParkingLot::getOccupiedParkingForVehicleTypeAtLevel(
    int level, const VehicleType &vt) const -> int {
  assert(level < m_parking_level_count);
  return m_parking_level.at(level).getVehicleTypeOccupiedParking(vt);
}

[[nodiscard]] auto ParkingLot::getParking(const VehicleType &vt)
    -> utils::StatusOr<ParkingSlot> {
  utils::StatusOr<ParkingSlot> slot;
  for (auto &level : m_parking_level) {
    if (level.getVehicleTypeAvailableParking(vt)) {
      slot.setData(level.getParkingSlot(vt).getData());
    }
  }
  return slot;
}

void ParkingLot::returnParking(const ParkingSlot &vt) {
  m_parking_level.at(vt.getParkingLevel()).returnParkingSlot(vt);
}
} // namespace component