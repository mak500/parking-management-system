#ifndef VEHICLE_HH
#define VEHICLE_HH

#include <array>
#include <map>
#include <string>
namespace component {
enum VehicleType { MINIVAN, CAR, MOTORCYCLE, CYCLE, TOTALVEHICLETYPE };
extern std::array<std::string, TOTALVEHICLETYPE> m_vt_vtstr;

extern std::map<std::string, VehicleType> m_vtstr_vt_map;
} // namespace component

#endif // VEHICLE_HH
