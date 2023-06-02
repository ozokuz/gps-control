#pragma once
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CHANNEL 0x5F
#define BUF_SIZE 32
#define WAYPOINT_MAX_COUNT 100

enum class Command : uint8_t {
  Sync,
  ManualMode,
  PilotMode,
  Home,
  Move
};

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

struct Waypoint {
  float latitude, longitude;

  Waypoint(float latitude, float longitude) : latitude(latitude), longitude(longitude) {}
};

struct WaypointIndex {
  uint8_t size;
  float latitude, longitude;
};