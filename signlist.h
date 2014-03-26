#ifndef SIGNLIST_H
#define SIGNLIST_H

#include <QListWidget>

class SignList : public QListWidget
{
    Q_OBJECT
public:
    explicit SignList();

    void showIcons(const QStringList &names, const QVector<QIcon> &icons);
signals:

};

#endif // SIGNLIST_H
