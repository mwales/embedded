cmake_minimum_required(VERSION 2.8)
project(tester)

add_subdirectory(ArduinoForLinux)


include_directories(ArduinoForLinux)

add_executable(gpio_tester GpioTester.cpp)
add_executable(spi_tester SpiTester.cpp)

target_link_libraries(gpio_tester arduino)
target_link_libraries(spi_tester arduino)

INCLUDE_DIRECTORIES(ArduinoForLinux)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -Wall")

