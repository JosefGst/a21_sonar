#include "a21_sonar/a21_sonar.h"

// init serial
void A21::begin(std::string port, int baudrate)
{
    my_serial.setPort(port);
    my_serial.setBaudrate(baudrate);
    serial::Timeout timeout = serial::Timeout::simpleTimeout(500);
    my_serial.setTimeout(timeout);

    my_serial.open();
    my_serial.flushInput();
    std::cout << "SERIAL OK!" << std::endl;
}

// high level range reading used in main node
uint16_t A21::read_range()
{
    my_serial.write(&SIGNAL, 1);
    std::string line = my_serial.read(4);
    for (int i = 0; i < line.size(); i++)
    {
        receive_hex[i] = uint8_t(line[i]);
    }
    return get_range();
}

// helper to get range data from the return hex
uint16_t A21::get_range()
{
    return (receive_hex[1] << 8) + receive_hex[2];
}
