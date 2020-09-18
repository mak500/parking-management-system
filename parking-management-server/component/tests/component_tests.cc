#include <iostream>

#include "../../include/parking.hh"
#include "gtest/gtest.h"

TEST(ParkingSlot, ParkingSlotAPI) {
  component::ParkingSlot slot(3, "3A 1", component::VehicleType::Car);
  ASSERT_EQ(slot.getParkingLevel(), 3)
      << "Parking level is not set correctly" << std::endl;
  ASSERT_EQ(slot.getParkingSlotId(), std::string("3A 1"))
      << "Parking Slot Id is not set correctly" << std::endl;
  ASSERT_EQ(slot.getVehicleType(), component::VehicleType::Car)
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
  component::ParkingSlot slot(1, "1A 1", component::VehicleType::Car);

  pl.addParkingSlot(slot);
  ASSERT_EQ(pl.getVehicleTypeAvailableParking(component::VehicleType::Car), 1)
      << "Incorrect Parking slot addition" << std::endl;
  ASSERT_EQ(pl.getVehicleTypeAvailableParking(component::VehicleType::Cycle), 0)
      << "Incorrect Parking slot addition" << std::endl;
  ASSERT_EQ(pl.getVehicleTypeAvailableParking(component::VehicleType::Minivan),
            0)
      << "Incorrect Parking slot addition" << std::endl;
  ASSERT_EQ(
      pl.getVehicleTypeAvailableParking(component::VehicleType::Motorcycle), 0)
      << "Incorrect Parking slot addition" << std::endl;

  ASSERT_EQ(pl.getVehicleTypeOccupiedParking(component::VehicleType::Car), 0)
      << "Incorrect Parking slot addition" << std::endl;
  ASSERT_EQ(pl.getVehicleTypeOccupiedParking(component::VehicleType::Cycle), 0)
      << "Incorrect Parking slot addition" << std::endl;
  ASSERT_EQ(pl.getVehicleTypeOccupiedParking(component::VehicleType::Minivan),
            0)
      << "Incorrect Parking slot addition" << std::endl;
  ASSERT_EQ(
      pl.getVehicleTypeOccupiedParking(component::VehicleType::Motorcycle), 0)
      << "Incorrect Parking slot addition" << std::endl;

  ASSERT_EQ(pl.getAvailableParking(), 1)
      << "Parking slot must be available" << std::endl;
  ASSERT_EQ(pl.getOccupiedParking(), 0)
      << "No parking slot should be occupied" << std::endl;
  ASSERT_EQ(pl.getParkingSlot(component::VehicleType::Cycle).isOk(), false)
      << "No Cycle Slot is available, Should not provide parking." << std::endl;

  auto assigned_slot = pl.getParkingSlot(component::VehicleType::Car);
  ASSERT_EQ(assigned_slot.isOk(), true)
      << "Car parking slot should be available" << std::endl;
  ASSERT_EQ(assigned_slot.getData().getParkingSlotId(), slot.getParkingSlotId())
      << "Incorrect Slot assigned" << std::endl;
  ASSERT_EQ(pl.getAvailableParking(), 0)
      << "Parking slot must not be available" << std::endl;
  ASSERT_EQ(pl.getOccupiedParking(), 1)
      << "Parking slot must be occupied" << std::endl;

  ASSERT_EQ(pl.getVehicleTypeAvailableParking(component::VehicleType::Car), 0)
      << "Incorrect Parking slot fetch" << std::endl;
  ASSERT_EQ(pl.getVehicleTypeAvailableParking(component::VehicleType::Cycle), 0)
      << "Incorrect Parking slot fetch" << std::endl;
  ASSERT_EQ(pl.getVehicleTypeAvailableParking(component::VehicleType::Minivan),
            0)
      << "Incorrect Parking slot fetch" << std::endl;
  ASSERT_EQ(
      pl.getVehicleTypeAvailableParking(component::VehicleType::Motorcycle), 0)
      << "Incorrect Parking slot fetch" << std::endl;

  ASSERT_EQ(pl.getVehicleTypeOccupiedParking(component::VehicleType::Car), 1)
      << "Incorrect Parking slot fetch" << std::endl;
  ASSERT_EQ(pl.getVehicleTypeOccupiedParking(component::VehicleType::Cycle), 0)
      << "Incorrect Parking slot fetch" << std::endl;
  ASSERT_EQ(pl.getVehicleTypeOccupiedParking(component::VehicleType::Minivan),
            0)
      << "Incorrect Parking slot fetch" << std::endl;
  ASSERT_EQ(
      pl.getVehicleTypeOccupiedParking(component::VehicleType::Motorcycle), 0)
      << "Incorrect Parking slot fetch" << std::endl;

  pl.returnParkingSlot(assigned_slot.getData());
  ASSERT_EQ(pl.getAvailableParking(), 1)
      << "Parking slot must be available" << std::endl;
  ASSERT_EQ(pl.getOccupiedParking(), 0)
      << "No parking slot should be occupied" << std::endl;
  ASSERT_EQ(pl.getVehicleTypeAvailableParking(component::VehicleType::Car), 1)
      << "Incorrect Parking slot return" << std::endl;
  ASSERT_EQ(pl.getVehicleTypeAvailableParking(component::VehicleType::Cycle), 0)
      << "Incorrect Parking slot return" << std::endl;
  ASSERT_EQ(pl.getVehicleTypeAvailableParking(component::VehicleType::Minivan),
            0)
      << "Incorrect Parking slot return" << std::endl;
  ASSERT_EQ(
      pl.getVehicleTypeAvailableParking(component::VehicleType::Motorcycle), 0)
      << "Incorrect Parking slot return" << std::endl;

  ASSERT_EQ(pl.getVehicleTypeOccupiedParking(component::VehicleType::Car), 0)
      << "Incorrect Parking slot return" << std::endl;
  ASSERT_EQ(pl.getVehicleTypeOccupiedParking(component::VehicleType::Cycle), 0)
      << "Incorrect Parking slot return" << std::endl;
  ASSERT_EQ(pl.getVehicleTypeOccupiedParking(component::VehicleType::Minivan),
            0)
      << "Incorrect Parking slot return" << std::endl;
  ASSERT_EQ(
      pl.getVehicleTypeOccupiedParking(component::VehicleType::Motorcycle), 0)
      << "Incorrect Parking slot return" << std::endl;
}