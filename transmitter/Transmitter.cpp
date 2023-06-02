#include "Transmitter.hpp"

const byte addresses[][6] = {"1Node", "2Node"};
const int connectionLed = 2;

Transmitter::Transmitter() : radio(RF24(9, 10)) {}

void Transmitter::Setup() {
  sendBuffer = (uint8_t*)malloc(BUF_SIZE);
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
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1, addresses[1]);
  radio.powerUp();
  radio.startListening();
}

void Transmitter::Loop() {
  connectionStatusIndex++;
  if (connectionStatusIndex >= CONNECTION_STATUS_SIZE) {
    connectionStatusIndex = 0;
  }

  if (!radio.available()) {
    Serial.println(F("No connection"));
    // Warning for no connection (Worse connection => faster blinking)
    digitalWrite(connectionLed, HIGH);

    connectionStatuses[connectionStatusIndex] = false;
    return;
  }

  // Main code
  ReceiveStatus();

  Sync();

  ReceiveCommand();

  Command command = static_cast<Command>(apiCommand.command);

  if (command == Command::Sync)
    SaveWaypoints();
  else
    SendCommand(command);

  // Reset connection led as we got through the loop
  digitalWrite(connectionLed, LOW);
  connectionStatuses[connectionStatusIndex] = true;
}

void Transmitter::Sync() {}

void Transmitter::ReceiveCommand() {
  int size = Serial.available();

  if (size == 0) {
    return;
  }

  uint8_t* buffer = (uint8_t*)malloc(size);
  Serial.readBytes(buffer, size);

  unpacker.feed(buffer, size);
  unpacker.deserialize(apiCommand);

  free(buffer);
}

void Transmitter::SaveWaypoints() {
  // apiCommand.waypoints.
}

void Transmitter::SendCommand(Command command) {
  radio.stopListening();

  sendBuffer[0] = apiCommand.command;

  switch (command) {
    case Command::Sync:
      /*uint8_t index = recvBuffer[1];
  float latitude = recvBuffer[2];
  float longitude = recvBuffer[6];*/
      break;
    case Command::ManualMode:
      pilotMode = false;
      break;
    case Command::PilotMode:
      pilotMode = true;
      break;
    case Command::Home:
      break;
    case Command::Move:
      break;
  }

  radio.write(sendBuffer, BUF_SIZE);

  radio.startListening();
}

void Transmitter::ReceiveStatus() {
  radio.read(&boatStatus, sizeof(Status));

  fullStatus.speed = boatStatus.speed;
  fullStatus.heading = boatStatus.heading;
  fullStatus.latitude = boatStatus.latitude;
  fullStatus.longitude = boatStatus.longitude;
  fullStatus.battery = boatStatus.battery;
  fullStatus.timeRemaining = boatStatus.timeRemaining;
  fullStatus.motorPower = boatStatus.motorPower;
  fullStatus.motorTemperature = boatStatus.motorTemperature;
  fullStatus.connectionQuality = ConnectionQuality();

  packer.serialize(fullStatus);
  Serial.write(packer.data(), packer.size());
}

uint8_t Transmitter::ConnectionQuality() {
  uint8_t total = 0;
  for (size_t i = 0; i < CONNECTION_STATUS_SIZE; i++) {
    total += connectionStatuses[i];
  }

  return total / CONNECTION_STATUS_SIZE;
}

void FullStatus::to_msgpack(MsgPack::Packer& packer) const {
  packer.to_map(key_speed, speed, key_heading, heading, key_latitude, latitude,
                key_longitude, longitude, key_battery, battery,
                key_timeRemaining, timeRemaining, key_motorPower, motorPower,
                key_motorTemperature, motorTemperature, key_connectionQuality,
                connectionQuality);
}

void FullStatus::from_msgpack(MsgPack::Unpacker& unpacker) {
  unpacker.from_map(key_speed, speed, key_heading, heading, key_latitude,
                    latitude, key_longitude, longitude, key_battery, battery,
                    key_timeRemaining, timeRemaining, key_motorPower,
                    motorPower, key_motorTemperature, motorTemperature,
                    key_connectionQuality, connectionQuality);
}

void ApiCommand::to_msgpack(MsgPack::Packer& packer) const {
  packer.to_map(key_command, command, key_waypoints, waypoints);
}

void ApiCommand::from_msgpack(MsgPack::Unpacker& unpacker) {
  unpacker.from_map(key_command, command, key_waypoints, waypoints);
}