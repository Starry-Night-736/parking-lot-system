#pragma once
#include <iostream>
#include <time.h>
using namespace std;

class Car {
public:
  string id;     //车牌号码
  int pos_park;  //车辆在停车场中的位置
  bool isArriving;
  bool isLeaving;
  tm *time_arrive; //到达时间
  tm *time_in;     //入场时间
  Car *next;       //次车指针, 用于便道停车管理

  Car() {}
  Car(string carID);
  void Car2Queue();            //汽车驶入队列
  void Car2Park();             //汽车驶入停车场
  double Fee(); //汽车离场时所需费用
};

//停车场类, 使用栈来实现
class ParkingLot {
public:
  int len;      //停车场长度
  Car **cars;   //车队栈, 长度为len
  Car **buffer; //车辆出场时的缓冲区, 长度为len
  int lastCar;  //车队栈顶

  ParkingLot(int len);
  bool CarIn(string carID);            //新来车辆进入停车场
  bool CarfromQueue(Car *newCar);      //队列首位车辆进入停车场
  bool CarOut(string carID, double *fee); //车辆离开停车场
  void FullWarning() {}                //停车场已满警告
  void NullWarning() {}                //未找到车辆警告
  Car *findCar(string carID);          //车辆查找
  int getCarPosition(string carID);    //获取车辆所在位置
  void getStatus(); //显示当前停车场状态(车辆信息)
};

//便道类, 使用队列链表来实现
class Lane {
public:
  Car *front; //便道首车指针
  Car *rear;  //便道尾车指针

  Lane() { rear = front = NULL; }
  void CarIn(string carID);         //新来车辆进入便道
  bool Car2Park(ParkingLot *park);  //首车进入停车场
  Car *findCar(string carID);       //车辆查找
  int getLength();                  //获取当前队列长度
  int getCarPosition(string carID); //获取车辆所在位置
  void getStatus();                 //显示当前便道状态(车辆信息)
};