#include "compasswidget.h"
#include <math.h>
#include <algorithm>
#include <QGraphicsItem>
#include <QStyle>
#include <QGraphicsTextItem>
#include <QTextDocument>
#include <QTextBlockFormat>
#include <QTextCursor>
#include <QSize>

CompassWidget::CompassWidget(QWidget *parent) : QGraphicsView(parent)
{
    setScene(&scene_);
    setStyleSheet("background: transparent");
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);
    setMinimumSize(QSize(180,180));
}

void CompassWidget::paintEvent(QPaintEvent *event)
{
    drawCompass();
    QGraphicsView::paintEvent(event);
}

void CompassWidget::setRotation(double rotation)
{
    for(QGraphicsItem* item: scene_.items())
    {
       //setTransformOriginPoint(QPoint(0,0));
       // setRotation(rotation);
    }
}

void CompassWidget::drawCompass()
{
    constexpr int numTicks = 360;
    constexpr double tickToRad = 2*M_PI/numTicks;
    double radius = std::min(size().rwidth()/2-5, size().rheight()/2-5);
    QPen pen(style()->standardPalette().windowText(), radius < 200 ? 1 : 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    scene_.clear();

    //rose
    double roseRadius = 4*radius/5 - 55;
    double roseCornerRadius = radius/6 - 10;
    scene_.addLine(-roseRadius, 0, roseRadius, 0, pen);
    scene_.addLine(0, -roseRadius, 0, roseRadius, pen);
    scene_.addLine(-roseCornerRadius, -roseCornerRadius, 0, -roseRadius, pen);
    scene_.addLine(-roseCornerRadius, -roseCornerRadius, -roseRadius, 0, pen);
    scene_.addLine(roseCornerRadius, -roseCornerRadius, 0, -roseRadius, pen);
    scene_.addLine(roseCornerRadius, -roseCornerRadius, roseRadius, 0, pen);
    scene_.addLine(roseCornerRadius, roseCornerRadius, roseRadius, 0, pen);
    scene_.addLine(roseCornerRadius, roseCornerRadius, 0, roseRadius, pen);
    scene_.addLine(-roseCornerRadius, roseCornerRadius, 0, roseRadius, pen);
    scene_.addLine(-roseCornerRadius, roseCornerRadius, -roseRadius, 0, pen);

    double nsewRadius = roseRadius+10;

    QGraphicsTextItem* labelNorth = scene_.addText("N");
    labelNorth->setPos(-labelNorth->boundingRect().width()/2, -nsewRadius-labelNorth->boundingRect().height());
    labelNorth->setDefaultTextColor(style()->standardPalette().windowText().color());

    QGraphicsTextItem* labelSouth = scene_.addText("S");
    labelSouth->setTransformOriginPoint(QPoint(0,0));
    QTransform rotate;
    rotate.rotate(180);
    labelSouth->setTransform(rotate);
    labelSouth->setPos(labelSouth->boundingRect().width()/2, nsewRadius+labelNorth->boundingRect().height());
    labelSouth->setDefaultTextColor(style()->standardPalette().windowText().color());

    QGraphicsTextItem* labelEast = scene_.addText("E");
    rotate.rotate(-90);
    labelEast->setTransform(rotate);
    labelEast->setPos(nsewRadius+labelEast->boundingRect().height(), -labelEast->boundingRect().width()/2);
    labelEast->setDefaultTextColor(style()->standardPalette().windowText().color());

    QGraphicsTextItem* labelWest = scene_.addText("W");
    rotate.rotate(180);
    labelWest->setTransform(rotate);
    labelWest->setPos(-nsewRadius-labelWest->boundingRect().height(), labelWest->boundingRect().width()/2);
    labelWest->setDefaultTextColor(style()->standardPalette().windowText().color());

    //circle/ticks
    double tickRadius = radius-30;
    scene_.setSceneRect(-radius/2, -radius/2, radius, radius);
    scene_.addEllipse(-radius,-radius,radius*2,radius*2, pen);
    scene_.addEllipse(-tickRadius,-tickRadius, tickRadius*2,tickRadius*2, pen);

    for(int i = 0; i < numTicks; i++)
    {
        scene_.addLine(sin(i*tickToRad)*tickRadius,
                       cos(i*tickToRad)*tickRadius,
                       sin(i*tickToRad)*(tickRadius+5+5*(i%5==0)+10*(i%30==0)),
                       cos(i*tickToRad)*(tickRadius+5+5*(i%5==0)+10*(i%30==0)), pen);
    }


}
