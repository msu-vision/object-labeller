#ifndef APPLICATION_H
#define APPLICATION_H

#include <QWidget>
#include <QMainWindow>
#include <QHash>

#include "imagearea.h"

class Application : public QWidget {
  Q_OBJECT

 public:
  explicit Application(QWidget *parent = 0);
  ~Application();

 private
slots:
  void prevImage();
  void nextImage();
  void openDirectory();
  void saveBboxes();
  void goToImageDialog();
  void addBboxes();

 private:
  void goToImage(int number);
  QString dirname_;
  QStringList filenames_;
  QString saveFilename_;
  int fileInd_;
  ImageArea *area_;

  QLabel *countLabel_;
  QPushButton *prevButton_;
  QPushButton *nextButton_;
  QShortcut *nextShortcut_;
  QShortcut *nextShortcut2_;
  QShortcut *prevShortcut_;
  QShortcut *prevShortcut2_;
  void showImage();
  void toggleButtons();
  void loadBboxes();

  QVector<QVector<QPair<QRect, int>>> bboxes_;

  void updateCountLabel();
};

#endif  // APPLICATION_H
