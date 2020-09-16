#include <filesystem>
#include <cmath>
#include <iostream>

#include "iiomagnetometer.h"

using namespace iio;


Magnetometer::Magnetometer(const std::string& device)
{
	openDevice(device);
}

std::string Magnetometer::findMagnetometer()
{
	return Device::findByProps(std::vector<std::string>({"in_magn_x_raw", "in_magn_y_raw", "in_magn_z_raw"}));
}

bool Magnetometer::openDevice(const std::string& device)
{
	return VectorDevice::openDevice(device, "in_magn_x", "in_magn_y", "in_magn_z");
}

Vector Magnetometer::getFrame()
{
	Vector frame = VectorDevice::getFrame();
	
	if(frame.x > maxX)
		maxX = frame.x;
	else if (frame.x < minX)
		minX = frame.x;
	
	if(frame.y > maxY)
		maxY = frame.y;
	else if (frame.y < minY)
		minY = frame.y;
	
	if(frame.z > maxZ)
		maxZ = frame.z;
	else if (frame.z < minZ)
		minZ = frame.z;

	Vector norm = frame;
	norm.x = (frame.x - minX) / (maxX-minX) - 0.5;
	norm.y = (frame.y - minY) / (maxY-minY) - 0.5;
	norm.z = (frame.z - minZ) / (maxZ-minZ) - 0.5;
	return norm;
}

double Magnetometer::getHeading(const Vector& magnFrame, Vector accelFrame)
{
	double tmp = accelFrame.x;
	accelFrame.x = accelFrame.y;
	accelFrame.y = tmp;
	Vector projection = magnFrame-accelFrame*((magnFrame*accelFrame)/(accelFrame*accelFrame));
	std::cout<<projection.x<<" "<<projection.y<<" "<<projection.z<<std::endl; 
	return atan2(projection.y, projection.x);
}

