#include "frame.h"

#include <QPainter>
#include <QPen>
#include <QPaintEvent>

Frame::Frame(QWidget *parent) : QRubberBand(QRubberBand::Rectangle, parent), color_(Qt::red), classChosen(false)
{
}

Frame::Frame(QWidget *parent, const QRect &r, const QString &classname) :
    QRubberBand(QRubberBand::Rectangle, parent), color_(Qt::red), classChosen(false)
{
    setGeometry(r);
    if (classname != "")
    {
        setClassname(classname);
    }
}

void
Frame::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    QPen pen(color_, 5);
    pen.setStyle(Qt::SolidLine);

    painter.begin(this);
    painter.setPen(pen);
    painter.drawRect(event->rect());
    painter.end();
}

void
Frame::activate()
{
    color_ = Qt::green;
    update();
}

void
Frame::deactivate()
{
    if (!classChosen)
        color_ = Qt::red;
    else
        color_ = Qt::blue;
    update();
}

void Frame::setClassname(const QString &classname)
{
    classname_ = classname;
    color_ = Qt::blue;
    classChosen = true;
}

QString Frame::getClassname()
{
    return classname_;
}
