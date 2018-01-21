#include "patron.h"
#include "ui_patron.h"
#include <QPushButton>
#include <QMessageBox>
#include <QSignalMapper>
#include <QDebug>
#include <QVector>
#include <skeleton.cpp>

PatronUser p;
Patron::Patron(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Patron)
{
    ui->setupUi(this);
}

Patron::~Patron()
{
    delete ui;
}

void Patron::on_button_search_books_clicked(){
    ui->table_search_books->setRowCount(0);
    QVector<Book> found;// = patron.search_books("", "", "", "", -1, -1, -1, 1);//CHANGE LATER
    for (int i = 0; i < found.size(); i++){
        ui->table_search_books->insertRow(i);
        QPushButton *btn = new QPushButton(this);
        btn->setText("check out");
        QSignalMapper *sm = new QSignalMapper(this);
        //connect(sm, SIGNAL(mapped(int)), this, SLOT(check_out_book(int)));
        connect(btn, SIGNAL(clicked()), sm, SLOT(map()));
        sm->setMapping(btn, found[i].id);
        ui->table_search_books->setItem(i, 0, new QTableWidgetItem(QString::number(found[i].id)));
        ui->table_search_books->setItem(i, 1, new QTableWidgetItem(QString(found[i].title)));
        ui->table_search_books->setItem(i, 2, new QTableWidgetItem(QString(found[i].authors)));
        ui->table_search_books->setItem(i, 3, new QTableWidgetItem(QString(found[i].publisher)));
        ui->table_search_books->setItem(i, 4, new QTableWidgetItem(QString::number(found[i].price)));
        ui->table_search_books->setItem(i, 5, new QTableWidgetItem(found[i].bestseller ? "yes" : "no"));
        ui->table_search_books->setItem(i, 6, new QTableWidgetItem(QString::number(found[i].year)));
        ui->table_search_books->setItem(i, 7, new QTableWidgetItem(QString::number(found[i].copies)));
        ui->table_search_books->setItem(i, 8, new QTableWidgetItem(QString::number(found[i].room)));
        ui->table_search_books->setCellWidget(i, 9, btn);
    }
    ui->table_search_books->resizeColumnsToContents();
}

void Patron::on_button_search_articles_clicked(){
    ui->table_search_articles->setRowCount(0);
    QVector<Article> found;// = patron.search_articles("", "", "", "", -1, -1, -1, 1);//CHANGE LATER
    for (int i = 0; i < found.size(); i++){
        ui->table_search_articles->insertRow(i);
        QPushButton *btn = new QPushButton(this);
        btn->setText("check out");
        QSignalMapper *sm = new QSignalMapper(this);
        //connect(sm, SIGNAL(mapped(int)), this, SLOT(check_out_article(int)));
        connect(btn, SIGNAL(clicked()), sm, SLOT(map()));
        sm->setMapping(btn, found[i].id);
        ui->table_search_articles->setItem(i, 0, new QTableWidgetItem(QString::number(found[i].copies)));
        ui->table_search_articles->setItem(i, 1, new QTableWidgetItem(QString(found[i].title)));
        ui->table_search_articles->setItem(i, 2, new QTableWidgetItem(QString(found[i].authors)));
        ui->table_search_articles->setItem(i, 3, new QTableWidgetItem(QString(found[i].publisher)));
        ui->table_search_articles->setItem(i, 4, new QTableWidgetItem(QString::number(found[i].price)));
        ui->table_search_articles->setItem(i, 6, new QTableWidgetItem(QString::number(found[i].year)));
        ui->table_search_articles->setItem(i, 7, new QTableWidgetItem(QString::number(found[i].room)));
        ui->table_search_articles->setCellWidget(i, 9, btn);
    }
    ui->table_search_articles->resizeColumnsToContents();
}

void Patron::on_button_search_va_clicked(){
    ui->table_search_va->setRowCount(0);
    QVector<VA> found;// = patron.search_va("", "", "", "", -1, -1, -1, 1);//CHANGE LATER
    for (int i = 0; i < found.size(); i++){
        ui->table_search_va->insertRow(i);
        QPushButton *btn = new QPushButton(this);
        btn->setText("check out");
        QSignalMapper *sm = new QSignalMapper(this);
        //connect(sm, SIGNAL(mapped(int)), this, SLOT(check_out_article(int)));
        connect(btn, SIGNAL(clicked()), sm, SLOT(map()));
        sm->setMapping(btn, found[i].id);
        ui->table_search_va->setItem(i, 0, new QTableWidgetItem(QString::number(found[i].id)));
        ui->table_search_va->setItem(i, 1, new QTableWidgetItem(QString(found[i].title)));
        ui->table_search_va->setItem(i, 2, new QTableWidgetItem(QString(found[i].authors)));
        ui->table_search_va->setItem(i, 3, new QTableWidgetItem(QString::number(found[i].price)));
        ui->table_search_va->setItem(i, 4, new QTableWidgetItem(QString::number(found[i].copies)));
        ui->table_search_va->setItem(i, 5, new QTableWidgetItem(QString::number(found[i].room)));
        ui->table_search_va->setCellWidget(i, 6, btn);
    }
    ui->table_search_va->resizeColumnsToContents();
}

void Patron::show_my_books(){
    ui->table_my_books->setRowCount(0);
    QVector<Book> found;// = patron.get_checked_out_books();//CHANGE LATER
    for (int i = 0; i < found.size(); i++){
        ui->table_my_books->insertRow(i);
        QPushButton *btn_renew = new QPushButton(this);
        btn_renew->setText("renew");
        QSignalMapper *sm = new QSignalMapper(this);
        //connect(sm, SIGNAL(mapped(int)), this, SLOT(renew_book(int)));
        connect(btn_renew, SIGNAL(clicked()), sm, SLOT(map()));
        sm->setMapping(btn_renew, found[i].id);

        QPushButton *btn_return = new QPushButton(this);
        btn_return->setText("return");
        QSignalMapper *sm2 = new QSignalMapper(this);
        //connect(sm2, SIGNAL(mapped(int)), this, SLOT(return_book(int)));
        connect(btn_return, SIGNAL(clicked()), sm2, SLOT(map()));
        sm2->setMapping(btn_return, found[i].id);

        ui->table_my_books->setItem(i, 0, new QTableWidgetItem(QString::number(found[i].id)));
        ui->table_my_books->setItem(i, 1, new QTableWidgetItem(QString(found[i].title)));
        ui->table_my_books->setItem(i, 2, new QTableWidgetItem(QString(found[i].authors)));
        ui->table_my_books->setItem(i, 3, new QTableWidgetItem(QString(found[i].publisher)));
        ui->table_my_books->setItem(i, 4, new QTableWidgetItem(QString::number(found[i].price)));
        ui->table_my_books->setItem(i, 5, new QTableWidgetItem(found[i].bestseller ? "yes" : "no"));
        ui->table_my_books->setItem(i, 6, new QTableWidgetItem(QString::number(found[i].year)));
        ui->table_my_books->setItem(i, 7, new QTableWidgetItem(QString::number(found[i].copies)));
        ui->table_my_books->setItem(i, 8, new QTableWidgetItem(QString::number(found[i].room)));
        ui->table_my_books->setCellWidget(i, 9, btn_renew);
        ui->table_my_books->setCellWidget(i, 10, btn_return);
    }
    ui->table_my_books->resizeColumnsToContents();
}

void Patron::show_my_articles(){
    ui->table_my_articles->setRowCount(0);
    QVector<Article> found;// = patron.get_checked_out_articles();//CHANGE LATER
    for (int i = 0; i < found.size(); i++){
        ui->table_my_articles->insertRow(i);
        QPushButton *btn_renew = new QPushButton(this);
        btn_renew->setText("renew");
        QSignalMapper *sm = new QSignalMapper(this);
        //connect(sm, SIGNAL(mapped(int)), this, SLOT(renew_article(int)));
        connect(btn_renew, SIGNAL(clicked()), sm, SLOT(map()));
        sm->setMapping(btn_renew, found[i].id);

        QPushButton *btn_return = new QPushButton(this);
        btn_return->setText("return");
        QSignalMapper *sm2 = new QSignalMapper(this);
        //connect(sm2, SIGNAL(mapped(int)), this, SLOT(return_article(int)));
        connect(btn_return, SIGNAL(clicked()), sm2, SLOT(map()));
        sm2->setMapping(btn_return, found[i].id);

        ui->table_my_articles->setItem(i, 0, new QTableWidgetItem(QString::number(found[i].id)));
        ui->table_my_articles->setItem(i, 1, new QTableWidgetItem(QString(found[i].title)));
        ui->table_my_articles->setItem(i, 2, new QTableWidgetItem(QString(found[i].authors)));
        ui->table_my_articles->setItem(i, 3, new QTableWidgetItem(QString(found[i].publisher)));
        ui->table_my_articles->setItem(i, 4, new QTableWidgetItem(QString::number(found[i].price)));
        ui->table_my_articles->setItem(i, 5, new QTableWidgetItem(QString::number(found[i].year)));
        ui->table_my_articles->setItem(i, 6, new QTableWidgetItem(QString::number(found[i].copies)));
        ui->table_my_articles->setItem(i, 7, new QTableWidgetItem(QString::number(found[i].room)));
        ui->table_my_articles->setCellWidget(i, 8, btn_renew);
        ui->table_my_articles->setCellWidget(i, 9, btn_return);
    }
    ui->table_my_articles->resizeColumnsToContents();
}

void Patron::show_my_vas(){
    ui->table_my_vas->setRowCount(0);
    QVector<VA> found;// = patron.get_checked_out_vas();//CHANGE LATER
    for (int i = 0; i < found.size(); i++){
        ui->table_my_vas->insertRow(i);
        QPushButton *btn_renew = new QPushButton(this);
        btn_renew->setText("renew");
        QSignalMapper *sm = new QSignalMapper(this);
        //connect(sm, SIGNAL(mapped(int)), this, SLOT(renew_va(int)));
        connect(btn_renew, SIGNAL(clicked()), sm, SLOT(map()));
        sm->setMapping(btn_renew, found[i].id);

        QPushButton *btn_return = new QPushButton(this);
        btn_return->setText("return");
        QSignalMapper *sm2 = new QSignalMapper(this);
        //connect(sm2, SIGNAL(mapped(int)), this, SLOT(return_va(int)));
        connect(btn_return, SIGNAL(clicked()), sm2, SLOT(map()));
        sm2->setMapping(btn_return, found[i].id);

        ui->table_my_vas->setItem(i, 0, new QTableWidgetItem(QString::number(found[i].id)));
        ui->table_my_vas->setItem(i, 1, new QTableWidgetItem(QString(found[i].title)));
        ui->table_my_vas->setItem(i, 2, new QTableWidgetItem(QString(found[i].authors)));
        ui->table_my_vas->setItem(i, 3, new QTableWidgetItem(QString::number(found[i].price)));
        ui->table_my_vas->setItem(i, 4, new QTableWidgetItem(QString::number(found[i].copies)));
        ui->table_my_vas->setItem(i, 5, new QTableWidgetItem(QString::number(found[i].room)));
        ui->table_my_vas->setCellWidget(i, 6, btn_renew);
        ui->table_my_vas->setCellWidget(i, 7, btn_return);
    }
    ui->table_my_vas->resizeColumnsToContents();
}
