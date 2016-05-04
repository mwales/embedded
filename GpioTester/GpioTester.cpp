// Add test code here
#include <iostream>
#include <unistd.h>
#include "GpioIo.h"

using namespace Arduino;

int main(int argc, char** argv)
{
   std::cout << "hello world" << std::endl;

   // Lets export GPIO 18
   GpioIo dog(18, OUTPUT);

   while(1)
   {
      //std::cout << "Turning on" << std::endl;
      dog.setLevel(HIGH);

      usleep(1000);

      //std::cout << "Turning off" << std::endl;
      dog.setLevel(LOW);

      usleep(1000);

   }

}
