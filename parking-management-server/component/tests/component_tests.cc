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
  auto time = std::time(nullptr);

  slot.setParkingTime(time);

  ASSERT_EQ(slot.isOccupied(), true)
      << "Parking Slot should be occupied" << std::endl;
  ASSERT_EQ(slot.getParkingTime().getData(), time)
      << "Parking Slot time isnt set properly" << std::endl;

  slot.setOccupied(false);

  ASSERT_EQ(slot.getParkingTime().isOk(), false)
      << "Parking Slot time should be unavailable" << std::endl;
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

TEST(ParkingLot, ParkingLotAPI) {
  component::ParkingLot parkinglot("Tejas", 2);
  parkinglot.deleteParkingSlots();
  parkinglot.addParking("1_CA_0_0");
  parkinglot.addParking("1_CA_0_1");
  parkinglot.addParking("1_CA_0_2");
  parkinglot.addParking("1_CA_0_3");

  ASSERT_EQ(parkinglot.getTotalAvailableParking(), 4)
      << "Incorrect available count" << std::endl;
  ASSERT_EQ(parkinglot.getAvailableParkingAtLevel(0), 0)
      << "Incorrect available count for level 0" << std::endl;
  ASSERT_EQ(parkinglot.getAvailableParkingAtLevel(1), 4)
      << "Incorrect available count for level 1" << std::endl;
  ASSERT_EQ(
      parkinglot.getAvailableParkingForVehicleType(component::VehicleType::CAR),
      4)
      << "Incorrect available count for vehicle" << std::endl;
  ASSERT_EQ(parkinglot.getAvailableParkingForVehicleTypeAtLevel(
                0, component::VehicleType::CAR),
            0)
      << "Incorrect available count for level 0 and car" << std::endl;
  ASSERT_EQ(parkinglot.getAvailableParkingForVehicleTypeAtLevel(
                1, component::VehicleType::CAR),
            4)
      << "Incorrect available count for level 1 and car" << std::endl;

  ASSERT_EQ(parkinglot.getTotalOccupiedParking(), 0)
      << "Incorrect occupied count" << std::endl;
  ASSERT_EQ(parkinglot.getOccupiedParkingAtLevel(0), 0)
      << "Incorrect occupied count for level 0" << std::endl;
  ASSERT_EQ(parkinglot.getOccupiedParkingAtLevel(1), 0)
      << "Incorrect occupied count for level 1" << std::endl;
  ASSERT_EQ(
      parkinglot.getOccupiedParkingForVehicleType(component::VehicleType::CAR),
      0)
      << "Incorrect occupied count for vehicle" << std::endl;
  ASSERT_EQ(parkinglot.getOccupiedParkingForVehicleTypeAtLevel(
                0, component::VehicleType::CAR),
            0)
      << "Incorrect occupied count for level 0 and car" << std::endl;
  ASSERT_EQ(parkinglot.getOccupiedParkingForVehicleTypeAtLevel(
                1, component::VehicleType::CAR),
            0)
      << "Incorrect occupied count for level 1 and car" << std::endl;

  auto slot = parkinglot.getParking(component::VehicleType::CAR);
  ASSERT_EQ(slot.isOk(), true) << "Unable to fetch a slot" << std::endl;

  auto result = parkinglot.getParkingSlot(slot.getData().getParkingSlotId());
  ASSERT_EQ(result.isOk(), true) << "Unable to fetch a slot" << std::endl;
  ASSERT_EQ(result.getData().isOccupied(), true)
      << "DB update issue" << std::endl;
  ASSERT_EQ(parkinglot.getTotalAvailableParking(), 3)
      << "Incorrect available count" << std::endl;
  ASSERT_EQ(parkinglot.getTotalOccupiedParking(), 1)
      << "Incorrect occupied count" << std::endl;

  parkinglot.returnParking(slot.getData());

  result = parkinglot.getParkingSlot(slot.getData().getParkingSlotId());
  ASSERT_EQ(result.isOk(), true) << "Unable to fetch a slot" << std::endl;
  ASSERT_EQ(result.getData().isOccupied(), false)
      << "DB update issue" << std::endl;
  ASSERT_EQ(parkinglot.getTotalAvailableParking(), 4)
      << "Incorrect available count" << std::endl;
  ASSERT_EQ(parkinglot.getTotalOccupiedParking(), 0)
      << "Incorrect occupied count" << std::endl;
}