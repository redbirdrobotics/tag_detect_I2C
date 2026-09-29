ESP32-CAM APRILTAG SENSOR

Detects Standard 41h12 AprilTags using an AI Thinker ESP32-CAM.

Tag values:
0 = No relevant tag
1 = Tag 1
2 = Tag 2
3 = Tag 3

SETUP

Enable ONE reporting mode at the top of the code:

#define REPORT_SERIAL

or

#define REPORT_I2C

Serial mode prints the detected tag at 115200 baud.

I2C mode makes the ESP32-CAM an I2C slave at address 0x69.

I2C PINS

SDA = GPIO 14
SCL = GPIO 15

I2C USAGE

The master does not need to send a command. Simply request one byte
from address 0x69.

The returned byte is the current detected tag:

0x00 = No tag
0x01 = Tag 1
0x02 = Tag 2
0x03 = Tag 3

The ESP32-CAM continuously performs AprilTag detection. An I2C request
simply returns the most recently detected tag.
It takes 250ms to detect the tag. It takes a safe estimate of 100ms, plus 150ms of delay to avoid overheat.