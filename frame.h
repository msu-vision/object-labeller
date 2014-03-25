#ifndef FRAME_H
#define FRAME_H

#include <QRubberBand>

class Frame : public QRubberBand
{
    Q_OBJECT
public:
    explicit Frame(QWidget *parent);

protected:
    void paintEvent(QPaintEvent *event);

};

#endif // FRAME_H
