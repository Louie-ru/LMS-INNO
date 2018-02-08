#include "librarian.h"
#include "ui_librarian.h"
#include <QSignalMapper>
#include <mainwindow.h>
#include <QWidget>
#include <QFormLayout>
#include <QLabel>
#include <QDebug>
#include <QLineEdit>
#include <QMessageBox>

Librarian::Librarian(QWidget *parent) : QWidget(parent), ui(new Ui::Librarian){
    ui->setupUi(this);
}
Librarian::~Librarian(){
    delete ui;
}

//those are widgets for data input(new/modify documents and users)
QWidget *widget;
QLineEdit *line1;
QLineEdit *line2;
QLineEdit *line3;
QLineEdit *line4;
QLineEdit *line5;
QLineEdit *line6;
QLineEdit *line7;
QLineEdit *line8;
QLineEdit *line9;
QLineEdit *line10;
QComboBox *combo;
QCheckBox *check;
QPushButton *ok;
QPushButton *cancel;
QFormLayout *w_layout;
int global_id;

//make new input fields
void Librarian::clearObjects(){
    line1 = new QLineEdit();
    line2 = new QLineEdit();
    line3 = new QLineEdit();
    line4 = new QLineEdit();
    line5 = new QLineEdit();
    line6 = new QLineEdit();
    line7 = new QLineEdit();
    line8 = new QLineEdit();
    line9 = new QLineEdit();
    line10 = new QLineEdit();
    check = new QCheckBox();
    combo = new QComboBox();
    ok = new QPushButton("Ok");
    cancel = new QPushButton("Cancel");
    w_layout = new QFormLayout();
    w_layout->setLabelAlignment(Qt::AlignRight);
    connect(cancel, SIGNAL (clicked()),this, SLOT (closeWidget()));
    widget->setWindowFlags(Qt::WindowStaysOnTopHint);
}

void Librarian::on_button_search_patrons_clicked(){
    ui->table_patrons->setRowCount(0);
    QVector<PatronUser> found;// = librarian.get_patrons();//CHANGE LATER
    for (int i = 0; i < found.size(); i++){
        ui->table_patrons->insertRow(i);
        QPushButton *btn_modify = new QPushButton(this);
        btn_modify->setText("modify");
        QSignalMapper *sm = new QSignalMapper(this);
        connect(sm, SIGNAL(mapped(int)), this, SLOT(on_modify_patron_clicked(int)));
        connect(btn_modify, SIGNAL(clicked()), sm, SLOT(map()));
        sm->setMapping(btn_modify, found[i].id);

        QPushButton *btn_delete = new QPushButton(this);
        btn_delete->setText("delete");
        QSignalMapper *sm2 = new QSignalMapper(this);
        connect(sm2, SIGNAL(mapped(int)), this, SLOT(on_delete_patron_clicked(int)));
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
        connect(sm, SIGNAL(mapped(int)), this, SLOT(on_modify_librarian_clicled(int)));
        connect(btn_modify, SIGNAL(clicked()), sm, SLOT(map()));
        sm->setMapping(btn_modify, found[i].id);

        QPushButton *btn_delete = new QPushButton(this);
        btn_delete->setText("delete");
        QSignalMapper *sm2 = new QSignalMapper(this);
        connect(sm2, SIGNAL(mapped(int)), this, SLOT(on_delete_librarian_clicked(int)));
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
        connect(sm, SIGNAL(mapped(int)), this, SLOT(on_modify_book_clicked(int)));
        connect(btn_modify, SIGNAL(clicked()), sm, SLOT(map()));
        sm->setMapping(btn_modify, found[i].id);

        QPushButton *btn_delete = new QPushButton(this);
        btn_delete->setText("delete");
        QSignalMapper *sm2 = new QSignalMapper(this);
        connect(sm2, SIGNAL(mapped(int)), this, SLOT(on_delete_book_clicked(int)));
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
        connect(sm, SIGNAL(mapped(int)), this, SLOT(on_modify_article_clicked(int)));
        connect(btn_modify, SIGNAL(clicked()), sm, SLOT(map()));
        sm->setMapping(btn_modify, found[i].id);

        QPushButton *btn_delete = new QPushButton(this);
        btn_delete->setText("delete");
        QSignalMapper *sm2 = new QSignalMapper(this);
        connect(sm2, SIGNAL(mapped(int)), this, SLOT(on_delete_article_clicked(int)));
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
        connect(sm, SIGNAL(mapped(int)), this, SLOT(on_modify_va_clicked(int)));
        connect(btn_modify, SIGNAL(clicked()), sm, SLOT(map()));
        sm->setMapping(btn_modify, found[i].id);

        QPushButton *btn_delete = new QPushButton(this);
        btn_delete->setText("delete");
        QSignalMapper *sm2 = new QSignalMapper(this);
        connect(sm2, SIGNAL(mapped(int)), this, SLOT(on_delete_va_clicked(int)));
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

void Librarian::on_modify_patron_clicked(int user_id){
    if (widget != NULL && !widget->isHidden()) return;
    global_id = user_id;
    widget = new QWidget();
    QLabel *id = new QLabel("card id:");
    QLabel *name = new QLabel("name:");
    QLabel *address = new QLabel("address:");
    QLabel *phone = new QLabel("phone:");
    QLabel *faculty = new QLabel("faculty:");
    clearObjects();
    w_layout->addRow(id, line1);
    w_layout->addRow(name, line2);
    w_layout->addRow(address, line3);
    w_layout->addRow(phone, line4);
    w_layout->addRow(faculty, check);
    w_layout->addRow(cancel, ok);
    connect(ok, SIGNAL (clicked()),this, SLOT (modifyPatron()));
    widget->setLayout(w_layout);
    widget->show();
}
void Librarian::on_modify_librarian_clicked(int user_id){
    if (widget != NULL && !widget->isHidden()) return;
    global_id = user_id;
    widget = new QWidget();
    QLabel *id = new QLabel("card id:");
    QLabel *name = new QLabel("name:");
    QLabel *address = new QLabel("address:");
    QLabel *phone = new QLabel("phone:");
    clearObjects();
    w_layout->addRow(id, line1);
    w_layout->addRow(name, line2);
    w_layout->addRow(address, line3);
    w_layout->addRow(phone, line4);
    w_layout->addRow(cancel, ok);
    connect(ok, SIGNAL (clicked()),this, SLOT (modifyLibrarian()));
    widget->setLayout(w_layout);
    widget->show();
}
void Librarian::on_modify_book_clicked(int id){
    if (widget != NULL && !widget->isHidden()) return;
    widget = new QWidget();
    QLabel *title = new QLabel("title:");
    QLabel *authors = new QLabel("authors:");
    QLabel *publisher = new QLabel("publisher:");
    QLabel *year = new QLabel("year:");
    QLabel *price = new QLabel("price:");
    QLabel *room = new QLabel("room:");
    QLabel *level = new QLabel("level:");
    QLabel *copies = new QLabel("copies:");
    QLabel *bestseller = new QLabel("bestseller:");
    clearObjects();
    w_layout = new QFormLayout();
    w_layout->addRow(title, line1);
    w_layout->addRow(authors, line2);
    w_layout->addRow(publisher, line3);
    w_layout->addRow(year, line4);
    w_layout->addRow(price, line5);
    w_layout->addRow(room, line6);
    w_layout->addRow(level, line7);
    w_layout->addRow(copies, line8);
    w_layout->addRow(bestseller, check);
    w_layout->addRow(cancel, ok);
    connect(ok, SIGNAL (clicked()),this, SLOT (modifyBook()));
    widget->setLayout(w_layout);
    widget->show();
}
void Librarian::on_modify_article_clicked(int id){
    if (widget != NULL && !widget->isHidden()) return;
    widget = new QWidget();
    QLabel *title = new QLabel("title:");
    QLabel *authors = new QLabel("authors:");
    QLabel *publisher = new QLabel("publisher:");
    QLabel *journal = new QLabel("journal:");
    QLabel *editors = new QLabel("editors:");
    QLabel *year = new QLabel("year:");
    QLabel *month = new QLabel("month:");
    QLabel *price = new QLabel("price:");
    QLabel *room = new QLabel("room:");
    QLabel *level = new QLabel("level:");
    QLabel *copies = new QLabel("copies:");
    clearObjects();
    combo->addItem("January");
    combo->addItem("February");
    combo->addItem("March");
    combo->addItem("April");
    combo->addItem("May");
    combo->addItem("June");
    combo->addItem("July");
    combo->addItem("August");
    combo->addItem("September");
    combo->addItem("October");
    combo->addItem("November");
    combo->addItem("December");
    w_layout->addRow(title, line1);
    w_layout->addRow(authors, line2);
    w_layout->addRow(publisher, line3);
    w_layout->addRow(journal, line4);
    w_layout->addRow(editors, line5);
    w_layout->addRow(year, line6);
    w_layout->addRow(month, combo);
    w_layout->addRow(price, line7);
    w_layout->addRow(room, line8);
    w_layout->addRow(level, line9);
    w_layout->addRow(copies, line10);
    w_layout->addRow(cancel, ok);
    connect(ok, SIGNAL (clicked()),this, SLOT (modifyArticle()));
    widget->setLayout(w_layout);
    widget->show();
}
void Librarian::on_modify_va_clicked(int id){
    if (widget != NULL && !widget->isHidden()) return;
    widget = new QWidget();
    QLabel *title = new QLabel("title:");
    QLabel *authors = new QLabel("authors:");
    QLabel *price = new QLabel("price:");
    QLabel *room = new QLabel("room:");
    QLabel *level = new QLabel("level:");
    QLabel *copies = new QLabel("copies:");
    clearObjects();
    w_layout->addRow(title, line1);
    w_layout->addRow(authors, line2);
    w_layout->addRow(price, line3);
    w_layout->addRow(room, line4);
    w_layout->addRow(level, line5);
    w_layout->addRow(copies, line6);
    w_layout->addRow(cancel, ok);
    connect(ok, SIGNAL (clicked()),this, SLOT (modifyVA()));
    widget->setLayout(w_layout);
    widget->show();
}

void Librarian::on_delete_patron_clicked(int id){
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Delete", "Are you sure you want to delete this patron?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No) return;
}
void Librarian::on_delete_librarian_clicked(int id){
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Delete", "Are you sure you want to delete this librarian?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No) return;
}
void Librarian::on_delete_book_clicked(int id){
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Delete", "Are you sure you want to delete this book?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No) return;
}
void Librarian::on_delete_article_clicked(int id){
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Delete", "Are you sure you want to delete this article?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No) return;
}
void Librarian::on_delete_va_clicked(int id){
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Delete", "Are you sure you want to delete this va?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No) return;
}

void Librarian::on_button_show_checked_out_books_clicked(){
    ui->table_checked_out_books->setRowCount(0);
    QVector<std::pair<Check_out, Book> > found;// = librarian.get_checked_out_books();//CHANGE LATER
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
        ui->table_checked_out_books->setItem(i, 7, new QTableWidgetItem(QString::number(found[i].second.level)));\
        ui->table_checked_out_books->setItem(i, 8, new QTableWidgetItem(found[i].second.bestseller ? "yes" : "no"));
        ui->table_checked_out_books->setItem(i, 9, new QTableWidgetItem(QString::number(found[i].second.copies)));
        ui->table_checked_out_books->setItem(i, 10, new QTableWidgetItem(date_start));
        ui->table_checked_out_books->setItem(i, 11, new QTableWidgetItem(date_end));
        ui->table_checked_out_books->setItem(i, 12, new QTableWidgetItem(QString::number(found[i].first.fine)));
    }
    ui->table_checked_out_books->resizeColumnsToContents();
}
void Librarian::on_button_show_checked_out_articles_clicked(){
    ui->table_checked_out_articles->setRowCount(0);
    QVector<std::pair<Check_out, Article> > found;// = librarian.get_checked_out_articles();//CHANGE LATER
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
    QVector<std::pair<Check_out, VA> > found;// = librarian.get_checked_out_vas();//CHANGE LATER
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

void Librarian::modifyPatron(){
    QString name = line2->text();
    QString address = line3->text();
    QString phone = line4->text();
    bool faculty = check->isChecked();
    closeWidget();
}
void Librarian::modifyLibrarian(){
    QString name = line2->text();
    QString address = line3->text();
    QString phone = line4->text();
    closeWidget();
}
void Librarian::modifyBook(){
    QString title = line1->text();
    QString authors = line2->text();
    QString publisher = line3->text();
    QString year = line4->text();
    QString price = line5->text();
    QString room = line6->text();
    QString level = line7->text();
    QString copies = line8->text();
    bool bestseller = check->isChecked();
    closeWidget();
}
void Librarian::modifyArticle(){
    QString title = line1->text();
    QString authors = line2->text();
    QString publisher = line3->text();
    QString journal = line4->text();
    QString editors = line5->text();
    QString year = line6->text();
    QString month = combo->currentText();
    QString price = line7->text();
    QString room = line8->text();
    QString level = line9->text();
    QString copies = line10->text();
}
void Librarian::modifyVA(){
    QString title = line1->text();
    QString authors = line2->text();
    QString price = line3->text();
    QString room = line4->text();
    QString level = line5->text();
    QString copies = line6->text();
    closeWidget();
}

void Librarian::closeWidget(){
    widget->close();
}

void Librarian::createPatron(){
    QString id = line1->text();
    QString name = line2->text();
    QString address = line3->text();
    QString phone = line4->text();
    bool faculty = check->isChecked();
    closeWidget();
}
void Librarian::createLibrarian(){
    QString id = line1->text();
    QString name = line2->text();
    QString address = line3->text();
    QString phone = line4->text();
    closeWidget();
}
void Librarian::createBook(){
    QString title = line1->text();
    QString authors = line2->text();
    QString publisher = line3->text();
    QString year = line4->text();
    QString price = line5->text();
    QString room = line6->text();
    QString level = line7->text();
    QString copies = line8->text();
    bool bestseller = check->isChecked();
    closeWidget();
}
void Librarian::createArticle(){
    QString title = line1->text();
    QString authors = line2->text();
    QString publisher = line3->text();
    QString journal = line4->text();
    QString editors = line5->text();
    QString year = line6->text();
    QString month = combo->currentText();
    QString price = line7->text();
    QString room = line8->text();
    QString level = line9->text();
    QString copies = line10->text();
    closeWidget();
}
void Librarian::createVA(){
    QString title = line1->text();
    QString authors = line2->text();
    QString price = line3->text();
    QString room = line4->text();
    QString level = line5->text();
    QString copies = line6->text();
    closeWidget();
}

void Librarian::on_button_new_patron_clicked(){
    if (widget != NULL && !widget->isHidden()) return;
    widget = new QWidget();
    QLabel *id = new QLabel("card id:");
    QLabel *name = new QLabel("name:");
    QLabel *address = new QLabel("address:");
    QLabel *phone = new QLabel("phone:");
    QLabel *faculty = new QLabel("faculty:");
    clearObjects();
    w_layout->addRow(id, line1);
    w_layout->addRow(name, line2);
    w_layout->addRow(address, line3);
    w_layout->addRow(phone, line4);
    w_layout->addRow(faculty, check);
    w_layout->addRow(cancel, ok);
    connect(ok, SIGNAL (clicked()),this, SLOT (createPatron()));
    widget->setLayout(w_layout);
    widget->show();
}
void Librarian::on_button_new_librarian_clicked(){
    if (widget != NULL && !widget->isHidden()) return;
    widget = new QWidget();
    QLabel *id = new QLabel("card id:");
    QLabel *name = new QLabel("name:");
    QLabel *address = new QLabel("address:");
    QLabel *phone = new QLabel("phone:");
    clearObjects();
    w_layout->addRow(id, line1);
    w_layout->addRow(name, line2);
    w_layout->addRow(address, line3);
    w_layout->addRow(phone, line4);
    w_layout->addRow(cancel, ok);
    connect(ok, SIGNAL (clicked()),this, SLOT (createLibrarian()));
    widget->setLayout(w_layout);
    widget->show();
}
void Librarian::on_button_new_book_clicked(){
    if (widget != NULL && !widget->isHidden()) return;
    widget = new QWidget();
    QLabel *title = new QLabel("title:");
    QLabel *authors = new QLabel("authors:");
    QLabel *publisher = new QLabel("publisher:");
    QLabel *year = new QLabel("year:");
    QLabel *price = new QLabel("price:");
    QLabel *room = new QLabel("room:");
    QLabel *level = new QLabel("level:");
    QLabel *copies = new QLabel("copies:");
    QLabel *bestseller = new QLabel("bestseller:");
    clearObjects();
    w_layout = new QFormLayout();
    w_layout->addRow(title, line1);
    w_layout->addRow(authors, line2);
    w_layout->addRow(publisher, line3);
    w_layout->addRow(year, line4);
    w_layout->addRow(price, line5);
    w_layout->addRow(room, line6);
    w_layout->addRow(level, line7);
    w_layout->addRow(copies, line8);
    w_layout->addRow(bestseller, check);
    w_layout->addRow(cancel, ok);
    connect(ok, SIGNAL (clicked()),this, SLOT (createBook()));
    widget->setLayout(w_layout);
    widget->show();
}
void Librarian::on_button_new_article_clicked(){
    if (widget != NULL && !widget->isHidden()) return;
    widget = new QWidget();
    QLabel *title = new QLabel("title:");
    QLabel *authors = new QLabel("authors:");
    QLabel *publisher = new QLabel("publisher:");
    QLabel *journal = new QLabel("journal:");
    QLabel *editors = new QLabel("editors:");
    QLabel *year = new QLabel("year:");
    QLabel *month = new QLabel("month:");
    QLabel *price = new QLabel("price:");
    QLabel *room = new QLabel("room:");
    QLabel *level = new QLabel("level:");
    QLabel *copies = new QLabel("copies:");
    clearObjects();
    combo->addItem("January");
    combo->addItem("February");
    combo->addItem("March");
    combo->addItem("April");
    combo->addItem("May");
    combo->addItem("June");
    combo->addItem("July");
    combo->addItem("August");
    combo->addItem("September");
    combo->addItem("October");
    combo->addItem("November");
    combo->addItem("December");
    w_layout->addRow(title, line1);
    w_layout->addRow(authors, line2);
    w_layout->addRow(publisher, line3);
    w_layout->addRow(journal, line4);
    w_layout->addRow(editors, line5);
    w_layout->addRow(year, line6);
    w_layout->addRow(month, combo);
    w_layout->addRow(price, line7);
    w_layout->addRow(room, line8);
    w_layout->addRow(level, line9);
    w_layout->addRow(copies, line10);
    w_layout->addRow(cancel, ok);
    connect(ok, SIGNAL (clicked()),this, SLOT (createArticle()));
    widget->setLayout(w_layout);
    widget->show();
}
void Librarian::on_button_new_va_clicked(){
    if (widget != NULL && !widget->isHidden()) return;
    widget = new QWidget();
    QLabel *title = new QLabel("title:");
    QLabel *authors = new QLabel("authors:");
    QLabel *price = new QLabel("price:");
    QLabel *room = new QLabel("room:");
    QLabel *level = new QLabel("level:");
    QLabel *copies = new QLabel("copies:");
    clearObjects();
    w_layout->addRow(title, line1);
    w_layout->addRow(authors, line2);
    w_layout->addRow(price, line3);
    w_layout->addRow(room, line4);
    w_layout->addRow(level, line5);
    w_layout->addRow(copies, line6);
    w_layout->addRow(cancel, ok);
    connect(ok, SIGNAL (clicked()),this, SLOT (createVA()));
    widget->setLayout(w_layout);
    widget->show();
}

void Librarian::on_button_logout_clicked(){
    MainWindow *mainwindow = new MainWindow();
    mainwindow->show();
    this->close();
}
