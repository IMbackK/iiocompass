#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <cmath>

namespace iio
{
	class Vector
	{
	public:
		double x;
		double y;
		double z;
		inline void scale(double scale)
		{
			x = x*scale;
			y = y*scale;
			z = z*scale;
		}
		inline Vector norm() const
		{
			Vector tmp;
			double normF = 1.0/sqrt(x*x + y*y + z*z);
			tmp.x = x*normF;
			tmp.y = y*normF;
			tmp.z = z*normF;
			return tmp;
		}
		inline Vector operator*(const double scalar) const
		{
			Vector tmp;
			tmp = *this;
			tmp.scale(scalar);
			return tmp;
		}
		inline double operator*(const Vector& in) const
		{
			return x*in.x + y*in.y + z*in.z;
		}
		inline void operator=(const Vector& in)
		{
			x = in.x;
			y = in.y;
			z = in.z;
		}
		inline Vector operator-(const Vector& in) const
		{
			Vector tmp;
			tmp = *this;
			tmp.x -= in.x;
			tmp.y -= in.y;
			tmp.z -= in.z;
			return tmp;
		}
		inline Vector operator+(const Vector& in) const
		{
			Vector tmp;
			tmp = *this;
			tmp.x += in.x;
			tmp.y += in.y;
			tmp.z += in.z;
			return tmp;
		}
	};
	
	class Device
	{
		protected:
		std::string deviceDir;
		
		static constexpr char IIO_DIRECTORY[] = "/sys/bus/iio/devices/";
		
		double readFile(const std::string& fileName, bool& status);
		double readFile(std::fstream& file, bool& status);
		
		static std::string findByProps(const std::vector<std::string>& props);
		
		public:

		bool setRate(unsigned int rate);
		int getRate();
	};
	
	class VectorDevice: public Device
	{
		protected:
		std::fstream fileX;
		std::fstream fileY;
		std::fstream fileZ;
		double xScale;
		double yScale;
		double zScale;
		
		bool openDevice(const std::string device, const std::string xName, const std::string yName, const std::string zName);
		
		public:
		
		Vector getFrame();
		~VectorDevice();
	};
	
}
