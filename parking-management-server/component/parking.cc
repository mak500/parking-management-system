#include "../include/parking.hh"

#include <ctime>
#include <regex>

#include "../include/utils.hh"
namespace component {

void ParkingLevel::addParkingSlot(ParkingSlot ps) {
  if (ps.isOccupied()) {
    auto &vt_occupied_parking = m_occupied_parking.at(ps.getVehicleType());
    if (vt_occupied_parking.find(ps.getParkingSlotId()) ==
        vt_occupied_parking.end()) {
      vt_occupied_parking[ps.getParkingSlotId()] = std::move(ps);
      m_occupied_parking_count++;
    }
  } else {
    auto &vt_available_parking = m_available_parking.at(ps.getVehicleType());
    if (vt_available_parking.find(ps.getParkingSlotId()) ==
        vt_available_parking.end()) {
      vt_available_parking[ps.getParkingSlotId()] = std::move(ps);
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
    if (level.getVehicleTypeAvailableParking(vt) > 0) {
      slot.setData(level.getParkingSlot(vt).getData());
    }
  }
  return slot;
}

void ParkingLot::returnParking(const ParkingSlot &vt) {
  m_parking_level.at(vt.getParkingLevel()).returnParkingSlot(vt);
}

void ParkingLot::addParking(std::string unique_id) {
  ParkingSlot slot = makeParkingSlot(std::move(unique_id));
  auto &parking_level = m_parking_level.at(slot.getParkingLevel());
  parking_level.addParkingSlot(std::move(slot));
}

[[nodiscard]] auto ParkingIdParser::parse(std::string unique_id)
    -> ParkingSlot {
  m_state_fptr.at(static_cast<unsigned>(States::PARKING_ID))(unique_id);
  std::regex re("_");
  std::sregex_token_iterator it(unique_id.begin(), unique_id.end(), re, -1);
  for (auto index = static_cast<unsigned>(States::PARKING_LEVEL);
       index < static_cast<unsigned>(States::TOTAL_STATE); index++, it++) {
    m_state_fptr.at(index)(*it);
  }
  return m_parking_slot;
}

[[nodiscard]] auto makeParkingSlot(std::string unique_id) -> ParkingSlot {
  return ParkingIdParser::parse(std::move(unique_id));
}

/// Dumper routine for component::ParkingSlot
auto operator<<(std::ostream &os, const component::ParkingSlot &obj)
    -> std::ostream & {
  PRINT_CONTAINER_START();
  PRINT_FIELD("Parking Level : ", obj.m_parking_level);
  PRINT_FIELD("Parking Id : ", obj.m_parking_slot_id);
  PRINT_FIELD("Vehicle Type : ", obj.m_vt);
  PRINT_FIELD("Occupied : ", obj.m_occupied);
  /// TODO @ (madhur) : Need to add remaining members
  PRINT_CONTAINER_END();
  return os;
}

/// Dumper routine for component::ParkingLevel
auto operator<<(std::ostream &os, const component::ParkingLevel &obj)
    -> std::ostream & {
  PRINT_CONTAINER_START();
  PRINT_FIELD("Parking Level : ", obj.m_parking_level);
  PRINT_FIELD("Available Parking : ", obj.m_available_parking_count);
  PRINT_FIELD("Occupied Parking : ", obj.m_occupied_parking_count);
  PRINT_CONTAINER_START();
  for (auto i = static_cast<unsigned>(component::VehicleType::MINIVAN);
       i < static_cast<unsigned>(component::VehicleType::TOTALVEHICLETYPE);
       i++) {
    PRINT_CONTAINER_START();
    for (const auto &node : obj.m_available_parking.at(i)) {
      PRINT_CONTAINER_START();
      PRINT_FIELD("key : ", node.first);
      PRINT_FIELD("value : ", node.second);
      PRINT_CONTAINER_ENDL();
    }
    PRINT_CONTAINER_ENDL();
  }
  PRINT_CONTAINER_ENDL();
  PRINT_CONTAINER_START();
  for (auto i = static_cast<unsigned>(component::VehicleType::MINIVAN);
       i < static_cast<unsigned>(component::VehicleType::TOTALVEHICLETYPE);
       i++) {
    PRINT_CONTAINER_START();
    for (const auto &node : obj.m_occupied_parking.at(i)) {
      PRINT_CONTAINER_START();
      PRINT_FIELD("key : ", node.first);
      PRINT_FIELD("value : ", node.second);
      PRINT_CONTAINER_ENDL();
    }
    PRINT_CONTAINER_ENDL();
  }
  PRINT_CONTAINER_ENDL();
  PRINT_CONTAINER_END();
  return os;
}

auto operator<<(std::ostream &os, const component::ParkingLot &obj)
    -> std::ostream & {
  PRINT_CONTAINER_START();
  PRINT_FIELD("Parking Name : ", obj.m_parking_name);
  PRINT_FIELD("Parking Levels : ", obj.m_parking_level_count);
  PRINT_CONTAINER_START();
  for (unsigned int i = 0; i < obj.m_parking_level.size(); i++) {
    PRINT_FIELD("Level[" + std::to_string(i) + "] : ",
                obj.m_parking_level.at(i));
  }
  PRINT_CONTAINER_ENDL();
  PRINT_CONTAINER_END();
  return os;
}
} // namespace component