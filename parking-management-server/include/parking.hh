#ifndef PARKING_HH
#define PARKING_HH

#include <sqlite3.h>

#include <array>
#include <cassert>
#include <chrono>
#include <functional>
#include <map>
#include <string>
#include <vector>

#include "utils.hh"
#include "vehicle.hh"

namespace component {
class ParkingSlot {
private:
  int m_parking_level{-1};
  std::string m_parking_slot_id;
  VehicleType m_vt{VehicleType::TOTALVEHICLETYPE};
  bool m_occupied{false};
  std::time_t m_occupied_at;

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
      -> utils::StatusOr<std::time_t> {
    if (m_occupied) {
      return utils::StatusOr<std::time_t>(m_occupied_at);
    } else {
      return utils::StatusOr<std::time_t>(utils::Status::UNAVAILABLE);
    }
  }

  /// Sets the parking time
  inline void setParkingTime(const std::time_t &occupied_at) {
    assert(m_occupied == false);
    m_occupied = true;
    m_occupied_at = occupied_at;
  }

  friend auto operator<<(std::ostream &os, const ParkingSlot &obj)
      -> std::ostream &;
};

class ParkingLot {
private:
  sqlite3 *m_db{nullptr};
  std::string m_db_name;
  std::string m_parking_name;
  unsigned m_parking_level_count{0};

public:
  ParkingLot() = default;
  explicit ParkingLot(std::string name, unsigned parking_levels);

  /// Opens up a DB if already not opened.
  void openDB();

  /// Provides the parking name
  [[nodiscard]] inline auto getName() const -> std::string {
    return m_parking_name;
  }

  /// Sets the Parking name
  void setName(std::string name);

  /// Provides number of parking levels
  [[nodiscard]] inline auto getParkingLevelCount() const -> unsigned {
    return m_parking_level_count;
  }

  /// Sets Parking total number of parking level
  void setParkingLevelCount(unsigned parking_level_count) {
    m_parking_level_count = parking_level_count;
  }

  /// Adds another parking level to the lot
  inline void addParkingLevel() { m_parking_level_count++; }

  /// Iterates over all the parking levels and computes available parking slot
  [[nodiscard]] auto getTotalAvailableParking() const -> unsigned;

  /// Iterates over all the parking levels and computes occupied parking slot
  [[nodiscard]] auto getTotalOccupiedParking() const -> unsigned;

  /// Fetches available parking for a certain parking level
  [[nodiscard]] auto getAvailableParkingAtLevel(unsigned pl) const -> unsigned;

  /// Fetches occupied parking for a certain parking level
  [[nodiscard]] auto getOccupiedParkingAtLevel(unsigned pl) const -> unsigned;

  /// Gets available parking across all levels for a specific VehicleType
  [[nodiscard]] auto
  getAvailableParkingForVehicleType(const VehicleType &vt) const -> unsigned;

  /// Gets occupied parking across all levels for a specific VehicleType
  [[nodiscard]] auto
  getOccupiedParkingForVehicleType(const VehicleType &vt) const -> unsigned;

  /// Gets available parking at certain level for a specific VehicleType
  [[nodiscard]] auto getAvailableParkingForVehicleTypeAtLevel(
      unsigned level, const VehicleType &vt) const -> unsigned;

  /// Gets occupied parking at certain level for a specific VehicleType
  [[nodiscard]] auto getOccupiedParkingForVehicleTypeAtLevel(
      unsigned level, const VehicleType &vt) const -> unsigned;

  /// Tries to get an available parking slot, returns status and slot if able to
  /// allocate
  [[nodiscard]] auto getParking(const VehicleType &vt)
      -> utils::StatusOr<ParkingSlot>;

  /// Returns occupied slot to specific parking level
  void returnParking(const ParkingSlot &vt);

  /// Adds a parking slot to the destined parking level given the unique
  /// identifier for the slot. The unique_id is always of the format
  /// {Parking_Level}_{Vehicle_type}_{Parking_Zone}_{Number}
  /// A typical unique_id for the ParkingSlot would be the following 2_CA_A_3
  /// ParkingLevel - 2
  /// VehicleType - CA (CAR)
  /// Parking Zone - A (Format would be the excel column numbering)
  /// Parking Slot - 3
  void addParking(std::string unique_id);

  /// Given an unique_id of the Parking lot, it provides the specific
  /// ParkingSlot
  [[nodiscard]] auto getParkingSlot(std::string unique_id)
      -> utils::StatusOr<ParkingSlot>;

  // Cleans up all the records without deleting the table for a certain level or
  // all the table.
  void deleteParkingSlots(int level = -1);

  virtual ~ParkingLot();
};

class ParkingIdParser {
private:
  enum States { PARKING_ID, PARKING_LEVEL, VEHICLE_TYPE, TOTAL_STATE };
  inline static ParkingSlot m_parking_slot;
  const inline static std::array<std::function<void(std::string)>,
                                 States::TOTAL_STATE>
      m_state_fptr = {
          [](std::string unique_id) {
            m_parking_slot.setParkingSlotId(std::move(unique_id));
          },
          [](const std::string &level) {
            m_parking_slot.setParkingLevel(std::stoi(level));
          },
          [](const std::string &vehicle_type) {
            assert(m_vtstr_vt_map.find(vehicle_type) != m_vtstr_vt_map.end());
            m_parking_slot.setVehicleType(
                m_vtstr_vt_map.find(vehicle_type)->second);
          },
  };

public:
  ParkingIdParser() = delete;

  /// Provided with a unique_id string, it creates an available ParkingSlot
  [[nodiscard]] static auto parse(std::string unique_id) -> ParkingSlot;
};

/// ParkingSlot creator function from unique_id
[[nodiscard]] auto makeParkingSlot(std::string unique_id) -> ParkingSlot;

/// Dump routines for component::ParkingSlot
auto operator<<(std::ostream &os, const component::ParkingSlot &obj)
    -> std::ostream &;
} // namespace component

#endif // PARKING_HH
