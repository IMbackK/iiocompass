#include <filesystem>
#include <iostream>
#include "iiodevice.h"

using namespace iio;

double Device::readFile(const std::string& fileName, bool& status)
{
	std::fstream file;
	file.open(fileName, std::ios_base::in);
	if(!file.is_open())
	{
		status = false;
		return 0;
	}
	double result = readFile(file, status);
	file.close();
	return result;
}

double Device::readFile(std::fstream& file, bool& status)
{
	double result = 0;
	file.seekg(std::ios_base::beg);
	file >> result;
	status = !file.rdstate();
	return result;
}

std::string Device::findByProps(const std::vector<std::string>& props)
{
	try
	{
		for(const auto & entry : std::filesystem::directory_iterator(IIO_DIRECTORY))
		{
			if(entry.is_directory())
			{
				bool match = true;
				for(const auto& matchName : props)
				{
					if(!std::filesystem::exists(entry.path() / matchName))
						match = false;
				}
				if(match) return entry.path();
			}
		}
	}
	catch(...)
	{
		//pass
	}
	
	return "";
}


bool Device::setRate(unsigned int rate)
{
	std::fstream availabeRatesFile;
	availabeRatesFile.open(deviceDir + "/sampling_frequency_available", std::ios_base::in);
	if(!availabeRatesFile.is_open()) return false;

	std::fstream rateFile;
	rateFile.open(deviceDir + "/sampling_frequency", std::ios_base::out);
	if(!rateFile.is_open())
	{
		availabeRatesFile.close();
		return false;
	}

	int curRate=-1;
	while(availabeRatesFile >> curRate)
	{
		std::cout<<"available rate "<<curRate<<'\n';
		if(curRate >= static_cast<int>(rate))
		{
			std::cout<<"closest available rate "<<curRate<<'\n';
			rateFile<<curRate;
			break;
		}
	}
	availabeRatesFile.close();
	rateFile.close();
	return true;
}

int Device::getRate()
{
	bool status = false;
	int rate = readFile(deviceDir + "/rate", status);
	if(status) return rate;
	else return -1;
}


bool VectorDevice::openDevice(const std::string device, const std::string xName, const std::string yName, const std::string zName)
{
	deviceDir = device;

	fileX.open(deviceDir + "/" + xName+"_raw", std::ios_base::in);
	if(!fileX.is_open())
	{
		std::cerr<<xName<<"_raw is required but not availble\n";
		return false;
	}

	fileY.open(deviceDir + "/" + yName+"_raw", std::ios_base::in);
	if(!fileY.is_open())
	{
		std::cerr<<yName<<"_raw is required but not availble\n";
		return false;
	}
	
	fileZ.open(deviceDir + "/" + zName+"_raw", std::ios_base::in);
	if(!fileZ.is_open())
	{
		std::cerr<<zName<<"_raw is required but not availble\n";
		return false;
	}
	
	bool status = false;
	
	xScale = readFile(deviceDir + "/" + xName+"_scale", status);
	if(!status)
	{
		xScale = 1;
		std::cerr<<"Device dosent have x scale assuming 1\n";
	}
	
	zScale = readFile(deviceDir + "/" + zName+"_scale", status);
	if(!status)
	{
		zScale = 1;
		std::cerr<<"Device dosent have z scale assuming 1\n";
	}
	
	yScale = readFile(deviceDir + "/" + yName+"_scale", status);
	if(!status)
	{
		yScale = 1;
		std::cerr<<"Device dosent have y scale assuming 1\n";
	}

	return true;
}

Vector VectorDevice::getFrame()
{
	Vector frame;
	bool status;
	frame.x = readFile(fileX, status)*xScale;
	frame.y = readFile(fileY, status)*yScale;
	frame.z = readFile(fileZ, status)*zScale;
	return frame;
}

VectorDevice::~VectorDevice()
{
	fileX.close();
	fileY.close();
	fileZ.close();
}
