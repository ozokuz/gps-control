#include "Receiver.hpp"

const int connectionLed = 2;

Receiver::Receiver() {}

void Receiver::Setup() {
  recvBuffer = (uint8_t*)malloc(BUF_SIZE);
  waypoints = (Waypoint*)malloc(WAYPOINT_MAX_COUNT);

  Serial.begin(9600);

  radio.Init(false);

  UpdateStatus();
}

void Receiver::Loop() {
  if (!radio.HasData()) {
    Serial.println(F("No connection"));
    // Warning for no connection (Worse connection => faster blinking)
    digitalWrite(connectionLed, HIGH);
  }

  // Main code
  Send();

  if (radio.HasData()) {
    Receive();
  }

  Logic();

  UpdateStatus();
  // Reset connection led as we got through the loop
  digitalWrite(connectionLed, LOW);
}

void Receiver::Logic() {}

void Receiver::Receive() {
  radio.Read(recvBuffer, BUF_SIZE);

  Command cmd = static_cast<Command>(recvBuffer[0]);
  void* otherData = (void*)recvBuffer[1];

  switch (cmd) {
    case Command::Sync:
      WaypointWithIndex waypoint = *(WaypointWithIndex*)otherData;

      waypoints[waypoint.index] = Waypoint(waypoint);
      break;
    case Command::ManualMode:
      pilotMode = false;
      break;
    case Command::PilotMode:
      pilotMode = true;
      break;
    case Command::Home:
      navigateHome = true;
      break;
    case Command::Move:
      joystick = Joystick(*(Joystick*)otherData);
      break;
  }
}

void Receiver::UpdateStatus() {
  boatStatus.speed = 0;
  boatStatus.heading = 0;
  boatStatus.latitude = 60;
  boatStatus.longitude = 60;
  boatStatus.battery = 100;
  boatStatus.timeRemaining = 300;
  boatStatus.motorPower = 0;
  boatStatus.motorTemperature = 18;
}

void Receiver::Send() {
  if (!radio.Write(&boatStatus, sizeof(Status))) {
    Serial.println(F("Couldn't send status"));
  }
}
