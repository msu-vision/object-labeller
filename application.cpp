#include <QtGui>
#include <QDir>
#include <QVBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>

#include <cstdlib>

#include <algorithm>
using std::min;
using std::max;

#include "application.h"

Application::Application(QWidget *parent_)
    : QWidget(parent_),
      dirname_(),
      filenames_(),
      saveFilename_(),
      fileInd_(0),
      area_(nullptr),
      countLabel_(nullptr),
      prevButton_(nullptr),
      nextButton_(nullptr),
      nextShortcut_(nullptr),
      prevShortcut_(nullptr) {
  QVBoxLayout *mainLayout = new QVBoxLayout;

  mainLayout->setSpacing(0);
  mainLayout->setMargin(0);
  mainLayout->setContentsMargins(0, 0, 0, 0);

  QHBoxLayout *buttonsLayout = new QHBoxLayout;
  buttonsLayout->setAlignment(Qt::AlignLeft);

  QPushButton *openButton = new QPushButton("Open directory");
  openButton->setMaximumWidth(100);
  buttonsLayout->addWidget(openButton);

  buttonsLayout->addSpacerItem(new QSpacerItem(50, 20));

  prevButton_ = new QPushButton("Prev image");
  openButton->setMaximumWidth(100);
  buttonsLayout->addWidget(prevButton_);

  countLabel_ = new QLabel("");
  countLabel_->setAlignment(Qt::AlignCenter);
  countLabel_->setFixedWidth(100);
  buttonsLayout->addWidget(countLabel_);

  nextButton_ = new QPushButton("Next image");
  openButton->setMaximumWidth(100);
  buttonsLayout->addWidget(nextButton_);

  toggleButtons();

  mainLayout->addLayout(buttonsLayout);

  QScrollArea *scroll = new QScrollArea;
  area_ = new ImageArea();
  scroll->setWidget(area_);
  scroll->setWidgetResizable(true);
  mainLayout->addWidget(scroll);

  setLayout(mainLayout);

  connect(openButton, SIGNAL(clicked()), this, SLOT(openDirectory()));
  connect(nextButton_, SIGNAL(clicked()), this, SLOT(nextImage()));

  nextShortcut_ = new QShortcut(QKeySequence("PgDown"), this);
  connect(nextShortcut_, SIGNAL(activated()), this, SLOT(nextImage()));
  nextShortcut_->setEnabled(false);

  nextShortcut2_ = new QShortcut(QKeySequence("Right"), this);
  connect(nextShortcut2_, SIGNAL(activated()), this, SLOT(nextImage()));
  nextShortcut2_->setEnabled(false);

  connect(prevButton_, SIGNAL(clicked()), this, SLOT(prevImage()));

  prevShortcut_ = new QShortcut(QKeySequence("PgUp"), this);
  connect(prevShortcut_, SIGNAL(activated()), this, SLOT(prevImage()));
  prevShortcut_->setEnabled(false);

  prevShortcut2_ = new QShortcut(QKeySequence("Left"), this);
  connect(prevShortcut2_, SIGNAL(activated()), this, SLOT(prevImage()));
  prevShortcut2_->setEnabled(false);

  QShortcut *goTo = new QShortcut(QKeySequence("G"), this);
  connect(goTo, SIGNAL(activated()), this, SLOT(goToImageDialog()));

  QShortcut *addBb = new QShortcut(QKeySequence("a"), this);
  connect(addBb, SIGNAL(activated()), this, SLOT(addBboxes()));

  showMaximized();
}

void Application::goToImageDialog() {
  bool ok;
  int frame_number =
      QInputDialog::getInt(this, "Input frame number", "Frame number:", 1, 1,
                           filenames_.size(), 1, &ok);
  if (ok) goToImage(frame_number);
}

Application::~Application() {}

void Application::openDirectory() {
  dirname_ = QFileDialog::getExistingDirectory(
      this, tr("Open Directory"), "",
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

  if (dirname_ == "") return;

  QDir directory(dirname_);
  saveFilename_ = dirname_ + "/labelling.txt";
  fileInd_ = 0;
  loadBboxes();
  updateCountLabel();
  toggleButtons();
  nextShortcut_->setEnabled(true);
  nextShortcut2_->setEnabled(true);
  prevShortcut_->setEnabled(true);
  prevShortcut2_->setEnabled(true);
  showImage();
}

void Application::showImage() {
  QString filename = dirname_ + "/" + filenames_[fileInd_];
  bool ok = area_->openImage(filename);
  if (!ok) {
    QMessageBox::warning(this, "File can't be opened",
                         "File " + filename + " can't be opened");
    return;
  }
  area_->replaceFrames(bboxes_[fileInd_]);
}

void Application::addBboxes() {
  const auto &cur_bboxes = area_->getBboxes();
  const auto &prev_bboxes = bboxes_[fileInd_ - 1];

  for (const auto &bbox : prev_bboxes) {
    int prev_id = bbox.second;
    bool add = true;
    for (const auto &cur_bbox : cur_bboxes) {
      if (cur_bbox.second == prev_id) {
        add = false;
        break;
      }
    }
    if (add) {
      area_->addFrame(bbox.first, bbox.second);
    }
  }
}

void Application::toggleButtons() {
  nextButton_->setEnabled(fileInd_ < filenames_.size() - 1);
  prevButton_->setEnabled(fileInd_ > 0);
}

void Application::updateCountLabel() {
  countLabel_->setText(QString::number(fileInd_ + 1) + "/" +
                       QString::number(filenames_.size()));
}

void Application::nextImage() { goToImage(fileInd_ + 1); }

void Application::prevImage() { goToImage(fileInd_ - 1); }

void Application::goToImage(int number) {
  saveBboxes();
  fileInd_ = min(max(number, 0), filenames_.size() - 1);
  updateCountLabel();
  toggleButtons();
  showImage();
}

void Application::saveBboxes() {
  QFile file(saveFilename_);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QMessageBox::warning(
        this, "File can't be opened",
        "File " + saveFilename_ + " can't be opened for writing");
    return;
  }

  bboxes_[fileInd_] = area_->getBboxes();
  QTextStream fhandle(&file);
  for (int i = 0; i < filenames_.size(); ++i) {
    const auto &filename = filenames_[i];
    for (const auto &frame : bboxes_[i]) {

      fhandle << filename << " "
              << frame.first.x() << " " << frame.first.y() << " "
              << frame.first.width() << " " << frame.first.height() << " "
              << frame.second << endl;
    }
  }
}

void Application::loadBboxes() {
  QFile file(saveFilename_);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    return;
  }

  QTextStream fhandle(&file);
  QString filename = "f", prev_filename = "";
  filenames_.clear();
  int id, max_id = -1;
  int x, y, h, w;
  bboxes_.clear();
  int cur_i = -1;
  while (!fhandle.atEnd()) {
    fhandle >> filename;
    if (fhandle.atEnd()) {
      break;
    }
    if (filename != prev_filename) {
        cur_i++;
        filenames_.push_back(filename);
        bboxes_.push_back(QVector<QPair<QRect, int>>());
    }

    fhandle >> x >> y >> w >> h >> id;
    bboxes_[cur_i].push_back(QPair<QRect, int>(QRect(x, y, w, h), id));
    max_id = max(max_id, id);
    prev_filename = filename;
  }
  if (max_id != -1) area_->setFrameId(max_id + 1);
}
