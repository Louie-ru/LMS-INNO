#include "librarian.h"
#include "ui_librarian.h"
#include <skeleton.cpp>
#include <QSignalMapper>

Librarian::Librarian(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Librarian)
{
    ui->setupUi(this);
}

Librarian::~Librarian()
{
    delete ui;
}

void Librarian::on_button_search_patrons_clicked(){
    ui->table_patrons->setRowCount(0);
    QVector<PatronUser> found;// = librarian.get_patrons();//CHANGE LATER
    for (int i = 0; i < found.size(); i++){
        ui->table_patrons->insertRow(i);
        QPushButton *btn_modify = new QPushButton(this);
        btn_modify->setText("modify");
        QSignalMapper *sm = new QSignalMapper(this);
        //connect(sm, SIGNAL(mapped(int)), this, SLOT(modify_patron(int)));
        connect(btn_modify, SIGNAL(clicked()), sm, SLOT(map()));
        sm->setMapping(btn_modify, found[i].id);

        QPushButton *btn_delete = new QPushButton(this);
        btn_delete->setText("delete");
        QSignalMapper *sm2 = new QSignalMapper(this);
        //connect(sm2, SIGNAL(mapped(int)), this, SLOT(delete_patron(int)));
        connect(btn_delete, SIGNAL(clicked()), sm2, SLOT(map()));
        sm2->setMapping(btn_delete, found[i].id);

        ui->table_patrons->setItem(i, 0, new QTableWidgetItem(QString::number(found[i].id)));
        ui->table_patrons->setItem(i, 1, new QTableWidgetItem(found[i].name));
        ui->table_patrons->setItem(i, 2, new QTableWidgetItem(found[i].address));
        ui->table_patrons->setItem(i, 3, new QTableWidgetItem(found[i].phone));
        ui->table_patrons->setItem(i, 4, new QTableWidgetItem(found[i].faculty ? "yes" : "no"));
        ui->table_patrons->setItem(i, 5, new QTableWidgetItem(found[i].login));
        ui->table_patrons->setItem(i, 6, new QTableWidgetItem(found[i].password));
        ui->table_patrons->setCellWidget(i, 7, btn_modify);
        ui->table_patrons->setCellWidget(i, 8, btn_delete);
    }
    ui->table_patrons->resizeColumnsToContents();
}

void Librarian::on_button_search_librarians_clicked(){
    ui->table_librarian->setRowCount(0);
    QVector<LibrarianUser> found;// = librarian.get_librarians();//CHANGE LATER
    for (int i = 0; i < found.size(); i++){
        ui->table_librarian->insertRow(i);
        QPushButton *btn_modify = new QPushButton(this);
        btn_modify->setText("modify");
        QSignalMapper *sm = new QSignalMapper(this);
        //connect(sm, SIGNAL(mapped(int)), this, SLOT(modify_librarian(int)));
        connect(btn_modify, SIGNAL(clicked()), sm, SLOT(map()));
        sm->setMapping(btn_modify, found[i].id);

        QPushButton *btn_delete = new QPushButton(this);
        btn_delete->setText("delete");
        QSignalMapper *sm2 = new QSignalMapper(this);
        //connect(sm2, SIGNAL(mapped(int)), this, SLOT(delete_librarian(int)));
        connect(btn_delete, SIGNAL(clicked()), sm2, SLOT(map()));
        sm2->setMapping(btn_delete, found[i].id);

        ui->table_librarian->setItem(i, 0, new QTableWidgetItem(QString::number(found[i].id)));
        ui->table_librarian->setItem(i, 1, new QTableWidgetItem(found[i].name));
        ui->table_librarian->setItem(i, 2, new QTableWidgetItem(found[i].address));
        ui->table_librarian->setItem(i, 3, new QTableWidgetItem(found[i].phone));
        ui->table_librarian->setItem(i, 4, new QTableWidgetItem(found[i].login));
        ui->table_librarian->setItem(i, 5, new QTableWidgetItem(found[i].password));
        ui->table_librarian->setCellWidget(i, 6, btn_modify);
        ui->table_librarian->setCellWidget(i, 7, btn_delete);
    }
    ui->table_librarian->resizeColumnsToContents();
}

void Librarian::on_button_search_books_clicked(){
    ui->table_search_books->setRowCount(0);
    QVector<Book> found;// = librarian.search_books();//CHANGE LATER
    for (int i = 0; i < found.size(); i++){
        ui->table_search_books->insertRow(i);
        QPushButton *btn_modify = new QPushButton(this);
        btn_modify->setText("modify");
        QSignalMapper *sm = new QSignalMapper(this);
        //connect(sm, SIGNAL(mapped(int)), this, SLOT(modify_book(int)));
        connect(btn_modify, SIGNAL(clicked()), sm, SLOT(map()));
        sm->setMapping(btn_modify, found[i].id);

        QPushButton *btn_delete = new QPushButton(this);
        btn_delete->setText("delete");
        QSignalMapper *sm2 = new QSignalMapper(this);
        //connect(sm2, SIGNAL(mapped(int)), this, SLOT(delete_book(int)));
        connect(btn_delete, SIGNAL(clicked()), sm2, SLOT(map()));
        sm2->setMapping(btn_delete, found[i].id);

        ui->table_search_books->setItem(i, 0, new QTableWidgetItem(found[i].title));
        ui->table_search_books->setItem(i, 1, new QTableWidgetItem(found[i].authors));
        ui->table_search_books->setItem(i, 2, new QTableWidgetItem(found[i].publisher));
        ui->table_search_books->setItem(i, 3, new QTableWidgetItem(QString::number(found[i].year)));
        ui->table_search_books->setItem(i, 4, new QTableWidgetItem(QString::number(found[i].price)));
        ui->table_search_books->setItem(i, 5, new QTableWidgetItem(QString::number(found[i].room)));
        ui->table_search_books->setItem(i, 6, new QTableWidgetItem(QString::number(found[i].level)));
        ui->table_search_books->setItem(i, 7, new QTableWidgetItem(QString::number(found[i].copies)));
        ui->table_search_books->setItem(i, 8, new QTableWidgetItem(found[i].bestseller ? "yes" : "no"));
        ui->table_search_books->setCellWidget(i, 9, btn_modify);
        ui->table_search_books->setCellWidget(i, 10, btn_delete);
    }
    ui->table_search_books->resizeColumnsToContents();
}

void Librarian::on_button_search_articles_clicked(){
    ui->table_search_articles->setRowCount(0);
    QVector<Article> found;// = librarian.search_articles();//CHANGE LATER
    for (int i = 0; i < found.size(); i++){
        ui->table_search_articles->insertRow(i);
        QPushButton *btn_modify = new QPushButton(this);
        btn_modify->setText("modify");
        QSignalMapper *sm = new QSignalMapper(this);
        //connect(sm, SIGNAL(mapped(int)), this, SLOT(modify_article(int)));
        connect(btn_modify, SIGNAL(clicked()), sm, SLOT(map()));
        sm->setMapping(btn_modify, found[i].id);

        QPushButton *btn_delete = new QPushButton(this);
        btn_delete->setText("delete");
        QSignalMapper *sm2 = new QSignalMapper(this);
        //connect(sm2, SIGNAL(mapped(int)), this, SLOT(delete_article(int)));
        connect(btn_delete, SIGNAL(clicked()), sm2, SLOT(map()));
        sm2->setMapping(btn_delete, found[i].id);

        ui->table_search_articles->setItem(i, 0, new QTableWidgetItem(found[i].title));
        ui->table_search_articles->setItem(i, 1, new QTableWidgetItem(found[i].authors));
        ui->table_search_articles->setItem(i, 2, new QTableWidgetItem(found[i].publisher));
        ui->table_search_articles->setItem(i, 3, new QTableWidgetItem(found[i].journal_title));
        ui->table_search_articles->setItem(i, 4, new QTableWidgetItem(found[i].editors));
        ui->table_search_articles->setItem(i, 5, new QTableWidgetItem(QString::number(found[i].year)));
        ui->table_search_articles->setItem(i, 6, new QTableWidgetItem(QString::number(found[i].month)));
        ui->table_search_articles->setItem(i, 7, new QTableWidgetItem(QString::number(found[i].price)));
        ui->table_search_articles->setItem(i, 8, new QTableWidgetItem(QString::number(found[i].room)));
        ui->table_search_articles->setItem(i, 9, new QTableWidgetItem(QString::number(found[i].level)));
        ui->table_search_articles->setItem(i, 10, new QTableWidgetItem(QString::number(found[i].copies)));
        ui->table_search_articles->setCellWidget(i, 11, btn_modify);
        ui->table_search_articles->setCellWidget(i, 12, btn_delete);
    }
    ui->table_search_articles->resizeColumnsToContents();
}

void Librarian::on_button_search_vas_clicked(){
    ui->table_search_va->setRowCount(0);
    QVector<VA> found;// = librarian.search_vas();//CHANGE LATER
    for (int i = 0; i < found.size(); i++){
        ui->table_search_va->insertRow(i);
        QPushButton *btn_modify = new QPushButton(this);
        btn_modify->setText("modify");
        QSignalMapper *sm = new QSignalMapper(this);
        //connect(sm, SIGNAL(mapped(int)), this, SLOT(modify_va(int)));
        connect(btn_modify, SIGNAL(clicked()), sm, SLOT(map()));
        sm->setMapping(btn_modify, found[i].id);

        QPushButton *btn_delete = new QPushButton(this);
        btn_delete->setText("delete");
        QSignalMapper *sm2 = new QSignalMapper(this);
        //connect(sm2, SIGNAL(mapped(int)), this, SLOT(delete_va(int)));
        connect(btn_delete, SIGNAL(clicked()), sm2, SLOT(map()));
        sm2->setMapping(btn_delete, found[i].id);

        ui->table_search_va->setItem(i, 0, new QTableWidgetItem(found[i].title));
        ui->table_search_va->setItem(i, 1, new QTableWidgetItem(found[i].authors));
        ui->table_search_va->setItem(i, 2, new QTableWidgetItem(QString::number(found[i].price)));
        ui->table_search_va->setItem(i, 3, new QTableWidgetItem(QString::number(found[i].room)));
        ui->table_search_va->setItem(i, 4, new QTableWidgetItem(QString::number(found[i].level)));
        ui->table_search_va->setItem(i, 5, new QTableWidgetItem(QString::number(found[i].copies)));
        ui->table_search_va->setCellWidget(i, 6, btn_modify);
        ui->table_search_va->setCellWidget(i, 7, btn_delete);
    }
    ui->table_search_va->resizeColumnsToContents();
}

void Librarian::on_button_show_checked_out_books_clicked(){
    ui->table_checked_out_books->setRowCount(0);
    QVector<std::pair<Check_out, Book>> found;// = librarian.get_checked_out_books();//CHANGE LATER
    for (int i = 0; i < found.size(); i++){
        ui->table_checked_out_books->insertRow(i);
        QString date_start = QString::number(found[i].first.day_start)+"."+QString::number(found[i].first.month_start)+"."+QString::number(found[i].first.year_start);
        QString date_end = QString::number(found[i].first.day_end)+"."+QString::number(found[i].first.month_end)+"."+QString::number(found[i].first.year_end);
        ui->table_checked_out_books->setItem(i, 0, new QTableWidgetItem(QString::number(found[i].first.user_id)));
        ui->table_checked_out_books->setItem(i, 1, new QTableWidgetItem(found[i].second.title));
        ui->table_checked_out_books->setItem(i, 2, new QTableWidgetItem(found[i].second.authors));
        ui->table_checked_out_books->setItem(i, 3, new QTableWidgetItem(found[i].second.publisher));
        ui->table_checked_out_books->setItem(i, 4, new QTableWidgetItem(QString::number(found[i].second.year)));
        ui->table_checked_out_books->setItem(i, 5, new QTableWidgetItem(QString::number(found[i].second.price)));
        ui->table_checked_out_books->setItem(i, 6, new QTableWidgetItem(QString::number(found[i].second.room)));
        ui->table_checked_out_books->setItem(i, 7, new QTableWidgetItem(QString::number(found[i].second.level)));
        ui->table_checked_out_books->insertRow(i);
        QString date_start = QString::number(found[i].first.day_start)+"."+QString::number(found[i].first.month_start)+"."+QString::number(found[i].first.year_start);
        QString date_end =ii->table_checked_out_books->insertRow(i);
        QString date_start = QString::number(found[i].first.day_start)+"."+QString::number(found[i].first.month_start)+"."+QString::number(found[i].first.year_start);
        QString date_end = QString::number(found[i].first.day_end)+"."+QString::number(found[i].first.month_end)+"."+QString::number(found[i].first.year_end);
        ui->table_checked_out_books->setItem(i, 0, new QTableWidgetItem(QString::number(found[i].first.user_id)));
      ->table_checked_out_books->insertRow(i);
        QStringng date_start = QString::number(found[i].first.day_start)+"."+QString::number(found[i].first.month_start)+"."+QString::number(found[i].first.year_start);
        QString i->table_checked_out_books->insertRow(i);
        QString date_start = QString::number(found[i].first.day_start)+"."+QString::number(found[i].first.month_start)+"."+QString::number(found[i].first.year_start);
        QString date_end = QString::number(found[i].first.day_end)+"."+QString::number(found[i].first.month_end)+"."+QString::number(found[i].first.year_end);
        ui->table_checked_out_books->setItem(i, 0, new QTableWidgetItem(QString::number(found[i].first.user_id)));
      date_end = QString::number(found[i].first.day_end)+"."+QString::number(found[i].first.month_end)+"."+QString::number(found[i].first.year_end);
        ui->table_checked_out_books->setItem(i, 0, new QTableWidgetItem(QString::number(found[i].first.user_id)));
       QString::number(found[i].first.day_end)+"."+QString::number(found[i].first.month_end)+"."+QString::number(found[i].first.year_end);
        ui->table_checked_out_books->setItem(i, 0, new QTableWidgetItem(QString::number(found[i].first.user_id)));
      i->table_checked_out_books->setItem(i, 8, new QTableWidgetItem(found[i].second.bestseller ? "yes" : "no"));
        ui->table_checked_out_books->setItem(i, 9, new QTableWidgetItem(QString::number(found[i].second.copies)));
        ui->table_checked_out_books->setItem(i, 10, new QTableWidgetItem(date_start));
        ui->table_checked_out_books->setItem(i, 11, new QTableWidgetItem(date_end));
        ui->table_checked_out_books->setItem(i, 12, new QTableWidgetItem(QString::number(found[i].first.fine)));
    }
    ui->table_checked_out_books->resizeColumnsToContents();
}

void Librarian::on_button_show_checked_out_articles_clicked(){
    ui->table_checked_out_articles->setRowCount(0);
    QVector<std::pair<Check_out, Article>> found;// = librarian.get_checked_out_articles();//CHANGE LATER
    for (int i = 0; i < found.size(); i++){
        QString date_start = QString::number(found[i].first.day_start)+"."+QString::number(found[i].first.month_start)+"."+QString::number(found[i].first.year_start);
        QString date_end = QString::number(found[i].first.day_end)+"."+QString::number(found[i].first.month_end)+"."+QString::number(found[i].first.year_end);
        ui->table_checked_out_articles->insertRow(i);
        ui->table_checked_out_articles->setItem(i, 0, new QTableWidgetItem(QString::number(found[i].first.user_id)));
        ui->table_checked_out_articles->setItem(i, 1, new QTableWidgetItem(found[i].second.title));
        ui->table_checked_out_articles->setItem(i, 2, new QTableWidgetItem(found[i].second.authors));
        ui->table_checked_out_articles->setItem(i, 3, new QTableWidgetItem(found[i].second.publisher));
        ui->table_checked_out_articles->setItem(i, 4, new QTableWidgetItem(found[i].second.journal_title));
        ui->table_checked_out_articles->setItem(i, 5, new QTableWidgetItem(found[i].second.editors));
        ui->table_checked_out_articles->setItem(i, 6, new QTableWidgetItem(QString::number(found[i].second.year)));
        ui->table_checked_out_articles->setItem(i, 7, new QTableWidgetItem(QString::number(found[i].second.month)));
        ui->table_checked_out_articles->setItem(i, 8, new QTableWidgetItem(QString::number(found[i].second.price)));
        ui->table_checked_out_articles->setItem(i, 9, new QTableWidgetItem(QString::number(found[i].second.room)));
        ui->table_checked_out_articles->setItem(i, 10, new QTableWidgetItem(QString::number(found[i].second.level)));
        ui->table_checked_out_articles->setItem(i, 11, new QTableWidgetItem(date_start));
        ui->table_checked_out_articles->setItem(i, 12, new QTableWidgetItem(date_end));
        ui->table_checked_out_articles->setItem(i, 13, new QTableWidgetItem(QString::number(found[i].first.fine)));
    }
    ui->table_checked_out_articles->resizeColumnsToContents();
}

void Librarian::on_button_show_checked_out_vas_clicked(){
    ui->table_checked_out_vas->setRowCount(0);
    QVector<std::pair<Check_out, VA>> found;// = librarian.get_checked_out_vas();//CHANGE LATER
    for (int i = 0; i < found.size(); i++){
        QString date_start = QString::number(found[i].first.day_start)+"."+QString::number(found[i].first.month_start)+"."+QString::number(found[i].first.year_start);
        QString date_end = QString::number(found[i].first.day_end)+"."+QString::number(found[i].first.month_end)+"."+QString::number(found[i].first.year_end);
        ui->table_checked_out_vas->insertRow(i);
        ui->table_checked_out_vas->setItem(i, 0, new QTableWidgetItem(QString::number(found[i].first.user_id)));
        ui->table_checked_out_vas->setItem(i, 1, new QTableWidgetItem(found[i].second.title));
        ui->table_checked_out_vas->setItem(i, 2, new QTableWidgetItem(found[i].second.authors));
        ui->table_checked_out_vas->setItem(i, 3, new QTableWidgetItem(QString::number(found[i].second.price)));
        ui->table_checked_out_vas->setItem(i, 4, new QTableWidgetItem(QString::number(found[i].second.room)));
        ui->table_checked_out_vas->setItem(i, 5, new QTableWidgetItem(QString::number(found[i].second.level)));
        ui->table_checked_out_vas->setItem(i, 6, new QTableWidgetItem(date_start));
        ui->table_checked_out_vas->setItem(i, 7, new QTableWidgetItem(date_end));
        ui->table_checked_out_vas->setItem(i, 8, new QTableWidgetItem(QString::number(found[i].first.fine)));
    }
    ui->table_checked_out_vas->resizeColumnsToContents();
}
