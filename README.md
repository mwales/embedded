# embedded
Arduino, Raspberry Pi, iMX6, Zynq, and other things headless and low power

Arduino CLI notes

I want to be able to use the Arduino libraries on a Raspberry Pi over ssh / mosh.  

Command line Arduino library available on github:  https://github.com/arduino/arduino-cli

Download the latest release.  Arduino-CLI binaries aren't universal, you
download for the arch of the host.

```
wget https://github.com/arduino/arduino-cli/releases/download/v1.2.2/arduino-cli_1.2.2_Linux_ARMv7.tar.gz
tar -xzvf arduino-cli_1.2.2_Linux_ARMv7.tar.gz
```

Installing support for new boards

```
./arduino-cli core search samd
./arduino-cli core install arduino:samd
```

Installing support for the Adafruit SAMD boards (Feather)

```
./arduino-cli core --additional-urls https://adafruit.github.io/arduino-board-index/package_adafruit_index.json search samd
./arduino-cli core --additional-urls https://adafruit.github.io/arduino-board-index/package_adafruit_index.json install adafruit:samd
```

List all the boards installed
```
./arduino-cli board listall
```

This should find the board connected, but it doesn't always work for me
```
./arduino-cli board list
```

My feather board:
adafruit:samd:adafruit_feather_m0


To compile my arduino project

```
cd project_directory
./arduino-cli compile -b adafruit:samd:adafruit_feather_m0
```

When using picocom on Linux with Arduino, the Arduino uses windows line endings:

```
picocom --omap lfcrlf -b 115200 /dev/ttyACM0
```


