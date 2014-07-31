#include "EepromDev.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>

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

bool EepromDev::loadFromDevice()
{
	bool ret_value = false;
	int fd;
	if (openDevice(fd)) {
		ret_value = readI2C(fd, 0, &data_, sizeof(data_));
		::close(fd);
	}

	return ret_value;
}

bool EepromDev::saveToDevice()
{
	bool ret_value = false;
	int fd;
	if (openDevice(fd)) {
		ret_value = writeI2C(fd, 0, &data_, sizeof(data_));
		::close(fd);
	}

	return ret_value;
}

bool EepromDev::readI2C(int fd, int addr, void *data, int count)
{
	struct i2c_rdwr_ioctl_data session;
	struct i2c_msg messages[2];
	uint8_t set_addr_buf[2];

	memset(set_addr_buf, 0, sizeof(set_addr_buf));
	memset(data, 0, count);

	set_addr_buf[0] = addr>>8;
	set_addr_buf[1] = addr;

	messages[0].addr = EepromAddress;
	messages[0].flags = 0;
	messages[0].len = sizeof(set_addr_buf);
	messages[0].buf = set_addr_buf;

	messages[1].addr = EepromAddress;
	messages[1].flags = I2C_M_RD;
	messages[1].len = count;
    messages[1].buf = (__u8*) data;

	session.msgs = messages;
	session.nmsgs = 2;

	if (ioctl(fd, I2C_RDWR, &session) < 0)	{
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
	uint8_t data_buf[2+count];

	data_buf[0] = addr>>8;
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
		return false;
	}

	return true;
}
