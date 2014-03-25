#include<QDebug>

#include "signlist.h"

QListWidgetItem *
createItem(const QString &filename)
{
    QListWidgetItem *item = new QListWidgetItem(QIcon(filename), filename);
    item->setSizeHint(QSize(70, 70));
    return item;
}

SignList::SignList()
{
    setViewMode(QListWidget::IconMode);
    setIconSize(QSize(70, 70));

    addItem(createItem("/tmp/1.gif"));
    addItem(createItem("/tmp/2.gif"));

    setFixedHeight(72);
}

void SignList::clear()
{
    qDebug() << "clear";
}
