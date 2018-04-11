#ifndef QUEUE_H
#define QUEUE_H
#include <QVector>
#include <QString>
#include <QPair>

class Queue
{
public:
    QVector<QPair<int,int>> users;

    Queue();
    Queue(QString users);

    void from_db(QString users);

    QString to_db();

    void add_patron(QPair<int,int>);

    int next_user();

    void delete_next();
};

#endif // QUEUE_H
