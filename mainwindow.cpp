#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWidget>
#include <QtSql>

Login mainLogin;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    mainLogin.make_database();//make database if not found
    //Testing testing; testing.testAll(); //no need in this =) ask me for special database to for testing
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
        patron->showName();
        this->close();
        return;
    }

    LibrarianUser librarianUser = mainLogin.login_librarian(login, password);
    if (librarianUser.id != -1){
        librarian = new Librarian();
        librarian->me = librarianUser;
        librarian->show();
        librarian->showName();
        this->close();
    }

    AdminUser adminUser = mainLogin.login_admin(login, password);
    if (adminUser.id != -1){
        admin = new Admin();
        admin->me = adminUser;
        admin->show();
        admin->showName();
        this->close();
    }
}
