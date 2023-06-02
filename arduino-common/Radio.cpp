#include "Radio.hpp"

const uint8_t addresses[][6] = {"1Node", "2Node"};

void Radio::Init(bool isSender) {
  if (!radio.begin(9, 10)) {
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
  radio.openWritingPipe(addresses[!isSender]);
  radio.openReadingPipe(1, addresses[isSender]);
  radio.powerUp();
  radio.startListening();
}

bool Radio::HasData() {
  return radio.available();
}

void Radio::Read(void * data, uint8_t size) {
  radio.read(data, BUF_SIZE);
}

bool Radio::Write(const void* data, uint8_t size) {
  radio.stopListening();

  bool ok = radio.write(data, size);

  radio.startListening();

  return ok;
}
