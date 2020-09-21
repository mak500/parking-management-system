#include <fstream>
#include <iostream>

#include "../../include/parking.hh"
#include "gtest/gtest.h"

TEST(ParkingSlot, ParkingSlotAPI) {
  component::ParkingSlot slot(3, "3A 1", component::VehicleType::CAR);
  ASSERT_EQ(slot.getParkingLevel(), 3)
      << "Parking level is not set correctly" << std::endl;
  ASSERT_EQ(slot.getParkingSlotId(), std::string("3A 1"))
      << "Parking Slot Id is not set correctly" << std::endl;
  ASSERT_EQ(slot.getVehicleType(), component::VehicleType::CAR)
      << "Parking Slot Vehicle Type is not set correctly" << std::endl;
  ASSERT_EQ(slot.getParkingTime().isOk(), false)
      << "Parking Slot time must not be fetched when not occupied" << std::endl;
  auto time = std::chrono::system_clock::now();

  slot.setParkingTime(time);

  ASSERT_EQ(slot.isOccupied(), true)
      << "Parking Slot should be occupied" << std::endl;
  ASSERT_EQ(slot.getParkingTime().getData(), time)
      << "Parking Slot time isnt set properly" << std::endl;

  slot.setOccupied(false);

  ASSERT_EQ(slot.getParkingTime().isOk(), false)
      << "Parking Slot time should be unavailable" << std::endl;
}

TEST(ParkingLevel, ParkingLevelAPI) {
  component::ParkingLevel pl(1);
  component::ParkingSlot slot(1, "1A 1", component::VehicleType::CAR);

  pl.addParkingSlot(slot);
  ASSERT_EQ(pl.getVehicleTypeAvailableParking(component::VehicleType::CAR), 1)
      << "Incorrect Parking slot addition" << std::endl;
  ASSERT_EQ(pl.getVehicleTypeAvailableParking(component::VehicleType::CYCLE), 0)
      << "Incorrect Parking slot addition" << std::endl;
  ASSERT_EQ(pl.getVehicleTypeAvailableParking(component::VehicleType::MINIVAN),
            0)
      << "Incorrect Parking slot addition" << std::endl;
  ASSERT_EQ(
      pl.getVehicleTypeAvailableParking(component::VehicleType::MOTORCYCLE), 0)
      << "Incorrect Parking slot addition" << std::endl;

  ASSERT_EQ(pl.getVehicleTypeOccupiedParking(component::VehicleType::CAR), 0)
      << "Incorrect Parking slot addition" << std::endl;
  ASSERT_EQ(pl.getVehicleTypeOccupiedParking(component::VehicleType::CYCLE), 0)
      << "Incorrect Parking slot addition" << std::endl;
  ASSERT_EQ(pl.getVehicleTypeOccupiedParking(component::VehicleType::MINIVAN),
            0)
      << "Incorrect Parking slot addition" << std::endl;
  ASSERT_EQ(
      pl.getVehicleTypeOccupiedParking(component::VehicleType::MOTORCYCLE), 0)
      << "Incorrect Parking slot addition" << std::endl;

  ASSERT_EQ(pl.getAvailableParking(), 1)
      << "Parking slot must be available" << std::endl;
  ASSERT_EQ(pl.getOccupiedParking(), 0)
      << "No parking slot should be occupied" << std::endl;
  ASSERT_EQ(pl.getParkingSlot(component::VehicleType::CYCLE).isOk(), false)
      << "No Cycle Slot is available, Should not provide parking." << std::endl;

  auto assigned_slot = pl.getParkingSlot(component::VehicleType::CAR);
  ASSERT_EQ(assigned_slot.isOk(), true)
      << "Car parking slot should be available" << std::endl;
  ASSERT_EQ(assigned_slot.getData().getParkingSlotId(), slot.getParkingSlotId())
      << "Incorrect Slot assigned" << std::endl;
  ASSERT_EQ(pl.getAvailableParking(), 0)
      << "Parking slot must not be available" << std::endl;
  ASSERT_EQ(pl.getOccupiedParking(), 1)
      << "Parking slot must be occupied" << std::endl;

  ASSERT_EQ(pl.getVehicleTypeAvailableParking(component::VehicleType::CAR), 0)
      << "Incorrect Parking slot fetch" << std::endl;
  ASSERT_EQ(pl.getVehicleTypeAvailableParking(component::VehicleType::CYCLE), 0)
      << "Incorrect Parking slot fetch" << std::endl;
  ASSERT_EQ(pl.getVehicleTypeAvailableParking(component::VehicleType::MINIVAN),
            0)
      << "Incorrect Parking slot fetch" << std::endl;
  ASSERT_EQ(
      pl.getVehicleTypeAvailableParking(component::VehicleType::MOTORCYCLE), 0)
      << "Incorrect Parking slot fetch" << std::endl;

  ASSERT_EQ(pl.getVehicleTypeOccupiedParking(component::VehicleType::CAR), 1)
      << "Incorrect Parking slot fetch" << std::endl;
  ASSERT_EQ(pl.getVehicleTypeOccupiedParking(component::VehicleType::CYCLE), 0)
      << "Incorrect Parking slot fetch" << std::endl;
  ASSERT_EQ(pl.getVehicleTypeOccupiedParking(component::VehicleType::MINIVAN),
            0)
      << "Incorrect Parking slot fetch" << std::endl;
  ASSERT_EQ(
      pl.getVehicleTypeOccupiedParking(component::VehicleType::MOTORCYCLE), 0)
      << "Incorrect Parking slot fetch" << std::endl;

  pl.returnParkingSlot(assigned_slot.getData());
  ASSERT_EQ(pl.getAvailableParking(), 1)
      << "Parking slot must be available" << std::endl;
  ASSERT_EQ(pl.getOccupiedParking(), 0)
      << "No parking slot should be occupied" << std::endl;
  ASSERT_EQ(pl.getVehicleTypeAvailableParking(component::VehicleType::CAR), 1)
      << "Incorrect Parking slot return" << std::endl;
  ASSERT_EQ(pl.getVehicleTypeAvailableParking(component::VehicleType::CYCLE), 0)
      << "Incorrect Parking slot return" << std::endl;
  ASSERT_EQ(pl.getVehicleTypeAvailableParking(component::VehicleType::MINIVAN),
            0)
      << "Incorrect Parking slot return" << std::endl;
  ASSERT_EQ(
      pl.getVehicleTypeAvailableParking(component::VehicleType::MOTORCYCLE), 0)
      << "Incorrect Parking slot return" << std::endl;

  ASSERT_EQ(pl.getVehicleTypeOccupiedParking(component::VehicleType::CAR), 0)
      << "Incorrect Parking slot return" << std::endl;
  ASSERT_EQ(pl.getVehicleTypeOccupiedParking(component::VehicleType::CYCLE), 0)
      << "Incorrect Parking slot return" << std::endl;
  ASSERT_EQ(pl.getVehicleTypeOccupiedParking(component::VehicleType::MINIVAN),
            0)
      << "Incorrect Parking slot return" << std::endl;
  ASSERT_EQ(
      pl.getVehicleTypeOccupiedParking(component::VehicleType::MOTORCYCLE), 0)
      << "Incorrect Parking slot return" << std::endl;
}

TEST(ParkingIdParser, ParkingIdParseAPI) {
  component::ParkingSlot slot = component::makeParkingSlot("2_CA_A_3");
  ASSERT_EQ(slot.getParkingSlotId(), "2_CA_A_3")
      << "Incorrect slot id" << std::endl;
  ASSERT_EQ(slot.getParkingLevel(), 2)
      << "Incorrect parking level" << std::endl;
  ASSERT_EQ(slot.getVehicleType(), component::VehicleType::CAR)
      << "Incorrect vehicle type" << std::endl;
  ASSERT_EQ(slot.isOccupied(), false) << "Must be available" << std::endl;
}

TEST(Dumping, ParkingLotDump) {
  component::ParkingLot lot("Park", 3);
  lot.addParking("0_MV_A_1");
  lot.addParking("0_CA_A_2");
  lot.addParking("0_MC_A_3");
  lot.addParking("0_CY_A_4");

  lot.addParking("1_MV_A_1");
  lot.addParking("1_CA_A_2");
  lot.addParking("1_MC_A_3");
  lot.addParking("1_CY_A_4");

  lot.addParking("2_MV_A_1");
  lot.addParking("2_CA_A_2");
  lot.addParking("2_MC_A_3");
  lot.addParking("2_CY_A_4");
  std::fstream file("Debug.log", std::ios::out);
  utils::Dumper d(file);
  std::cout << lot << std::endl;
  file.close();
}