#include "queue.h"

#define PATRON_STUDENT             1
#define PATRON_INSTRUCTOR          2
#define PATRON_TA                  3
#define PATRON_PROFESSOR           4
#define PATRON_VISITINGPROFESSOR   5

Queue::Queue() {
}

Queue::Queue(QString users) {
    from_db(users);
}

void Queue::from_db(QString users) {
    QStringList pairs = users.split(";");
    for (int i = 0; i < pairs.size(); i++){
        QString user = pairs[i];
        QStringList user_data = user.split(":");
        this->users.append(QPair<int,int>(user_data.at(0).toInt(), user_data.at(1).toInt()));
    }
}

QString Queue::to_db() {
    QString result;
    for (int i = 0; i < this->users.size(); i++){
        QPair<int,int> user = users[i];
        result += QString::number(user.first) + ':' + QString::number(user.second) + ';';
    }
    result.chop(1);
    return result;
}

void Queue::add_patron(QPair<int, int> user) {
    for (int i = 0; i < this->users.size(); ++i) {
        if (this->users[i].first > user.first) {
            this->users.insert(i,user);
            return;
        }
    }
    this->users.append(user);
}

int Queue::next_user() {
    if(!this->users.empty())
        return this->users[0].second;
    else
        return -1;
}

void Queue::delete_next() {
    if(!this->users.empty())
        this->users.removeFirst();
}
