#ifndef SIGNLIST_H
#define SIGNLIST_H

#include <QListWidget>

class SignList : public QListWidget
{
    Q_OBJECT
public:
    explicit SignList();

signals:

public slots:
    void clear();

};

#endif // SIGNLIST_H
