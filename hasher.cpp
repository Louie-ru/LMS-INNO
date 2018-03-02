#include "hasher.h"
#include <sha_512.h>
#include <string>

#define localsolt "tpmutnjyfttqjdft"

QString Hasher::hash_password(QString login, QString password) {
    std::string hashing_string = login.toStdString() + password.toStdString() + localsolt;
     return QString::fromStdString(sha512(hashing_string));
}
