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

    static const int EepromAddress = (0xac>>1);
    static const char* I2CBus;// = "/dev/i2c-2";

    enum Features {
        ES8328_F = 0x0001,
        Senoko_F = 0x0002,
        Pcie_F = 0x0010,
        Gbit_F = 0x0020,
        EepromOops_F = 0x0080
    };

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
