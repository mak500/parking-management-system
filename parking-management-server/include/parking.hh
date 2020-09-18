#ifndef PARKING_HH
#define PARKING_HH

#include <array>
#include <cassert>
#include <chrono>
#include <map>
#include <string>
#include <vector>

#include "utils.hh"
#include "vehicle.hh"

namespace component {
class ParkingSlot {
private:
  int m_parking_level;
  std::string m_parking_slot_id{0};
  VehicleType m_vt{VehicleType::TotalVehicleType};
  bool m_occupied{false};
  std::chrono::time_point<std::chrono::system_clock> m_occupied_at;

public:
  ParkingSlot() = default;
  ParkingSlot(int level, std::string parking, const VehicleType &vt)
      : m_parking_level(level), m_parking_slot_id(std::move(parking)),
        m_vt(vt) {}

  /// Return parking level for the parking spot
  [[nodiscard]] inline auto getParkingLevel() const -> int {
    return m_parking_level;
  }

  /// Sets parking level for the parking spot
  inline void setParkingLevel(int level) { m_parking_level = level; }

  /// Returns an unique ID for the parking slot
  [[nodiscard]] inline auto getParkingSlotId() const -> std::string {
    return m_parking_slot_id;
  }

  /// Sets an unique ID for the parking slot
  inline void setParkingSlotId(std::string id) {
    m_parking_slot_id = std::move(id);
  }

  /// Returns the type of Vehicle that the parking spot can park
  [[nodiscard]] inline auto getVehicleType() const -> VehicleType {
    return m_vt;
  }

  /// Sets the type of Vehicle that the parking spot can park
  inline void setVehicleType(const VehicleType &vt) { m_vt = vt; }

  /// Returns if the parking spot is occupied
  [[nodiscard]] inline auto isOccupied() const -> bool { return m_occupied; }

  /// Marks the parking spot available or occupied
  inline void setOccupied(bool occupied) { m_occupied = occupied; }

  /// Returns the time at which parking spot was occupied. If the parking spot
  /// is not occupied, it retuns UNAVAILABLE status
  [[nodiscard]] inline auto getParkingTime() const
      -> utils::StatusOr<std::chrono::time_point<std::chrono::system_clock>> {
    if (m_occupied) {
      return utils::StatusOr<
          std::chrono::time_point<std::chrono::system_clock>>(m_occupied_at);
    } else {
      return utils::StatusOr<
          std::chrono::time_point<std::chrono::system_clock>>(
          utils::Status::UNAVAILABLE);
    }
  }

  /// Sets the parking time
  inline void setParkingTime(
      const std::chrono::time_point<std::chrono::system_clock> &occupied_at) {
    assert(m_occupied == false);
    m_occupied = true;
    m_occupied_at = occupied_at;
  }
};

class ParkingLevel {
private:
  using Parking = std::array<std::map<std::string, ParkingSlot>,
                             VehicleType::TotalVehicleType>;
  int m_parking_level{0};
  int m_available_parking_count{0};
  int m_occupied_parking_count{0};
  Parking m_available_parking;
  Parking m_occupied_parking;

public:
  ParkingLevel() = default;
  explicit ParkingLevel(int parking_level) : m_parking_level(parking_level) {}

  /// Returns the parking level of the parking
  [[nodiscard]] inline auto getParkingLevel() const -> int {
    return m_parking_level;
  }

  /// Sets the parking level of the parking
  inline void setParkingLevel(int parking_level) {
    m_parking_level = parking_level;
  }

  /// Returns the number of total available parking
  [[nodiscard]] inline auto getAvailableParking() const -> int {
    return m_available_parking_count;
  }

  /// Returns the number of total occupied parking
  [[nodiscard]] inline auto getOccupiedParking() const -> int {
    return m_occupied_parking_count;
  }

  /// Returns the number of available parking spot for a specific vehicle type
  [[nodiscard]] inline auto
  getVehicleTypeAvailableParking(const VehicleType &vt) const -> int {
    assert(vt < VehicleType::TotalVehicleType);
    return m_available_parking.at(vt).size();
  }

  /// Returns the number of occupied parking spot for a specific vehicle type
  [[nodiscard]] inline auto
  getVehicleTypeOccupiedParking(const VehicleType &vt) const -> int {
    assert(vt < VehicleType::TotalVehicleType);
    return m_occupied_parking.at(vt).size();
  }

  /// Adds a parking slot to the respective occupied/available parking to the
  /// respective vehicle type
  void addParkingSlot(const ParkingSlot &ps);

  /// For a specific VehicleType, it returns ParkingSlot if available or returns
  /// the status unavailable
  [[nodiscard]] auto getParkingSlot(const VehicleType &vt)
      -> utils::StatusOr<ParkingSlot>;

  /// Adds back the parking slot to available
  void returnParkingSlot(const ParkingSlot &ps);
};

class ParkingLot {
private:
  int m_parking_level_count{0};
  std::vector<ParkingLevel> m_parking_level;

public:
  ParkingLot() = default;
  explicit ParkingLot(int parking_level_count)
      : m_parking_level_count(parking_level_count) {
    m_parking_level.reserve(m_parking_level_count);
  }

  /// Iterates over all the parking levels and computes available parking slot
  [[nodiscard]] auto getTotalAvailableParking() const -> int;

  /// Iterates over all the parking levels and computes occupied parking slot
  [[nodiscard]] auto getTotalOccupiedParking() const -> int;

  /// Fetches available parking for a certain parking level
  [[nodiscard]] inline auto getAvailableParkingAtLevel(int pl) const -> int {
    assert(pl < m_parking_level_count && pl >= 0);
    return m_parking_level.at(pl).getAvailableParking();
  }

  /// Fetches occupied parking for a certain parking level
  [[nodiscard]] auto getOccupiedParkingAtLevel(int pl) const -> int {
    assert(pl < m_parking_level_count && pl >= 0);
    return m_parking_level.at(pl).getOccupiedParking();
  }

  /// Gets available parking across all levels for a specific VehicleType
  [[nodiscard]] auto
  getAvailableParkingForVehicleType(const VehicleType &vt) const -> int;

  /// Gets occupied parking across all levels for a specific VehicleType
  [[nodiscard]] auto
  getOccupiedParkingForVehicleType(const VehicleType &vt) const -> int;

  /// Gets available parking at certain level for a specific VehicleType
  [[nodiscard]] auto
  getAvailableParkingForVehicleTypeAtLevel(int level,
                                           const VehicleType &vt) const -> int;

  /// Gets occupied parking at certain level for a specific VehicleType
  [[nodiscard]] auto
  getOccupiedParkingForVehicleTypeAtLevel(int level,
                                          const VehicleType &vt) const -> int;

  /// Tries to get an available parking slot, returns status and slot if able to
  /// allocate
  [[nodiscard]] auto getParking(const VehicleType &vt)
      -> utils::StatusOr<ParkingSlot>;

  /// Returns occupied slot to specific parking level
  void returnParking(const ParkingSlot &vt);
};
} // namespace component

#endif // PARKING_HH
