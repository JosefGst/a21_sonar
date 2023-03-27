#ifndef A21_SONAR
#define A21_SONAR

#include "ros/ros.h"
#include "sensor_msgs/Range.h"

#include <string>
#include <iostream>
#include <chrono>

// OS Specific sleep
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "serial/serial.h"

class A21
{
private:
    uint8_t receive_hex[4] = {0};
    const uint8_t SIGNAL = 0x01;
    uint16_t get_range();

public:
    sensor_msgs::Range data;

    serial::Serial my_serial;
    void begin(std::string port, int baudrate);
    uint16_t read_range();
};

#endif
