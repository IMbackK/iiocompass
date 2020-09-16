#pragma once
#include <string>
#include <fstream>
#include "iiodevice.h"

namespace iio
{

class Accelerometer: public VectorDevice
{
public:
	Accelerometer(const std::string& device);
	Accelerometer() = default;
	static std::string findAccellerometer();
	bool openDevice(const std::string& device);
};

}
