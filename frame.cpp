#include "frame.h"

#include <QPainter>
#include <QPen>
#include <QPaintEvent>

Frame::Frame(QWidget *parent) : QRubberBand(QRubberBand::Rectangle, parent)
{
}

void
Frame::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    QPen pen(Qt::red, 5);
    pen.setStyle(Qt::SolidLine);

    painter.begin(this);
    painter.setPen(pen);
    painter.drawRect(event->rect());
    painter.end();
}
