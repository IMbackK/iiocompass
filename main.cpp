#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>
#include <string.h>
#include <QTimer>
#include "iiosensors/iiomagnetometer.h"
#include "iiosensors/iioaccell.h"
#include <iostream>

void updateHeading(iio::Magnetometer& magn, iio::Accelerometer& accel, MainWindow& w)
{
	iio::Vector frame = magn.getFrame();
	iio::Vector accelFrame = accel.getFrame();
	w.updateHeading(magn.getHeading(frame, accelFrame));
	std::cout<<"x: "<<frame.x<<" Y: "<<frame.y<<" Z:"<<frame.z<<" heading: "<<magn.getHeading(frame, frame)<<'\n';
}


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	std::string device = iio::Magnetometer::findMagnetometer();

	if(device.empty())
	{
		QMessageBox::critical(nullptr, "Error", "No Magnetometer available.");
		return 0;
	}
	iio::Magnetometer magn(device);
	
	device = iio::Accelerometer::findAccellerometer();
	if(device.empty())
	{
		QMessageBox::critical(nullptr, "Error", "No Accelerometer available.");
		return 0;
	}
	iio::Accelerometer accel(device);
	int rate = accel.getRate();
	//if(rate > 0 && rate < 10)
		accel.setRate(100);
	
	QTimer sampleTimer;
	sampleTimer.setInterval(100);
	sampleTimer.setSingleShot(false);
	sampleTimer.start();

	MainWindow w;
	QObject::connect(&sampleTimer, &QTimer::timeout, [&magn, &accel, &w](){updateHeading(magn, accel, w);});

	w.show();
	return a.exec();
}
