#ifndef HASHER_H
#define HASHER_H

#include<QCoreApplication>

class Hasher
{
public:
    static QString hash_password(QString login, QString password);
};

#endif // HASHER_H
