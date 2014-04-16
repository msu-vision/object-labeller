#include <QtGui>
#include <QDir>
#include <QVBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QMessageBox>
#include <QFileDialog>

#include <cstdlib>

#include <algorithm>
using std::min;
using std::max;

#include "application.h"
#include "signlist.h"

Application::Application(QWidget *parent_) :
    QWidget(parent_), classIcons_(), classNames_(), classFileNames_(),
    dirname_(), filenames_(), saveFilename_(), fileInd_(0),
    area_(nullptr), list_(nullptr), label_(nullptr), countLabel_(nullptr),
    backButton_(nullptr), prevButton_(nullptr), nextButton_(nullptr),
    saveButton_(nullptr), exitButton_(nullptr), unlabelledButton_(nullptr),
    nextShortcut_(nullptr), prevShortcut_(nullptr), superclassIcons_(),
    superclassNames_(), superclassFileNames_()
{
    loadIcons();

    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->setAlignment(Qt::AlignLeft);

    QPushButton *openButton = new QPushButton("Open directory");
    openButton->setMaximumWidth(100);
    buttonsLayout->addWidget(openButton);

    QSpacerItem *spacer1 = new QSpacerItem(50, 20);
    buttonsLayout->addSpacerItem(spacer1);

    backButton_ = new QPushButton("Back to superclass");
    backButton_->setMaximumWidth(150);
    buttonsLayout->addWidget(backButton_);

    QSpacerItem *spacer3 = new QSpacerItem(20, 20);
    buttonsLayout->addSpacerItem(spacer3);

    label_ = new QLabel("Current class: ");
    label_->setFixedWidth(200);
    buttonsLayout->addWidget(label_);

    QSpacerItem *spacer2 = new QSpacerItem(50, 20);
    buttonsLayout->addSpacerItem(spacer2);

    prevButton_ = new QPushButton("Prev image");
    openButton->setMaximumWidth(100);
    buttonsLayout->addWidget(prevButton_);

    countLabel_ = new QLabel("");
    countLabel_->setAlignment(Qt::AlignCenter);
    countLabel_->setFixedWidth(50);
    buttonsLayout->addWidget(countLabel_);

    nextButton_ = new QPushButton("Next image");
    openButton->setMaximumWidth(100);
    buttonsLayout->addWidget(nextButton_);

    QSpacerItem *spacer4 = new QSpacerItem(20, 20);
    buttonsLayout->addSpacerItem(spacer4);

    unlabelledButton_ = new QPushButton("First unlabelled");
    unlabelledButton_->setMaximumWidth(100);
    unlabelledButton_->setEnabled(false);
    buttonsLayout->addWidget(unlabelledButton_);

    QSpacerItem *spacer5 = new QSpacerItem(100, 20);
    buttonsLayout->addSpacerItem(spacer5);

    saveButton_ = new QPushButton("Save");
    saveButton_->setMaximumWidth(100);
    saveButton_->setEnabled(false);
    buttonsLayout->addWidget(saveButton_);

    exitButton_ = new QPushButton("Exit");
    exitButton_->setMaximumWidth(100);
    buttonsLayout->addWidget(exitButton_);

    toggleButtons();

    mainLayout->addLayout(buttonsLayout);

    list_ = new SignList;
    list_->setFixedWidth(width());
    showSuperclassIcons();
    mainLayout->addWidget(list_);


    QScrollArea *scroll = new QScrollArea;
    area_ = new ImageArea();
    scroll->setWidget(area_);
    scroll->setWidgetResizable(true);
    mainLayout->addWidget(scroll);

    setLayout(mainLayout);

    connect(list_, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(itemSelected(QListWidgetItem *)));
    connect(list_, SIGNAL(itemActivated(QListWidgetItem *)), this, SLOT(itemSelected(QListWidgetItem *)));

    connect(openButton, SIGNAL(clicked()), this, SLOT(openDirectory()));
    connect(area_, SIGNAL(frameAdded()), this, SLOT(showSuperclassIcons()));
    connect(area_, SIGNAL(frameActivated(Frame *)), this, SLOT(showFrameClass(Frame *)));
    connect(backButton_, SIGNAL(clicked()), this, SLOT(showSuperclassIcons()));
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

    connect(unlabelledButton_, SIGNAL(clicked()), this, SLOT(goToFirstUnlabelled()));
    connect(exitButton_, SIGNAL(clicked()), this, SLOT(exit()));
    connect(saveButton_, SIGNAL(clicked()), this, SLOT(saveBboxes()));

    showMaximized();

}

void Application::exit()
{
    if (saveButton_->isEnabled())
        saveBboxes();
    QCoreApplication::exit();
}

void Application::goToFirstUnlabelled()
{
    int i = 0;
    for (; i < filenames_.size(); ++i)
        if (!bboxes_.contains(filenames_[i]))
            break;
    if (i == filenames_.size()) {
        QMessageBox::warning(this, "OK", "Everything is labelled");
        return;
    }
    bboxes_[filenames_[fileInd_]] = area_->getBboxes();
    fileInd_ = i;
    updateCountLabel();
    toggleButtons();
    showImage();
}

void
Application::showFrameClass(Frame *frame)
{
    label_->setText("Current class: " + frame->getClassname());
    label_->repaint();
}

void
Application::showSuperclassIcons()
{
    backButton_->setEnabled(false);
    list_->setFixedWidth(width());
    list_->showIcons(superclassNames_, superclassIcons_);
}

void
Application::showClassIcons(int ind)
{
    backButton_->setEnabled(true);
    list_->setFixedWidth(width());
    list_->showIcons(classNames_[ind], classIcons_[ind]);
}

void
Application::itemSelected(QListWidgetItem *item)
{
    if (item->text().length() == 1) {
        // superclass
        int ind = getSuperclassInd(item->text());
        if (ind == -1) {
            QMessageBox::warning(this, "ind = -1", "Superclass not found by text " + item->text());
            qDebug() << "Superclass ind = -1";
            return;
        }
        showClassIcons(ind);
    } else {
        // class
        area_->setClass(item->text());
    }
}

void
Application::loadIcons()
{
    QString dirname = "sign-icons/superclass";
    QDir dir(dirname);
    for (QString superclassFileName : dir.entryList({"*.gif"}))
    {
        QString filepath = dirname + "/" + superclassFileName;
        superclassFileNames_.push_back(filepath);
        superclassNames_.push_back(superclassFileName.left(superclassFileName.size() - 4));
        superclassIcons_.push_back(QIcon(filepath));
    }
    for (const QString &superclassName : superclassNames_)
    {
        dirname = "sign-icons/" + superclassName;
        dir.setPath(dirname);
        QStringList filenames;
        QStringList names;
        QVector<QIcon> icons;
        for (const QString &classFileName : dir.entryList({"*.gif"}))
        {
            QString filepath = dirname + "/" + classFileName;
            filenames.push_back(filepath);
            names.push_back(classFileName.left(classFileName.size() - 4));
            icons.push_back(QIcon(filepath));
        }
        classIcons_.push_back(icons);
        classNames_.push_back(names);
        classFileNames_.push_back(filenames);
    }
}

Application::~Application()
{
}

void
Application::openDirectory()
{
    dirname_ = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
        "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    QDir directory(dirname_);
    filenames_ = directory.entryList({"*.jpg"});
    if (filenames_.size() == 0) {
        QMessageBox::warning(this, "No images", "No JPG images found");
        return;
    }
    saveFilename_ = dirname_ + "/labelling.txt";
    fileInd_ = 0;
    updateCountLabel();
    toggleButtons();
    saveButton_->setEnabled(true);
    nextShortcut_->setEnabled(true);
    nextShortcut2_->setEnabled(true);
    prevShortcut_->setEnabled(true);
    prevShortcut2_->setEnabled(true);
    unlabelledButton_->setEnabled(true);
    loadBboxes();
    showImage();
}

void
Application::showImage()
{
    QString filename = dirname_ + "/" + filenames_[fileInd_];
    bool ok = area_->openImage(filename);
    if (!ok) {
        QMessageBox::warning(this, "File can't be opened", "File " + filename + " can't be opened");
        return;
    }
    if (bboxes_.contains(filenames_[fileInd_]))
        area_->replaceFrames(bboxes_[filenames_[fileInd_]]);
}

void
Application::toggleButtons()
{
    nextButton_->setEnabled(fileInd_ < filenames_.size() - 1);
    prevButton_->setEnabled(fileInd_ > 0);
}

void
Application::updateCountLabel()
{
    countLabel_->setText(QString::number(fileInd_ + 1) + "/" + QString::number(filenames_.size()));
}

void
Application::nextImage()
{
    saveBboxes();
    fileInd_ = min(fileInd_ + 1, filenames_.size() - 1);
    updateCountLabel();
    toggleButtons();
    showImage();

}

void
Application::prevImage()
{
    saveBboxes();
    fileInd_ = max(fileInd_ - 1, 0);
    updateCountLabel();
    toggleButtons();
    showImage();
}

int
Application::getSuperclassInd(const QString &name)
{
    int i = 0;
    for (; i < superclassNames_.size(); ++i)
        if (superclassNames_[i] == name)
            break;
    if (i == superclassNames_.size())
        return -1;
    return i;
}

void
Application::saveBboxes()
{
    QFile file(saveFilename_);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "File can't be opened", "File " + saveFilename_ + " can't be opened for writing");
        return;
    }

    bboxes_[filenames_[fileInd_]] = area_->getBboxes();
    QTextStream fhandle(&file);
    for (const auto &key : bboxes_.keys())
    {
        for (const auto &frame : bboxes_[key])
        {
            fhandle << key << " " << frame.first.x() << " " << frame.first.y() << " "
                 << frame.first.width() << " " << frame.first.height() << " ";
            if (frame.second == "")
                fhandle << "NONE" << endl;
            else
                fhandle << frame.second << endl;
        }
    }

}

void
Application::loadBboxes()
{
    QFile file(saveFilename_);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream fhandle(&file);
    QString key;
    QString classname;
    int x, y, h, w;
    while (!fhandle.atEnd())
    {
        fhandle >> key >> x >> y >> w >> h >> classname;
        if (classname == "NONE")
            classname = "";
        bboxes_[key].push_back(QPair<QRect, QString>(QRect(x, y, w, h), classname));
    }

}
