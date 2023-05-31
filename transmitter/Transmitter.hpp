#pragma once

#include <Arduino.h>
#include <GPSArduinoCommon.h>
#include <MsgPack.h>

#define CONNECTION_STATUS_SIZE 20

struct FullStatus {
  MsgPack::str_t key_speed{"speed"};
  float speed;
  MsgPack::str_t key_rotation{"rotation"};
  uint8_t rotation;
  MsgPack::str_t key_latitude{"latitude"};
  float latitude;
  MsgPack::str_t key_longitude{"longitude"};
  float longitude;
  MsgPack::str_t key_battery{"battery"};
  uint8_t battery;
  MsgPack::str_t key_timeRemaining{"timeRemaining"};
  uint8_t timeRemaining;
  MsgPack::str_t key_motorPower{"motorPower"};
  uint8_t motorPower;
  MsgPack::str_t key_motorTemperature{"motorTemperature"};
  uint8_t motorTemperature;
  MsgPack::str_t key_connectionQuality{"connectionQuality"};
  uint8_t connectionQuality;

  void to_msgpack(MsgPack::Packer& packer) const;
  void from_msgpack(MsgPack::Unpacker& unpacker);
};

struct ApiCommand {
  MsgPack::str_t key_command{"command"};
  uint8_t command;
  MsgPack::str_t key_waypoints{"waypoints"};
  MsgPack::arr_t<MsgPack::arr_t<float>> waypoints;

  void to_msgpack(MsgPack::Packer& packer) const;
  void from_msgpack(MsgPack::Unpacker& unpacker);
};

class Transmitter {
 public:
  Transmitter();

  void Setup();
  void Loop();

 private:
  void Sync();
  void ReceiveCommand();
  void SaveWaypoints();
  void SendCommand(Command command);
  void ReceiveStatus();
  uint8_t ConnectionQuality();

 private:
  RF24 radio;

  uint8_t* sendBuffer = nullptr;
  int8_t connectionStatusIndex = -1;
  bool connectionStatuses[CONNECTION_STATUS_SIZE];
  Status boatStatus;
  FullStatus fullStatus;
  ApiCommand apiCommand;
  MsgPack::Packer packer;
  MsgPack::Unpacker unpacker;
  Waypoint* waypoints;
  uint8_t lastSentWaypoint = -1;
  bool pilotMode = false;
};