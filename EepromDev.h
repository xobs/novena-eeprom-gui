#ifndef EEPROMDEV_H
#define EEPROMDEV_H

#include "novena-eeprom.h"
#include <string>

class EepromDev
{
public:
    EepromDev();

    bool loadFromDevice(void);
    bool saveToDevice(void);

    static const int EepromAddress = (0xac >> 1);
    static const char* I2CBus;

    bool readI2C(int fd, int addr, void* data, int count);
    bool writeI2C(int fd, int addr, void *data, int count);
    novena_eeprom_data& data() { return data_; }
    const std::string& error() const { return error_; }

private:
    bool openDevice(int& fd);
    bool isValidData(void);
    void loadFromDefaults(void);

private:
    std::string error_;
    novena_eeprom_data data_;
};

#endif // EEPROMDEV_H
