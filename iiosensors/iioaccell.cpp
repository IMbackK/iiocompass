#include <filesystem>
#include <iostream>

#include "iioaccell.h"

using namespace iio;

Accelerometer::Accelerometer(const std::string& device)
{
	openDevice(device);
}

bool Accelerometer::openDevice(const std::string& device)
{
	return VectorDevice::openDevice(device, "in_accel_x", "in_accel_y", "in_accel_z");
}

std::string Accelerometer::findAccellerometer()
{
	return findByProps(std::vector<std::string>({"in_accel_x_raw", "in_accel_y_raw", "in_accel_z_raw"}));
}

