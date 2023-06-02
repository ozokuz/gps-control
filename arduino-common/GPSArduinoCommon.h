#pragma once
#include "Radio.hpp"

#define WAYPOINT_MAX_COUNT 100

enum class Command : uint8_t { Sync, ManualMode, PilotMode, Home, Move };

struct Status {
  float speed;
  uint8_t heading;
  float latitude;
  float longitude;
  uint8_t battery;
  uint8_t timeRemaining;
  uint8_t motorPower;
  uint8_t motorTemperature;
};

struct WaypointWithIndex {
  uint8_t index;
  float latitude, longitude;
};

struct Waypoint {
  float latitude, longitude;

  Waypoint(const WaypointWithIndex& waypoint)
      : latitude(waypoint.latitude), longitude(waypoint.longitude) {}
};

struct Joystick {
  uint8_t joyX = 127, joyY = 127;

  Joystick() {}

  Joystick(uint8_t x, uint8_t y) : joyX(x), joyY(y) {}

  Joystick(const Joystick& joystick)
      : joyX(joystick.joyX), joyY(joystick.joyY) {}
};
