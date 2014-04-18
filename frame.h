#ifndef FRAME_H
#define FRAME_H

#include <QRubberBand>
#include <QColor>
#include <QLabel>

class Frame : public QRubberBand
{
    Q_OBJECT
public:
    Frame(QWidget *parent, int id);
    Frame(QWidget *parent, int id, const QRect &r);
    ~Frame();

    void activate();
    void deactivate();

    void setGeometry(const QRect &r);
    void setGeometry(int x, int y, int w, int h);

    int getId() const;

    void setColor(const QColor &color);

protected:
    void paintEvent(QPaintEvent *event);
private:
    QColor color_;
    QLabel *idLabel_;
    int id_;
};

#endif // FRAME_H
