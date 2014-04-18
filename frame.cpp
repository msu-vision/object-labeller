#include "frame.h"

#include <QPainter>
#include <QPen>
#include <QPaintEvent>
#include <QDebug>
#include <QLabel>

Frame::Frame(QWidget *parent, int id) : QRubberBand(QRubberBand::Rectangle, parent), color_(Qt::red), idLabel_(nullptr), id_(id)
{
    idLabel_ = new QLabel(QString::number(id), parent);
    idLabel_->setFont(QFont("Sans", 12, QFont::Bold));
    setColor(color_);
}

Frame::Frame(QWidget *parent, int id, const QRect &r) :
    QRubberBand(QRubberBand::Rectangle, parent), color_(Qt::red), idLabel_(nullptr), id_(id)
{
    idLabel_ = new QLabel(QString::number(id), parent);
    setGeometry(r);
    setColor(color_);
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

int Frame::getId() const
{
    return id_;
}

void Frame::setColor(const QColor &color)
{
    color_ = color;
    QPalette palette = idLabel_->palette();
    palette.setColor(idLabel_->foregroundRole(), color_);
    idLabel_->setPalette(palette);
}

void
Frame::activate()
{
    setColor(Qt::green);
    update();
}

void Frame::setGeometry(const QRect &r)
{
    setGeometry(r.x(), r.y(), r.width(), r.height());
}

void Frame::setGeometry(int x, int y, int w, int h)
{
    QRubberBand::setGeometry(x, y, w, h);
    if (w > 10 && h > 10) {
        idLabel_->show();
        idLabel_->move(x + 2, y);
    }
}

void Frame::deactivate()
{
    setColor(Qt::red);
    update();
}

Frame::~Frame()
{
    idLabel_->setParent(nullptr);
    delete idLabel_;
}
