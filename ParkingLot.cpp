#pragma warning(disable : 4996)
#include "class.h"
#include <Windows.h>
#include <conio.h>
#include <iomanip>
#include <iostream>
#include <string>
#include <time.h>
using namespace std;

/*类的声明*/
class Car;
class Lane;
class ParkingLot;

/*函数声明*/
void MainOps(Lane *lane, ParkingLot *park);

/*基本函数*/
//获取系统当前时间
tm *CurrentTime() {
  time_t current_time = time(NULL);
  tm *t_tm = localtime(&current_time);
  return t_tm;
}

string time2str(tm *time) {
  string timestr;
  timestr = to_string(time->tm_year + 1900);
  timestr += "-";
  timestr += to_string(time->tm_mon);
  timestr += "-";
  timestr += to_string(time->tm_mday);
  timestr += " ";
  timestr += to_string(time->tm_hour);
  timestr += ":";
  timestr += to_string(time->tm_min);
  timestr += ":";
  timestr += to_string(time->tm_sec);
  return timestr;
}

string duration2str(int dur) {
  string durationstr;
  int day = dur / 86400;
  int hour = (dur % 86400) / 3600;
  int min = ((dur % 86400) % 3600) / 60;
  int sec = dur % 86400 % 3600 % 60;
  if (day) {
    durationstr = to_string(day);
    durationstr += " day(s),";
  }
  if (hour) {
    durationstr += to_string(hour);
    durationstr += " hour(s),";
  }
  if (min) {
    durationstr += to_string(min);
    durationstr += " minute(s),";
  }
  durationstr += to_string(sec);
  durationstr += " second(s)";
  return durationstr;
}

bool strExam(string str) {
  if (str.length() != 7)
    return false;
  else {
    if (!(str[0] >= 65 && str[0] <= 90))
      return false;
    else {
      if (!(str[1] >= 65 && str[1] <= 90))
        return false;
      else {
        for (int i = 2; i < 7; i++) {
          if (!(str[i] >= 48 && str[i] <= 57 || str[i] >= 65 && str[i] <= 90))
            return false;
        }
      }
    }
  }
  return true;
}

/*类函数*/
Car::Car(string carID) {
  this->id = carID;
  isArriving = true;
  time_arrive = new tm;
  time_in = new tm;
  *time_arrive = *CurrentTime();
  next = NULL;
}
void Car::Car2Queue() { *time_arrive = *CurrentTime(); }
void Car::Car2Park() {
  isArriving = false;
  *time_arrive = *CurrentTime();
  *time_in = *time_arrive;
}
double Car::Fee() {
  return 0.0833 * (mktime(CurrentTime()) - mktime(time_in)) /
         60.0; //计费标准: 5元/小时
}

ParkingLot::ParkingLot(int len) {
  this->len = len;
  cars = new Car *[len];
  buffer = new Car *[len];
  lastCar = -1;
}
bool ParkingLot::CarIn(string carID) {
  if (this->lastCar == len - 1)
    return false;
  else {
    Car *newCar = new Car(carID);
    this->cars[++lastCar] = newCar;
    newCar->pos_park = this->lastCar + 1;
    newCar->Car2Park();
    return true;
  }
}
bool ParkingLot::CarfromQueue(Car *newCar) {
  if (this->lastCar == len - 1)
    return false;
  else {
    this->cars[++lastCar] = newCar;
    newCar->pos_park = this->lastCar + 1;
    newCar->Car2Park();
    return true;
  }
}
bool ParkingLot::CarOut(string carID, double *fee) {
  Car *outCar = findCar(carID);
  if (outCar->id == "null") {
    NullWarning();
    return false;
  } else {
    *fee = outCar->Fee();
    int preLen = lastCar;
    int temp = this->lastCar - outCar->pos_park + 1;
    for (int i = 0; i < temp; i++) {
      buffer[i] = this->cars[lastCar];
      this->cars[lastCar--] = new Car;
    }
    delete cars[lastCar];
    for (int i = lastCar; i < preLen; i++) {
      cars[lastCar] = buffer[preLen - i - 1];
      cars[lastCar]->pos_park = lastCar++;
      buffer[preLen - i - 1] = new Car;
    }
    lastCar--;
    return true;
  }
}
Car *ParkingLot::findCar(string carID) {
  if (this->lastCar == -1) {
    Car *emptyCar = new Car("empty");
    return emptyCar;
  } else {
    for (int i = 0; i <= this->lastCar; i++) {
      if (cars[i]->id == carID)
        return cars[i];
    }
    Car *nullCar = new Car("null");
    return nullCar;
  }
}

int ParkingLot::getCarPosition(string carID) {
  if (this->lastCar == -1)
    return 0; //停车场为空
  else {
    int pos;
    for (pos = 0; pos <= this->lastCar; pos++) {
      if (this->cars[pos]->id == carID)
        return pos + 1;
    }
    return -1; //错误, 未找到该车
  }
}
void ParkingLot::getStatus() {
  if (this->lastCar == -1)
    cout << "The Parking Lot is Empty !" << endl;
  else {
    cout << "Total Number of Cars in Parking Lot: " << this->lastCar + 1 << endl
         << "Cars in Parking Lot:" << endl;
    cout << "Car ID" << setw(14) << "Arrival Time" << setw(23) << "Parked Time"
         << setw(24) << "Parking Fee" << endl;
    for (int i = 0; i <= lastCar; i++) {
      cout << cars[i]->id << "\t" << time2str(cars[i]->time_arrive) << "\t"
           << time2str(cars[i]->time_in) << "\t" << cars[i]->Fee() << endl;
    }
  }
}

void Lane::CarIn(string carID) {
  Car *newCar = new Car(carID);
  newCar->next = NULL;
  if (rear == NULL) {
    rear = newCar;
    front = newCar;
  } else {
    rear->next = newCar;
    rear = newCar;
    newCar->Car2Queue();
  }
}
bool Lane::Car2Park(ParkingLot *park) {
  if (front == NULL)
    return false;
  else {
    park->CarfromQueue(front);
    front = front->next;
    return true;
  }
}
Car *Lane::findCar(string carID) {
  if (front == NULL) {
    Car *emptyCar = new Car("empty");
    return emptyCar;
  } else {
    Car *temp = front;
    do {
      if (temp->id == carID)
        return temp;
      else
        temp = temp->next;
    } while (temp != NULL);
    Car *nullCar = new Car("null");
    return nullCar;
  }
}
int Lane::getLength() {
  int len = 0;
  Car *temp = this->front;
  if (temp == NULL)
    return len;
  else {
    len++;
    while (temp->next != NULL) {
      len++;
      temp = temp->next;
    }
    return len;
  }
}
int Lane::getCarPosition(string carID) {
  int pos = 0;
  Car *temp = this->front;
  if (temp == NULL)
    return 0; //队列为空
  else {
    pos++;
    while (temp->next != NULL) {
      if (temp->id == carID)
        return pos;
      pos++;
      temp = temp->next;
    }
    return -1; //错误, 未找到该车
  }
}
void Lane::getStatus() {
  if (this->front == NULL)
    cout << "The Queueing Lane is Empty !" << endl;
  else {
    Car *temp = front;
    cout << "Total Queueing Car Number: " << this->getLength() << endl
         << "Queueing Cars:" << endl;
    cout << "Car ID\tArrival Time\t" << endl;
    while (temp->next != NULL) {
      cout << temp->id << "\t" << time2str(temp->time_arrive) << endl;
      temp = temp->next;
    }
    cout << temp->id << "\t" << time2str(temp->time_arrive) << endl;
  }
}

int CarAllocate(string carID, Lane *lane, ParkingLot *park) {
  if (park->lastCar == park->len - 1) {
    lane->CarIn(carID);
    return 0;
  } else {
    park->CarIn(carID);
    return 1;
  }
}

void CarComing(Lane *lane, ParkingLot *park) {
  system("cls");
  cout << " - Car Coming - " << endl;
  cout << endl << "Please enter the id of car:" << endl;
  string carID;
  cin >> carID;
  if (!strExam(carID)) {
    cout << "Error: incorrect id format !" << endl;
    Sleep(15000);
  } else {
    if (park->findCar(carID)->id != "null" &&
        park->findCar(carID)->id != "empty")
      cout << "Error: Car with this id already in the Parking Lot !" << endl;
    else if (lane->findCar(carID)->id != "null" &&
             lane->findCar(carID)->id != "empty")
      cout << "Error: Car with this id already in the Queueing Lane !" << endl;
    else {
      if (CarAllocate(carID, lane, park)) {
        cout << "Car into Parking Lot !" << endl
             << "Parked Time:" << time2str(park->findCar(carID)->time_in)
             << endl
             << park->lastCar << "  car(s) ahead in the Parking Lot" << endl;
      } else {
        cout << "Parking Lot is full, Car into Queueing Lane !" << endl
             << "Arrival Time:" << time2str(lane->findCar(carID)->time_arrive)
             << endl
             << lane->getLength() - 1 << " car(s) ahead in the Queueing Lane"
             << endl;
      }
    }
  }
  Sleep(15000);
  MainOps(lane, park);
}

void CarLeaving(Lane *lane, ParkingLot *park) {
  system("cls");
  cout << " - Car Leaving - " << endl;
  cout << endl << "Please enter the id of car:" << endl;
  string carID;
  cin >> carID;
  if (!strExam(carID)) {
    cout << "Error: incorrect id format !" << endl;
  } else {
    if (park->findCar(carID)->id == "empty")
      cout << "Error: the Parking Lot is empty !" << endl;
    else if (park->findCar(carID)->id == "null") {
      if (lane->findCar(carID)->id == "null" ||
          lane->findCar(carID)->id == "empty")
        cout << "Error: car not found in the Parking Lot or the Queueing Lane !"
             << endl;
      else
        cout << "Error: car still in the Queueing Lane !" << endl;
    } else {
      double fee = 0;
      cout << "Car No." << carID << endl
           << "Parking Time: "
           << duration2str(mktime(CurrentTime()) -
                           mktime(park->findCar(carID)->time_in))
           << endl;
      park->CarOut(carID, &fee);
      cout << "Parking Fee: " << fee << endl
           << "Thank you for using the Parking Lot !" << endl;
      lane->Car2Park(park);
    }
  }
  Sleep(15000);
  MainOps(lane, park);
}

void SearchCarInfo(Lane *lane, ParkingLot *park) {
  system("cls");
  cout << " - Search Car Information - " << endl;
  cout << endl << "Please enter the id of car:" << endl;
  string carID;
  cin >> carID;
  if (!strExam(carID)) {
    cout << "Error: incorrect id format !" << endl;
  } else {
    if (park->findCar(carID)->id == "empty")
      cout << "Error: the Parking Lot is empty !" << endl;
    else if (park->findCar(carID)->id == "null") {
      if (lane->findCar(carID)->id == "null" ||
          lane->findCar(carID)->id == "empty")
        cout << "Error: car not found in the Parking Lot or the Queueing Lane !"
             << endl;
      else {
        cout << "Car No." << carID << " is in the Queueing Lane" << endl;
        cout << lane->getCarPosition(carID) - 1 << " car(s) ahead" << endl;
      }
    } else {
      cout << "Car No." << carID << " is in the Parking Lot" << endl
           << park->findCar(carID)->pos_park - 1 << " car(s) ahead" << endl
           << "Parking Time: "
           << duration2str(mktime(CurrentTime()) -
                           mktime(park->findCar(carID)->time_in))
           << endl;
      cout << "Current Parking Fee: " << park->findCar(carID)->Fee() << endl;
    }
  }
  Sleep(15000);
  MainOps(lane, park);
}

void MainOps(Lane *lane, ParkingLot *park) {
  system("cls");
  cout << "Welcome to Parking Lot Manager !" << endl;
  cout << endl << "Please Choose an Option:" << endl;
  cout << " -A- Car Coming" << endl;
  cout << " -B- Car Leaving" << endl;
  cout << " -C- Search Car Information" << endl;
  cout << " -D- Show Parking Lot Status" << endl;
  cout << " -E- Show Queueing Lane Status" << endl;
  cout << endl << endl << " -X- Exit" << endl;
  char ops;
  cin >> ops;
  switch (ops) {
  case 'A':
    CarComing(lane, park);
    break;
  case 'B':
    CarLeaving(lane, park);
    break;
  case 'C':
    SearchCarInfo(lane, park);
    break;
  case 'D':
    system("cls");
    cout << " - Parking Lot Status - " << endl << endl;
    park->getStatus();
    system("pause");
    MainOps(lane, park);
    break;
  case 'E':
    system("cls");
    cout << " - Queueing Lane Status - " << endl << endl;
    lane->getStatus();
    system("pause");
    MainOps(lane, park);
    break;
  case 'X':
    exit(0);
  default:
    cout << "Error: incorrect input !" << endl;
    Sleep(15000);
    MainOps(lane, park);
  }
}

int main() {
  int len;
Initialize:
  cout << "Please enter a length for the Parking Lot: " << endl;
  cin >> len;
  if (len <= 0) {
    cout << "Error: invalid length number !" << endl;
    Sleep(15000);
    system("cls");
    goto Initialize;
  }

  Lane lane;
  ParkingLot park(len);

  CarAllocate("CA48Z12", &lane, &park);
  CarAllocate("CA9F814", &lane, &park);
  CarAllocate("JB09C74", &lane, &park);
  CarAllocate("CAXB192", &lane, &park);
  CarAllocate("CBHM402", &lane, &park);
  CarAllocate("CHT28B0", &lane, &park);

  MainOps(&lane, &park);

  return 0;
}