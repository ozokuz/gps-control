#include "Receiver.hpp"

const uint8_t addresses[][6] = {"1Node", "2Node"};
const int connectionLed = 2;

Receiver::Receiver() : radio(RF24(9, 10)) {}

void Receiver::Setup() {
  recvBuffer = (uint8_t*)malloc(BUF_SIZE);
  waypoints = (Waypoint*)malloc(WAYPOINT_MAX_COUNT);

  Serial.begin(9600);
  if (!radio.begin()) {
    Serial.println(F("* Error: radio.begin failed *"));
    while (true) {}
  }

  // set settings
  radio.setDataRate(RF24_2MBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.setRetries(0, 15);
  radio.setChannel(CHANNEL);
  radio.enableDynamicPayloads();
  radio.setAutoAck(true);

  // complete initialization
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);
  radio.powerUp();
  radio.startListening();

  UpdateStatus();
}

void Receiver::Loop() {
  if (!radio.available()) {
    Serial.println(F("No connection"));
    // Warning for no connection (Worse connection => faster blinking)
    digitalWrite(connectionLed, HIGH);
    return;
  }

  // Main code
  Send();

  Receive();

  Logic();

  UpdateStatus();
  // Reset connection led as we got through the loop
  digitalWrite(connectionLed, LOW);
}

void Receiver::Logic() {}

void Receiver::Receive() {
  radio.read(recvBuffer, BUF_SIZE);

  Command cmd = static_cast<Command>(recvBuffer[0]);

  switch (cmd) {
    case Command::Sync:
      uint8_t index = recvBuffer[1];
      float latitude = recvBuffer[2];
      float longitude = recvBuffer[6];
      waypoints[index] = Waypoint(latitude, longitude);
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
      joyX = recvBuffer[1];
      joyY = recvBuffer[2];
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
  radio.stopListening();

  if (!radio.write(&boatStatus, sizeof(Status))) {
    Serial.println(F("Couldn't send status"));
  }

  radio.startListening();
}
