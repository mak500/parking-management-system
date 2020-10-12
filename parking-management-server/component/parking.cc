#include "../include/parking.hh"

#include <ctime>
#include <regex>

#include "../include/utils.hh"
namespace component {
auto sql_call_and_check = [](std::string_view filename, int lineno, sqlite3 *db,
                             auto fn) {
  int error_code = fn();
  if (error_code != SQLITE_OK && error_code != SQLITE_DONE &&
      error_code != SQLITE_ROW) {
    std::cerr << filename.data() << "@" << lineno << " : " << error_code << "="
              << sqlite3_errmsg(db) << std::endl;
  }
};

ParkingLot::ParkingLot(std::string name, unsigned parking_level_count)
    : m_parking_name(std::move(name)),
      m_parking_level_count(parking_level_count) {
  openDB();
}

void ParkingLot::setName(std::string name) {
  m_parking_name = std::move(name);
  openDB();
}

void ParkingLot::openDB() {
  if (m_db != nullptr) {
    return;
  }

  m_db_name = m_parking_name + ".db";
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_open, m_db_name.c_str(), &m_db));
  sqlite3_stmt *sql_stmt;

  // clang format off
  std::string command = "create table if not exists parking ("
                        "parking_id varchar(20) primary key,"
                        "occupied_status binary,"
                        "parking_level int,"
                        "vehicle_type varchar(20),"
                        "occupied_at time);";
  // clang format on

  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_prepare_v2, m_db, command.c_str(), -1,
                               &sql_stmt, nullptr));
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_step, sql_stmt));
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_finalize, sql_stmt));
}

[[nodiscard]] auto ParkingLot::getTotalAvailableParking() const -> unsigned {
  sqlite3_stmt *sql_stmt;

  // clang format off
  std::string command = "select count (parking_id) from parking "
                        "where occupied_status = false";
  // clang format on
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_prepare_v2, m_db, command.c_str(), -1,
                               &sql_stmt, nullptr));
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_step, sql_stmt));
  unsigned result = sqlite3_column_int(sql_stmt, 0);
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_finalize, sql_stmt));
  return result;
}

[[nodiscard]] auto ParkingLot::getTotalOccupiedParking() const -> unsigned {
  sqlite3_stmt *sql_stmt;

  // clang format off
  std::string command = "select count (parking_id) from parking "
                        "where occupied_status = true";
  // clang format on
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_prepare_v2, m_db, command.c_str(), -1,
                               &sql_stmt, nullptr));
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_step, sql_stmt));
  unsigned result = sqlite3_column_int(sql_stmt, 0);
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_finalize, sql_stmt));
  return result;
}

[[nodiscard]] auto ParkingLot::getAvailableParkingAtLevel(unsigned pl) const
    -> unsigned {
  sqlite3_stmt *sql_stmt;

  // clang format off
  std::string command = "select count (parking_id) from parking "
                        "where occupied_status = false and parking_level = ?";
  // clang format on
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_prepare_v2, m_db, command.c_str(), -1,
                               &sql_stmt, nullptr));
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_bind_int, sql_stmt, 1, pl));
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_step, sql_stmt));
  unsigned result = sqlite3_column_int(sql_stmt, 0);
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_finalize, sql_stmt));
  return result;
};

[[nodiscard]] auto ParkingLot::getOccupiedParkingAtLevel(unsigned int pl) const
    -> unsigned {
  sqlite3_stmt *sql_stmt;

  // clang format off
  std::string command = "select count (parking_id) from parking "
                        "where occupied_status = true and parking_level = ?";
  // clang format on
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_prepare_v2, m_db, command.c_str(), -1,
                               &sql_stmt, nullptr));
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_bind_int, sql_stmt, 1, pl));
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_step, sql_stmt));
  unsigned result = sqlite3_column_int(sql_stmt, 0);
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_finalize, sql_stmt));
  return result;
}

[[nodiscard]] auto
ParkingLot::getAvailableParkingForVehicleType(const VehicleType &vt) const
    -> unsigned {
  sqlite3_stmt *sql_stmt;

  // clang format off
  std::string command = "select count (parking_id) from parking "
                        "where occupied_status = false and vehicle_type = ?";
  // clang format on
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_prepare_v2, m_db, command.c_str(), -1,
                               &sql_stmt, nullptr));
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_bind_text, sql_stmt, 1,
                               m_vt_vtstr[static_cast<unsigned>(vt)].c_str(),
                               -1, nullptr));
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_step, sql_stmt));
  unsigned result = sqlite3_column_int(sql_stmt, 0);
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_finalize, sql_stmt));
  return result;
}

[[nodiscard]] auto
ParkingLot::getOccupiedParkingForVehicleType(const VehicleType &vt) const
    -> unsigned {
  sqlite3_stmt *sql_stmt;

  // clang format off
  std::string command = "select count (parking_id) from parking "
                        "where occupied_status = true and vehicle_type = ?";
  // clang format on
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_prepare_v2, m_db, command.c_str(), -1,
                               &sql_stmt, nullptr));
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_bind_text, sql_stmt, 1,
                               m_vt_vtstr[static_cast<unsigned>(vt)].c_str(),
                               -1, nullptr));
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_step, sql_stmt));
  unsigned result = sqlite3_column_int(sql_stmt, 0);
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_finalize, sql_stmt));
  return result;
}

[[nodiscard]] auto ParkingLot::getAvailableParkingForVehicleTypeAtLevel(
    unsigned level, const VehicleType &vt) const -> unsigned {
  sqlite3_stmt *sql_stmt;

  // clang format off
  std::string command = "select count (parking_id) from parking "
                        "where occupied_status = false and vehicle_type = ? "
                        "and parking_level = ?";
  // clang format on
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_prepare_v2, m_db, command.c_str(), -1,
                               &sql_stmt, nullptr));
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_bind_text, sql_stmt, 1,
                               m_vt_vtstr[static_cast<unsigned>(vt)].c_str(),
                               -1, nullptr));
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_bind_int, sql_stmt, 2, level));
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_step, sql_stmt));
  unsigned result = sqlite3_column_int(sql_stmt, 0);
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_finalize, sql_stmt));
  return result;
}

[[nodiscard]] auto ParkingLot::getOccupiedParkingForVehicleTypeAtLevel(
    unsigned level, const VehicleType &vt) const -> unsigned {
  sqlite3_stmt *sql_stmt;

  // clang format off
  std::string command = "select count (parking_id) from parking "
                        "where occupied_status = true and vehicle_type = ? "
                        "and parking_level = ?";
  // clang format on
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_prepare_v2, m_db, command.c_str(), -1,
                               &sql_stmt, nullptr));
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_bind_text, sql_stmt, 1,
                               m_vt_vtstr[static_cast<unsigned>(vt)].c_str(),
                               -1, nullptr));
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_bind_int, sql_stmt, 2, level));
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_step, sql_stmt));
  unsigned result = sqlite3_column_int(sql_stmt, 0);
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_finalize, sql_stmt));
  return result;
}

[[nodiscard]] auto ParkingLot::getParking(const VehicleType &vt)
    -> utils::StatusOr<ParkingSlot> {
  sqlite3_stmt *sql_stmt;
  utils::StatusOr<ParkingSlot> result;

  // clang format off
  std::string command = "select * from parking where vehicle_type= ? and "
                        "occupied_status=false limit 1";
  // clang format on
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_prepare_v2, m_db, command.c_str(), -1,
                               &sql_stmt, nullptr));
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_bind_text, sql_stmt, 1,
                               m_vt_vtstr[static_cast<unsigned>(vt)].c_str(),
                               -1, nullptr));

  if (sqlite3_step(sql_stmt) == SQLITE_ROW) {
    ParkingSlot slot;
    slot.setParkingSlotId(
        reinterpret_cast<const char *>(sqlite3_column_text(sql_stmt, 0)));
    slot.setParkingLevel(sqlite3_column_int(sql_stmt, 2));
    slot.setVehicleType(vt);
    slot.setParkingTime(std::time(nullptr));
    result.setData(std::move(slot));

    sql_call_and_check(__FILE__, __LINE__, m_db,
                       std::bind(sqlite3_finalize, sql_stmt));
    std::string command = "update parking set occupied_status = true, "
                          "occupied_at = ? where parking_id = ?";
    sql_call_and_check(__FILE__, __LINE__, m_db,
                       std::bind(sqlite3_prepare_v2, m_db, command.c_str(), -1,
                                 &sql_stmt, nullptr));
    sql_call_and_check(__FILE__, __LINE__, m_db,
                       std::bind(sqlite3_bind_int64, sql_stmt, 1,
                                 slot.getParkingTime().getData()));
    sql_call_and_check(__FILE__, __LINE__, m_db,
                       std::bind(sqlite3_bind_text, sql_stmt, 2,
                                 slot.getParkingSlotId().c_str(), -1, nullptr));
    sql_call_and_check(__FILE__, __LINE__, m_db,
                       std::bind(sqlite3_step, sql_stmt));
  }

  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_finalize, sql_stmt));
  return result;
}

void ParkingLot::returnParking(const ParkingSlot &slot) {
  sqlite3_stmt *sql_stmt;
  std::string command = "update parking set occupied_status = false "
                        "where parking_id = ?";
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_prepare_v2, m_db, command.c_str(), -1,
                               &sql_stmt, nullptr));
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_bind_text, sql_stmt, 1,
                               slot.getParkingSlotId().c_str(), -1, nullptr));
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_step, sql_stmt));
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_finalize, sql_stmt));
}

void ParkingLot::addParking(std::string unique_id) {
  ParkingSlot slot = makeParkingSlot(unique_id);

  sqlite3_stmt *sql_stmt;
  std::string command = "insert into parking values(?, ?, ?, ?, ?);";
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_prepare_v2, m_db, command.c_str(), -1,
                               &sql_stmt, nullptr));
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_bind_text, sql_stmt, 1,
                               slot.getParkingSlotId().c_str(), -1, nullptr));
  sql_call_and_check(
      __FILE__, __LINE__, m_db,
      std::bind(sqlite3_bind_int, sql_stmt, 2, slot.isOccupied()));
  sql_call_and_check(
      __FILE__, __LINE__, m_db,
      std::bind(sqlite3_bind_int, sql_stmt, 3, slot.getParkingLevel()));
  sql_call_and_check(
      __FILE__, __LINE__, m_db,
      std::bind(
          sqlite3_bind_text, sql_stmt, 4,
          m_vt_vtstr[static_cast<unsigned>(slot.getVehicleType())].c_str(), -1,
          nullptr));
  sql_call_and_check(
      __FILE__, __LINE__, m_db,
      std::bind(sqlite3_bind_int64, sql_stmt, 5, std::time(nullptr)));
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_step, sql_stmt));
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_finalize, sql_stmt));
}

[[nodiscard]] auto ParkingLot::getParkingSlot(std::string unique_id)
    -> utils::StatusOr<ParkingSlot> {
  utils::StatusOr<ParkingSlot> result;
  sqlite3_stmt *sql_stmt;
  std::string command =
      "select * from parking where parking_id = \"" + unique_id + "\"";
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_prepare_v2, m_db, command.c_str(), -1,
                               &sql_stmt, nullptr));
  if (sqlite3_step(sql_stmt) == SQLITE_ROW) {
    const char *unique_id =
        reinterpret_cast<const char *>(sqlite3_column_text(sql_stmt, 0));
    bool isOccupied = sqlite3_column_int(sql_stmt, 1);
    int level = sqlite3_column_int(sql_stmt, 2);
    const char *vehicle_type =
        reinterpret_cast<const char *>(sqlite3_column_text(sql_stmt, 3));
    assert(m_vtstr_vt_map.find(vehicle_type) != m_vtstr_vt_map.end());
    VehicleType vt = m_vtstr_vt_map[vehicle_type];
    std::time_t occupied_at = sqlite3_column_int(sql_stmt, 4);
    ParkingSlot slot(level, unique_id, vt);
    if (isOccupied) {
      slot.setParkingTime(occupied_at);
    }
    result.setData(slot);
  }
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_finalize, sql_stmt));
  return result;
}

void ParkingLot::deleteParkingSlots(int level) {
  std::string command = "delete from parking";
  if (level != -1) {
    command += " where parking_level = " + std::to_string(level);
  }
  sql_call_and_check(__FILE__, __LINE__, m_db,
                     std::bind(sqlite3_exec, m_db, command.c_str(), nullptr,
                               nullptr, nullptr));
}

ParkingLot::~ParkingLot() {
  sql_call_and_check(__FILE__, __LINE__, m_db, std::bind(sqlite3_close, m_db));
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
  PRINT_FIELD("Occupied at :",
              std::asctime(std::localtime(&obj.m_occupied_at)));
  PRINT_CONTAINER_END();
  return os;
}
} // namespace component