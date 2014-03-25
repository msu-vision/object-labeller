#ifndef APPLICATION_H
#define APPLICATION_H

#include <QWidget>
#include <QMainWindow>

class Application : public QWidget
{
    Q_OBJECT

public:
    explicit Application(QWidget *parent = 0);
    ~Application();

private:
};

#endif // APPLICATION_H
