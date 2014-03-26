#include<QDebug>

#include "signlist.h"

QListWidgetItem *
createItem(const QString &name, const QIcon &icon)
{    
    QListWidgetItem *item = new QListWidgetItem(icon, name);
    item->setSizeHint(QSize(70, 70));
    return item;
}

SignList::SignList()
{
    setViewMode(QListWidget::IconMode);
    setDragEnabled(false);
    setIconSize(QSize(70, 70));
    setFixedHeight(72);
}

void
SignList::showIcons(const QStringList &names, const QVector<QIcon> &icons)
{
    clear();
    for (int i = 0; i < names.size(); ++i)
        addItem(createItem(names.at(i), icons.at(i)));
}
