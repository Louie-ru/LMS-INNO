#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    patron = new Patron();
    patron->show();
    this->close();
}

void MainWindow::on_pushButton_2_clicked()
{
    librarian = new Librarian();
    librarian->show();
    this->close();
}