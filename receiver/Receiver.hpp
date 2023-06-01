#pragma once

#include <Arduino.h>
#include <GPSArduinoCommon.h>

class Receiver {
 public:
  Receiver();

  void Setup();
  void Loop();

 private:
  void Logic();
  void Receive();
  void UpdateStatus();
  void Send();

 private:
  RF24 radio;
  uint8_t* recvBuffer = nullptr;
  Status boatStatus;
  Waypoint* waypoints;
  bool pilotMode = false;
  bool navigateHome = false;
  uint8_t joyX = 127;
  uint8_t joyY = 127;
  uint8_t lastWaypoint = -1;
  uint8_t nextWaypoint = -1;
};