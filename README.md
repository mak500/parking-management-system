# Parking Management System {#mainpage}
This project implements a simple parking management system. It contains
a server and a client.

## Features
Client Side
* Ability to view parking statistics (Total/Available/Occupied). Per level and
  per vehicle type
* Request for a parking slot
* Able to make the payment

## Parking Management Server
This is a parking management server. This implements all the services that the parking management system will offer. These services can be classified into parking space manangement and payment gateway.

### Heirarchy of Parking system
`ParkingLot` class contains an array of `ParkingLevel` whose size is determined via the levels of parking associated with the parking. `ParkingLevel` is made of `ParkingSlot` which contains the information about the parking. `ParkingManager` has `ParkingLot`.