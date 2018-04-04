#include "mainwindow.h"
#include <QApplication>
#include <QtSql>
#include <tester2.h>

QSqlDatabase sdb;

int main(int argc, char *argv[]){
    //Tester2::run_tests();
    QApplication a(argc, argv);
    sdb = QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName("db.sqlite");
    sdb.open();
    MainWindow w;
    w.show();
    return a.exec();
}
