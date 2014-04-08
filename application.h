#ifndef APPLICATION_H
#define APPLICATION_H

#include <QWidget>
#include <QMainWindow>
#include <QHash>

#include "imagearea.h"
#include "signlist.h"

class Application : public QWidget
{
    Q_OBJECT

public:
    explicit Application(QWidget *parent = 0);
    ~Application();


private slots:
    void itemSelected(QListWidgetItem *item);
    void prevImage();
    void nextImage();
    void openDirectory();
    void showSuperclassIcons();
    void showFrameClass(Frame *frame);
    void saveBboxes();
    void goToFirstUnlabelled();
    void exit();
private:
    QVector<QVector<QIcon>> classIcons_;
    QVector<QStringList> classNames_;
    QVector<QStringList> classFileNames_;

    QString dirname_;
    QStringList filenames_;
    QString saveFilename_;
    int fileInd_;
    ImageArea *area_;
    SignList *list_;

    QLabel *label_;
    QLabel *countLabel_;
    QPushButton *backButton_;
    QPushButton *prevButton_;
    QPushButton *nextButton_;
    QPushButton *saveButton_;
    QPushButton *exitButton_;
    QPushButton *unlabelledButton_;
    QShortcut *nextShortcut_;
    QShortcut *prevShortcut_;
    QVector<QIcon> superclassIcons_;
    QStringList superclassNames_;
    QStringList superclassFileNames_;
    void showImage();
    void toggleButtons();
    void loadIcons();
    int getSuperclassInd(const QString &name);





    void showClassIcons(int ind);
    void loadBboxes();

    QHash<QString, QVector<QPair<QRect, QString>>> bboxes_;

    void updateCountLabel();
};

#endif // APPLICATION_H
