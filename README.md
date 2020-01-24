# Parking-Lot-Management-System
## Description
This project is a management system designed for parking lot entrance which also serves as a coding practice for “Data Structure” course.

The main management area is divided into the Lane (waiting area) and the Parking Lot (parking area). 

The parking rules are as follows:
* The maximum length of the Parking Lot is **100**, length of the Lane is **UNLIMITED**
* Cars are firstly assigned to the Parking Lot if not full
* If the Parking Lot is full, cars are then assigned to the Lane
* Cars in the Parking Lot are organized similarly to a “stack”, obeying the “LIFO” rule
* Car number information is recorded as XYZZZZZ (X, Y - capital letters; Z - Arabic numbers or capital letters), i.e. CA48Z12
* All cars ::in the Parking Lot:: will be charged ¥5 per hour

## Functions
Functions of the system include:
* Add cars into the Lane or Parking Lot and returns their current position
* Show current situation of the Lane and Parking Lot
* Display parking fees when cars leave
* Search for car information in the Lane or Parking Lot

## Building Environment
The project has been tested in the following environments:
* Visual Studio 2019 Community on Windows 10
* Xcode 11.3.1 on macOS Catalina 10.15.2
