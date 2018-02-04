#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWidget>
#include <QtSql>

QSqlDatabase sdb;

Login mainLogin;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    sdb = QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName("db.sqlite");
    sdb.open();

    mainLogin.make_database();
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_button_login_clicked(){
    QString login = ui->line_login->text();
    QString password = ui->line_password->text();

    PatronUser patronUser = mainLogin.login_patron(login, password);
    if (patronUser.id != -1){
        patron = new Patron();
        patron->me = patronUser;
        patron->show();
        this->close();
    }

    LibrarianUser librarianUser = mainLogin.login_librarian(login, password);
    if (librarianUser.id != -1){
        librarian = new Librarian();
        librarian->me = librarianUser;
        librarian->show();
        this->close();
    }
}
