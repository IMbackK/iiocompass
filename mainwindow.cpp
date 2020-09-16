#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>
#include <QTransform>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::updateHeading(double heading)
{
	QTransform tr;
	tr.rotate((heading/(2*M_PI))*360);
	ui->graphicsView->setTransform(tr);
}

