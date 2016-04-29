cmake_minimum_required (VERSION 2.6)
project (ladyada)

set(SOURCE_FILES example.cpp)

#add_subdirectory(Adafruit-GFX-Library)
add_subdirectory(Adafruit_SSD1306)

add_executable(ladyada ${SOURCE_FILES} example.cpp)

target_link_libraries(ladyada Adafruit_SSD1306)
INCLUDE(FindPkgConfig)

INCLUDE_DIRECTORIES(Adafruit_SSD1306 Adafruit_SSD1306/Adafruit-GFX-Library)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -Wall")

MESSAGE("Compiler flags=${CMAKE_CXX_FLAGS}")

