#include "tester.h"
#include <QtSql>
#include <skeleton.cpp>

void Tester::run_tests() {
    const QString db_name = "db_test.sqlite";

    QSqlDatabase sdb;
    sdb = QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName(db_name);
    sdb.open();
    Login::make_database();
    LibrarianUser libr;
    libr.add_librarian("Сидорович", "Бункер на Кордоне", "88005553535", "sidr", "123");

    qDebug() << "TEST1: " << (test1() ? "OK" : "FAIL");
    qDebug() << "TEST2: " << (test2() ? "OK" : "FAIL");
    qDebug() << "TEST3: " << (test3() ? "OK" : "FAIL");
    qDebug() << "TEST4: " << (test4() ? "OK" : "FAIL");
    qDebug() << "TEST5: " << (test5() ? "OK" : "FAIL");
    qDebug() << "TEST6: " << (test6() ? "OK" : "FAIL");
    qDebug() << "TEST7: " << (test7() ? "OK" : "FAIL");
    qDebug() << "TEST8: " << (test8() ? "OK" : "FAIL");
    qDebug() << "TEST9: " << (test9() ? "OK" : "FAIL");

    sdb.close();
    //QFile(db_name).remove();
}


bool Tester::test1() {
    return true;
}

bool Tester::test2() {
    return true;
}

bool Tester::test3() {
    return true;
}

bool Tester::test4() {
    return true;
}

bool Tester::test5() {
    return true;
}

bool Tester::test6() {
    return true;
}

bool Tester::test7() {
    return true;
}

bool Tester::test8() {
    return true;
}

bool Tester::test9() {
    return true;
}
