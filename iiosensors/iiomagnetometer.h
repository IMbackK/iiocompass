#pragma once
#include <iostream>
#include <fstream>
#include "iiodevice.h"

namespace iio
{
	
class Magnetometer: public VectorDevice
{
private:
	double maxX = std::numeric_limits<double>::lowest();
	double minX = std::numeric_limits<double>::max();
	double maxY = std::numeric_limits<double>::lowest();
	double minY = std::numeric_limits<double>::max();
	double maxZ = std::numeric_limits<double>::lowest();
	double minZ = std::numeric_limits<double>::max();

public:
	Magnetometer() = default;
	Magnetometer(const std::string& device);
	static std::string findMagnetometer();
	bool openDevice(const std::string& device);
	double getHeading(const Vector& magnFrame, Vector accelFrame);
	Vector getFrame();
};

}

