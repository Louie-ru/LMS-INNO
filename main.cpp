#include "mainwindow.h"
#include <QApplication>
#include <QtSql>

QSqlDatabase sdb;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    sdb = QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName("db.sqlite");
    sdb.open();

    MainWindow w;
    w.show();


    return a.exec();
}
