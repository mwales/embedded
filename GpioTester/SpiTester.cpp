#include <iostream>
#include "ArduinoOnLinux.h"
#include "SPI.h"
#include <stdint.h>
#include <unistd.h>

using namespace Arduino;



int main()
{
   SPISettings settings(1000, LSBFIRST, SPI_MODE0);

   SPI.beginTransaction(settings);
   SPI.setBitOrder(MSBFIRST);

   while(true)
   {
      std::cout << "About to send 2 byte 0x1382 MSB First to cs0 mode0" << std::endl;
      SPI.setBitOrder(MSBFIRST);
      SPI.setDataMode(SPI_MODE0);
      uint16_t sendMe = 0x1382;
      uint16_t recvMe = SPI.transfer(sendMe);

      sleep(5);

      std::cout << "About to send 2 bytes 0x0103 LSB First to cs1 mode1" << std::endl;
      SPI.setDataMode(SPI_MODE1);
      SPI.setBitOrder(LSBFIRST);
      uint16_t sendMe16 = 0x1382;
      uint16_t recvMe16 = SPI.transfer(sendMe16);

      sleep(5);

      std::cout << "About to send 2 byte 0x1382 MSB First to cs0 mode2" << std::endl;
      SPI.setDataMode(SPI_MODE2);
      SPI.setBitOrder(MSBFIRST);
      recvMe = SPI.transfer(sendMe);

      sleep(5);

      std::cout << "About to send 2 bytes 0x0103 LSB First to cs1 mode3" << std::endl;
      SPI.setDataMode(SPI_MODE3);
      SPI.setBitOrder(LSBFIRST);
      recvMe16 = SPI.transfer(sendMe16);

      sleep(5);
   }
}


