#include "EepromDev.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <QDebug>

const char* EepromDev::I2CBus = "/dev/i2c-2";

EepromDev::EepromDev()
{
    memset(&data_, 0xff, sizeof(data_));
    memcpy(data_.signature, NOVENA_SIGNATURE, sizeof(data_.signature));
    data_.version = NOVENA_VERSION;
}

bool EepromDev::openDevice(int& fd)
{
    fd = open(I2CBus, O_RDWR);

    if (fd < 0) {
        error_ = "Failed to open I2C bus: ";
        error_.append(strerror(errno));
        return false;
    }
    return true;
}

bool EepromDev::isValidData(void)
{
    if (memcmp(data_.signature, NOVENA_SIGNATURE, sizeof(data_.signature))) {
        qDebug() << "No signature found";
        return false;
    }

    if ((data_.version != 1) && (data_.version != 2)) {
        qDebug() << "Unrecognized version number";
        return false;
    }

    qDebug() << "EEPROM contains valid data";
    return true;
}

void EepromDev::loadFromDefaults(void)
{
    memset(&data_, 0, sizeof(data_));

    memcpy(data_.signature, NOVENA_SIGNATURE, sizeof(data_.signature));

    memset(&data_.mac, 0xff, sizeof(data_.mac));

    data_.version = NOVENA_VERSION;

    data_.features = feature_es8328 | feature_pcie | feature_gbit |
                     feature_hdmi | feature_eepromoops;

    data_.eepromoops_offset = 4096;
    data_.eepromoops_length = 61440;

    data_.eeprom_size = 65536;
    data_.page_size = 128;

    data_.lvds1.frequency = 148500000;
    data_.lvds1.hactive = 1920;
    data_.lvds1.vactive = 1080;
    data_.lvds1.hback_porch = 148;
    data_.lvds1.hfront_porch = 88;
    data_.lvds1.hsync_len = 44;
    data_.lvds1.vback_porch = 36;
    data_.lvds1.vfront_porch = 4;
    data_.lvds1.vsync_len = 5;
    data_.lvds1.flags = vsync_polarity | hsync_polarity | data_width_8bit
                      | mapping_jeida | dual_channel | channel_present;

    data_.lvds2.flags = channel_present;

    /* Pull HDMI settings from e.g. EDID */
    data_.hdmi.flags = channel_present | ignore_settings | data_width_8bit;

}

bool EepromDev::loadFromDevice(void)
{
    bool ret_value = false;
    int fd;
    if (openDevice(fd)) {
        ret_value = readI2C(fd, 0, &data_, sizeof(data_));
        ::close(fd);
    }

    if (!isValidData())
        loadFromDefaults();

    return ret_value;
}

bool EepromDev::saveToDevice(void)
{
    bool ret_value = false;
    int fd;
    if (openDevice(fd)) {
        const char *buffer = (const char *)&data_;
        unsigned int buffer_offset = 0;
        int page_size = data_.page_size;
        bool ret;

        while (buffer_offset < sizeof(data_)) {
            if ((buffer_offset + page_size) > sizeof(data_))
                page_size = sizeof(data_) - buffer_offset;

            ret = writeI2C(fd, buffer_offset, (void *)(buffer + buffer_offset),
                            page_size);
            if (!ret) {
                break;
            }

            buffer_offset += page_size;
        }

        ::close(fd);
    }
    else
        ret_value = -1;

    return ret_value;
}

bool EepromDev::readI2C(int fd, int addr, void *data, int count)
{
    struct i2c_rdwr_ioctl_data session;
    struct i2c_msg messages[2];
    char set_addr_buf[2];

    memset(set_addr_buf, 0, sizeof(set_addr_buf));
    memset(data, 0, count);

    set_addr_buf[0] = addr >> 8;
    set_addr_buf[1] = addr;

    messages[0].addr = EepromAddress;
    messages[0].flags = 0;
    messages[0].len = sizeof(set_addr_buf);
    messages[0].buf = set_addr_buf;

    messages[1].addr = EepromAddress;
    messages[1].flags = I2C_M_RD;
    messages[1].len = count;
    messages[1].buf = (char *) data;

    session.msgs = messages;
    session.nmsgs = 2;

    if (ioctl(fd, I2C_RDWR, &session) < 0)  {
        error_ = "Unable to communicate with i2c device";
        error_.append(": ");
        error_.append(strerror(errno));
        return false;
    }

    return true;
}

bool EepromDev::writeI2C(int fd, int addr, void *data, int count)
{
    struct i2c_rdwr_ioctl_data session;
    struct i2c_msg messages[1];
    char data_buf[2 + count];

    data_buf[0] = addr >> 8;
    data_buf[1] = addr;
    memcpy(&data_buf[2], data, count);

    messages[0].addr = EepromAddress;
    messages[0].flags = 0;
    messages[0].len = sizeof(data_buf);
    messages[0].buf = data_buf;

    session.msgs = messages;
    session.nmsgs = 1;

    if (ioctl(fd, I2C_RDWR, &session) < 0) {
        error_ = "Unable to communicate with i2c device: ";
        error_.append(strerror(errno));
        qDebug() << "Unable to communicate with i2c device: " << strerror(errno);
        return false;
    }

    usleep(50000);
    return true;
}
