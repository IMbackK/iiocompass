#ifndef COMPASSWIDGET_H
#define COMPASSWIDGET_H

#include <QGraphicsView>

class CompassWidget : public QGraphicsView
{
    Q_OBJECT
public:
    explicit CompassWidget(QWidget *parent = nullptr);
    QGraphicsScene scene_;

public slots:
    void setRotation(double rotation);

signals:

protected:
   virtual void paintEvent(QPaintEvent *event);

private:
    void drawCompass();

};

#endif // COMPASSWIDGET_H
