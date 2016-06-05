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
   SPI.setBitOrder(MSBFIRST);

   while(true)
   {
      std::cout << "About to send 2 byte 0x1382 MSB First to cs0" << std::endl;
      SPI.setChipSelect(0);
      uint16_t sendMe = 0x1382;
      uint16_t recvMe = SPI.transfer(sendMe);

      sleep(5);

      std::cout << "About to send 2 bytes 0x0103 LSB First to cs1" << std::endl;
      SPI.setChipSelect(1);
      SPI.setBitOrder(LSBFIRST);
      uint16_t sendMe16 = 0x0103;
      uint16_t recvMe16 = SPI.transfer(sendMe16);

      sleep(5);
   }
}


