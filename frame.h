#ifndef FRAME_H
#define FRAME_H

#include <QRubberBand>
#include <QColor>

class Frame : public QRubberBand
{
    Q_OBJECT
public:
    explicit Frame(QWidget *parent);

    void activate();
    void deactivate();
    void chooseClass();
    void setClassname(const QString &classname);
    QString getClassname();
    Frame(QWidget *parent, const QRect &r, const QString &classname);
protected:
    void paintEvent(QPaintEvent *event);
private:
    QColor color_;
    bool classChosen;
    QString classname_;
};

#endif // FRAME_H
