#include <QtGui>

#include "application.h"
#include "imagearea.h"
#include "signlist.h"

Application::Application(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    SignList *list = new SignList;
    mainLayout->addWidget(list);


    QScrollArea *scroll = new QScrollArea;
    ImageArea *area = new ImageArea("/home/vlad/s.png");
    scroll->setWidget(area);
    scroll->setWidgetResizable(true);
    mainLayout->addWidget(scroll);

    setLayout(mainLayout);

    connect(list, SIGNAL(itemClicked(QListWidgetItem *)), area, SLOT(onItemSelect(QListWidgetItem *)));
    connect(list, SIGNAL(itemActivated(QListWidgetItem *)), area, SLOT(onItemSelect(QListWidgetItem *)));

    connect(area, SIGNAL(frameAdded()), list, SLOT(clear()));

    showMaximized();
}

Application::~Application()
{
}
