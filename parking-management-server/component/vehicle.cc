#include "../include/vehicle.hh"

namespace component {
std::array<std::string, TOTALVEHICLETYPE> m_vt_vtstr = {"MINIVAN", "CAR",
                                                        "MOTORCYCLE", "CYCLE"};

std::map<std::string, VehicleType> m_vtstr_vt_map = {
    {"MV", VehicleType::MINIVAN},
    {"CA", VehicleType::CAR},
    {"MC", VehicleType::MOTORCYCLE},
    {"CY", VehicleType::CYCLE},
    {"MINIVAN", VehicleType::MINIVAN},
    {"CAR", VehicleType::CAR},
    {"MOTORCYCLE", VehicleType::MOTORCYCLE},
    {"CYCLE", VehicleType::CYCLE}};
} // namespace component