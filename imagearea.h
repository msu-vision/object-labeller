#ifndef IMAGEAREA_H
#define IMAGEAREA_H

#include <QtGui>
#include <QWidget>
#include <QVector>
#include <QListWidgetItem>
#include <QLabel>
#include <QPushButton>

#include "frame.h"

namespace Direction {
enum {
  N = 1,
  S = 2,
  W = 4,
  E = 8,
  NW = 5,
  NE = 9,
  SW = 6,
  SE = 10
};
}

namespace FrameAction {
enum {
  XResize = 1,
  YResize = 2,
  XYResize = 3,
  Move = 4
};
}

class ImageArea : public QWidget {
  Q_OBJECT
 public:
  ImageArea();
  ~ImageArea();

  bool openImage(const QString &filename);
  void setClass(const QString &classname);
  QVector<QPair<QRect, int>> getBboxes();
  void replaceFrames(const QVector<QPair<QRect, int>> &bboxes);
  void setFrameId(int id);
  void addFrame(const QRect &r, int id);
 public
slots:
  void onItemSelect(QListWidgetItem *item);

signals:
  void frameAdded();
  void frameActivated(Frame *frame);

 protected:
  void paintEvent(QPaintEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);

 private:
  int findFrame(int x, int y);
  QImage img_;
  QVector<Frame *> frames_;
  Frame *activeFrame_;
  int state;

  int fromX_;
  int fromY_;

  int posX_;
  int posY_;

  int frameId_;

  QPoint checkBounds(QPoint pos);
  int computeActionAndSetCursor(int x, int y, Frame *frame);
  bool eraseFrame(Frame *frame);
};

#endif  // IMAGEAREA_H
