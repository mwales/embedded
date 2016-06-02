#include <iostream>
#include "ArduinoOnLinux.h"
#include "SPI.h"
#include <stdint.h>
#include <unistd.h>

using namespace Arduino;



int main()
{
   SPISettings settings(10000, MSBFIRST, SPI_MODE0);

   SPI.beginTransaction(settings);

   std::cout << "About to send 1 byte 0x55" << std::endl;
   uint8_t sendMe = 0x55;
   uint8_t recvMe = SPI.transfer(sendMe);

   sleep(5);

   std::cout << "About to send 2 bytes 0x0103" << std::endl;
   uint16_t sendMe16 = 0x0103;
   uint16_t recvMe16 = SPI.transfer(sendMe16);

}


