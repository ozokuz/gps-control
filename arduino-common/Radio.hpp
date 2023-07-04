#pragma once

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CHANNEL 0x5F
#define BUF_SIZE 32

class Radio {
 public:
  void Init(bool isSender);
  bool HasData();
  void Read(void* data, uint8_t size);
  bool Write(const void* data, uint8_t size);

 private:
  RF24 radio;
};
