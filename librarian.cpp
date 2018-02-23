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
    ui->line_year_books->setValidator(new QIntValidator(0, 2100, this));
    ui->line_year_articles->setValidator(new QIntValidator(0, 2100, this));
    ui->line_year_books_checked->setValidator(new QIntValidator(0, 2100, this));
    ui->line_year_checked_out_articles->setValidator(new QIntValidator(0, 2100, this));
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
QLineEdit *line11;
QLineEdit *line12;
QComboBox *combo;
QCheckBox *check;
QCheckBox *check2;
QPushButton *ok;
QPushButton *cancel;
QFormLayout *w_layout;

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
    line11 = new QLineEdit();
    line12 = new QLineEdit();
    check = new QCheckBox();
    check2 = new QCheckBox();
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

    int user_id = ui->line_patrons_id->text().toInt();
    QString name = ui->line_patrons_name->text();
    QString phone = ui->line_patrons_phone->text();
    QString address = ui->line_patrons_address->text();
    bool faculty = ui->checkbox_patrons_faculty->isChecked();
    bool or_and = ui->checkbox_patrons_criteria->isChecked();

    QVector<PatronUser> found = me.search_patrons(user_id, name, address, phone, faculty, or_and);
    for (int i = 0; i < found.size(); i++){
        ui->table_patrons->insertRow(i);
        QPushButton *btn_modify = new QPushButton(this);
        btn_modify->setText("modify");
        QSignalMapper *sm = new QSignalMapper(this);
        connect(sm, SIGNAL(mapped(int)), this, SLOT(modify_patron_clicked(int)));
        connect(btn_modify, SIGNAL(clicked()), sm, SLOT(map()));
        sm->setMapping(btn_modify, found[i].id);

        QPushButton *btn_delete = new QPushButton(this);
        btn_delete->setText("delete");
        QSignalMapper *sm2 = new QSignalMapper(this);
        connect(sm2, SIGNAL(mapped(int)), this, SLOT(delete_patron_clicked(int)));
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

    int user_id = ui->line_librarian_id->text().toInt();
    QString name = ui->line_librarian_name->text();
    QString phone = ui->line_librarian_phone->text();
    QString address = ui->line_librarian_adress->text();
    bool or_and = ui->checkbox_librarian_criteria->isChecked();

    QVector<LibrarianUser> found = me.search_librarians(user_id, name, address, phone, or_and);
    for (int i = 0; i < found.size(); i++){
        ui->table_librarian->insertRow(i);
        QPushButton *btn_modify = new QPushButton(this);
        btn_modify->setText("modify");
        QSignalMapper *sm = new QSignalMapper(this);
        connect(sm, SIGNAL(mapped(int)), this, SLOT(modify_librarian_clicled(int)));
        connect(btn_modify, SIGNAL(clicked()), sm, SLOT(map()));
        sm->setMapping(btn_modify, found[i].id);

        QPushButton *btn_delete = new QPushButton(this);
        btn_delete->setText("delete");
        QSignalMapper *sm2 = new QSignalMapper(this);
        connect(sm2, SIGNAL(mapped(int)), this, SLOT(delete_librarian_clicked(int)));
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

    QString authors = ui->line_authors_books->text();
    QString title = ui->line_title_books->text();
    QString keywords = ui->line_keywords_books->text();
    QString publisher = ui->line_publisher_books->text();
    int year = (ui->line_year_books->text() == "" ? 0 : ui->line_year_books->text().toInt());
    bool bestseller = ui->checkbox_bestseller_books->isChecked();
    bool available = ui->checkbox_available_books->isChecked();
    bool or_and = ui->checkbox_criteria_books->isChecked();

    QVector<Book> found = me.search_books(authors, title, keywords, publisher, year, bestseller, available, or_and);
    for (int i = 0; i < found.size(); i++){
        ui->table_search_books->insertRow(i);
        QPushButton *btn_modify = new QPushButton(this);
        btn_modify->setText("modify");
        QSignalMapper *sm = new QSignalMapper(this);
        connect(sm, SIGNAL(mapped(int)), this, SLOT(modify_book_clicked(int)));
        connect(btn_modify, SIGNAL(clicked()), sm, SLOT(map()));
        sm->setMapping(btn_modify, found[i].id);

        QPushButton *btn_delete = new QPushButton(this);
        btn_delete->setText("delete");
        QSignalMapper *sm2 = new QSignalMapper(this);
        connect(sm2, SIGNAL(mapped(int)), this, SLOT(delete_book_clicked(int)));
        connect(btn_delete, SIGNAL(clicked()), sm2, SLOT(map()));
        sm2->setMapping(btn_delete, found[i].id);

        ui->table_search_books->setItem(i, 0, new QTableWidgetItem(found[i].title));
        ui->table_search_books->setItem(i, 1, new QTableWidgetItem(found[i].authors));
        ui->table_search_books->setItem(i, 2, new QTableWidgetItem(found[i].publisher));
        ui->table_search_books->setItem(i, 3, new QTableWidgetItem(found[i].keywords));
        ui->table_search_books->setItem(i, 4, new QTableWidgetItem(QString::number(found[i].year)));
        ui->table_search_books->setItem(i, 5, new QTableWidgetItem(QString::number(found[i].price)));
        ui->table_search_books->setItem(i, 6, new QTableWidgetItem(QString::number(found[i].room)));
        ui->table_search_books->setItem(i, 7, new QTableWidgetItem(QString::number(found[i].level)));
        ui->table_search_books->setItem(i, 8, new QTableWidgetItem(QString::number(found[i].copies)));
        ui->table_search_books->setItem(i, 9, new QTableWidgetItem(found[i].bestseller ? "yes" : "no"));
        ui->table_search_books->setItem(i, 10, new QTableWidgetItem(found[i].reference ? "yes" : "no"));
        ui->table_search_books->setCellWidget(i, 11, btn_modify);
        ui->table_search_books->setCellWidget(i, 12, btn_delete);
    }
    ui->table_search_books->resizeColumnsToContents();
}
void Librarian::on_button_search_articles_clicked(){
    ui->table_search_articles->setRowCount(0);

    QString authors = ui->line_author_articles->text();
    QString title = ui->line_title_articles->text();
    QString journal = ui->line_journal_articles->text();
    QString keywords = ui->line_keywords_articles->text();
    QString publisher = ui->line_publisher_articles->text();
    QString editors = ui->line_editors_articles->text();
    int year = (ui->line_year_articles->text() == "" ? 0 : ui->line_year_articles->text().toInt());
    int month = (ui->combobox_month_articles->currentText() == "" ? 0 : ui->combobox_month_articles->currentText().toInt());
    bool available = ui->checkbox_available_articles->isChecked();
    bool or_and = ui->checkbox_criteria_articles->isChecked();

    QVector<Article> found = me.search_articles(authors, title, keywords, journal, publisher, editors, year, month, available, or_and);
    for (int i = 0; i < found.size(); i++){
        ui->table_search_articles->insertRow(i);
        QPushButton *btn_modify = new QPushButton(this);
        btn_modify->setText("modify");
        QSignalMapper *sm = new QSignalMapper(this);
        connect(sm, SIGNAL(mapped(int)), this, SLOT(modify_article_clicked(int)));
        connect(btn_modify, SIGNAL(clicked()), sm, SLOT(map()));
        sm->setMapping(btn_modify, found[i].id);

        QPushButton *btn_delete = new QPushButton(this);
        btn_delete->setText("delete");
        QSignalMapper *sm2 = new QSignalMapper(this);
        connect(sm2, SIGNAL(mapped(int)), this, SLOT(delete_article_clicked(int)));
        connect(btn_delete, SIGNAL(clicked()), sm2, SLOT(map()));
        sm2->setMapping(btn_delete, found[i].id);

        ui->table_search_articles->setItem(i, 0, new QTableWidgetItem(found[i].title));
        ui->table_search_articles->setItem(i, 1, new QTableWidgetItem(found[i].authors));
        ui->table_search_articles->setItem(i, 2, new QTableWidgetItem(found[i].publisher));
        ui->table_search_articles->setItem(i, 3, new QTableWidgetItem(found[i].journal_title));
        ui->table_search_articles->setItem(i, 4, new QTableWidgetItem(found[i].editors));
        ui->table_search_articles->setItem(i, 5, new QTableWidgetItem(QString::number(found[i].year)));
        ui->table_search_articles->setItem(i, 6, new QTableWidgetItem(QDate::longMonthName(found[i].month, QDate::DateFormat)));
        ui->table_search_articles->setItem(i, 7, new QTableWidgetItem(QString::number(found[i].price)));
        ui->table_search_articles->setItem(i, 8, new QTableWidgetItem(QString::number(found[i].room)));
        ui->table_search_articles->setItem(i, 9, new QTableWidgetItem(QString::number(found[i].level)));
        ui->table_search_articles->setItem(i, 10, new QTableWidgetItem(QString::number(found[i].copies)));
        ui->table_search_articles->setItem(i, 11, new QTableWidgetItem(found[i].reference ? "yes" : "no"));
        ui->table_search_articles->setCellWidget(i, 12, btn_modify);
        ui->table_search_articles->setCellWidget(i, 13, btn_delete);
    }
    ui->table_search_articles->resizeColumnsToContents();
}
void Librarian::on_button_search_vas_clicked(){
    ui->table_search_va->setRowCount(0);

    QString authors = ui->line_authors_va->text();
    QString title = ui->line_title_va->text();
    QString keywords = ui->line_keywords_va->text();
    bool available = ui->checkbox_available_va->isChecked();
    bool or_and = ui->checkbox_criteria_va->isChecked();

    QVector<VA> found = me.search_vas(authors, title, keywords, available, or_and);
    for (int i = 0; i < found.size(); i++){
        ui->table_search_va->insertRow(i);
        QPushButton *btn_modify = new QPushButton(this);
        btn_modify->setText("modify");
        QSignalMapper *sm = new QSignalMapper(this);
        connect(sm, SIGNAL(mapped(int)), this, SLOT(modify_va_clicked(int)));
        connect(btn_modify, SIGNAL(clicked()), sm, SLOT(map()));
        sm->setMapping(btn_modify, found[i].id);

        QPushButton *btn_delete = new QPushButton(this);
        btn_delete->setText("delete");
        QSignalMapper *sm2 = new QSignalMapper(this);
        connect(sm2, SIGNAL(mapped(int)), this, SLOT(delete_va_clicked(int)));
        connect(btn_delete, SIGNAL(clicked()), sm2, SLOT(map()));
        sm2->setMapping(btn_delete, found[i].id);

        ui->table_search_va->setItem(i, 0, new QTableWidgetItem(found[i].title));
        ui->table_search_va->setItem(i, 1, new QTableWidgetItem(found[i].authors));
        ui->table_search_va->setItem(i, 2, new QTableWidgetItem(QString::number(found[i].price)));
        ui->table_search_va->setItem(i, 3, new QTableWidgetItem(QString::number(found[i].room)));
        ui->table_search_va->setItem(i, 4, new QTableWidgetItem(QString::number(found[i].level)));
        ui->table_search_va->setItem(i, 5, new QTableWidgetItem(QString::number(found[i].copies)));
        ui->table_search_va->setItem(i, 6, new QTableWidgetItem(found[i].reference ? "yes" : "no"));
        ui->table_search_va->setCellWidget(i, 7, btn_modify);
        ui->table_search_va->setCellWidget(i, 8, btn_delete);
    }
    ui->table_search_va->resizeColumnsToContents();
}

void Librarian::modify_patron_clicked(int user_id){
    if (widget != NULL && !widget->isHidden()) return;
    PatronUser patron = me.get_patron(user_id);
    widget = new QWidget();
    QLabel *id = new QLabel("card id:");
    QLabel *name = new QLabel("name:");
    QLabel *address = new QLabel("address:");
    QLabel *phone = new QLabel("phone:");
    QLabel *faculty = new QLabel("faculty:");
    QLabel *login = new QLabel("login:");
    QLabel *password = new QLabel("password:");
    clearObjects();
    line1->setText(QString::number(patron.id));
    line2->setText(patron.name);
    line3->setText(patron.address);
    line4->setText(patron.phone);
    check->setChecked(patron.faculty);
    line5->setText(patron.login);
    line6->setText(patron.password);
    line1->setEnabled(false);
    w_layout->addRow(id, line1);
    w_layout->addRow(name, line2);
    w_layout->addRow(address, line3);
    w_layout->addRow(phone, line4);
    w_layout->addRow(faculty, check);
    w_layout->addRow(login, line5);
    w_layout->addRow(password, line6);
    w_layout->addRow(cancel, ok);
    w_layout->setLabelAlignment(Qt::AlignRight);
    connect(ok, SIGNAL (clicked()),this, SLOT (modifyPatron()));
    widget->setLayout(w_layout);
    widget->show();
}
void Librarian::modify_librarian_clicked(int user_id){
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
    connect(ok, SIGNAL (clicked()),this, SLOT (modifyLibrarian()));
    widget->setLayout(w_layout);
    widget->show();
}
void Librarian::modify_book_clicked(int document_id){
    if (widget != NULL && !widget->isHidden()) return;
    widget = new QWidget();
    QLabel *id = new QLabel("id:");
    QLabel *title = new QLabel("title:");
    QLabel *authors = new QLabel("authors:");
    QLabel *publisher = new QLabel("publisher:");
    QLabel *keywords = new QLabel("keywords:");
    QLabel *year = new QLabel("year:");
    QLabel *price = new QLabel("price:");
    QLabel *room = new QLabel("room:");
    QLabel *level = new QLabel("level:");
    QLabel *copies = new QLabel("copies:");
    QLabel *bestseller = new QLabel("bestseller:");
    QLabel *reference = new QLabel("reference:");
    clearObjects();
    line1->setEnabled(0);
    Book book = me.get_book(document_id);
    line1->setText(QString::number(book.id));
    line2->setText(book.title);
    line3->setText(book.authors);
    line4->setText(book.publisher);
    line5->setText(book.keywords);
    line6->setText(QString::number(book.year));
    line7->setText(QString::number(book.price));
    line8->setText(QString::number(book.room));
    line9->setText(QString::number(book.level));
    line10->setText(QString::number(book.copies));
    check->setChecked(book.bestseller);
    check2->setChecked(book.reference);

    w_layout = new QFormLayout();
    w_layout->addRow(id, line1);
    w_layout->addRow(title, line2);
    w_layout->addRow(authors, line3);
    w_layout->addRow(publisher, line4);
    w_layout->addRow(keywords, line5);
    w_layout->addRow(year, line6);
    w_layout->addRow(price, line7);
    w_layout->addRow(room, line8);
    w_layout->addRow(level, line9);
    w_layout->addRow(copies, line10);
    w_layout->addRow(bestseller, check);
    w_layout->addRow(reference, check2);
    w_layout->addRow(cancel, ok);
    connect(ok, SIGNAL (clicked()),this, SLOT (modifyBook()));
    widget->setLayout(w_layout);
    widget->show();
}
void Librarian::modify_article_clicked(int document_id){
    if (widget != NULL && !widget->isHidden()) return;
    widget = new QWidget();
    QLabel *id = new QLabel("id:");
    QLabel *title = new QLabel("title:");
    QLabel *authors = new QLabel("authors:");
    QLabel *publisher = new QLabel("publisher:");
    QLabel *journal = new QLabel("journal:");
    QLabel *editors = new QLabel("editors:");
    QLabel *keywords = new QLabel("keywords:");
    QLabel *year = new QLabel("year:");
    QLabel *month = new QLabel("month:");
    QLabel *price = new QLabel("price:");
    QLabel *room = new QLabel("room:");
    QLabel *level = new QLabel("level:");
    QLabel *copies = new QLabel("copies:");
    QLabel *reference = new QLabel("reference:");
    clearObjects();
    line1->setEnabled(0);
    Article article = me.get_article(document_id);
    line1->setText(QString::number(article.id));
    line2->setText(article.title);
    line3->setText(article.authors);
    line4->setText(article.publisher);
    line5->setText(article.journal_title);
    line6->setText(article.editors);
    line7->setText(article.keywords);
    line8->setText(QString::number(article.year));
    line9->setText(QString::number(article.price));
    line10->setText(QString::number(article.room));
    line11->setText(QString::number(article.level));
    line12->setText(QString::number(article.copies));
    check->setChecked(article.reference);
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
    combo->setCurrentIndex(article.month - 1);
    w_layout->addRow(id, line1);
    w_layout->addRow(title, line2);
    w_layout->addRow(authors, line3);
    w_layout->addRow(publisher, line4);
    w_layout->addRow(journal, line5);
    w_layout->addRow(editors, line6);
    w_layout->addRow(keywords, line7);
    w_layout->addRow(year, line8);
    w_layout->addRow(month, combo);
    w_layout->addRow(price, line9);
    w_layout->addRow(room, line10);
    w_layout->addRow(level, line11);
    w_layout->addRow(copies, line12);
    w_layout->addRow(reference, check);
    w_layout->addRow(cancel, ok);
    connect(ok, SIGNAL (clicked()),this, SLOT (modifyArticle()));
    widget->setLayout(w_layout);
    widget->show();
}
void Librarian::modify_va_clicked(int document_id){
    if (widget != NULL && !widget->isHidden()) return;
    widget = new QWidget();
    QLabel *id = new QLabel("id:");
    QLabel *title = new QLabel("title:");
    QLabel *authors = new QLabel("authors:");
    QLabel *keywords = new QLabel("keywords:");
    QLabel *price = new QLabel("price:");
    QLabel *room = new QLabel("room:");
    QLabel *level = new QLabel("level:");
    QLabel *copies = new QLabel("copies:");
    QLabel *reference = new QLabel("reference:");
    clearObjects();
    line1->setEnabled(0);
    VA va = me.get_va(document_id);
    line1->setText(QString::number(va.id));
    line2->setText(va.title);
    line3->setText(va.authors);
    line4->setText(va.keywords);
    line5->setText(QString::number(va.price));
    line6->setText(QString::number(va.room));
    line7->setText(QString::number(va.level));
    line8->setText(QString::number(va.copies));
    check->setChecked(va.reference);
    w_layout->addRow(id, line1);
    w_layout->addRow(title, line2);
    w_layout->addRow(authors, line3);
    w_layout->addRow(keywords, line4);
    w_layout->addRow(price, line5);
    w_layout->addRow(room, line6);
    w_layout->addRow(level, line7);
    w_layout->addRow(copies, line8);
    w_layout->addRow(reference, check);
    w_layout->addRow(cancel, ok);
    connect(ok, SIGNAL (clicked()),this, SLOT (modifyVA()));
    widget->setLayout(w_layout);
    widget->show();
}

void Librarian::delete_patron_clicked(int id){
    PatronUser patron = me.get_patron(id);
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Delete", "Are you sure you want to delete this patron?\nname: " + patron.name + "\nlogin: " + patron.login, QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No) return;
    if (!me.delete_patron(id)){
        ui->status->setText("This patron has actual check outs");
        return;
    }
    ui->status->setText("Patron deleted successfully");
    on_button_search_patrons_clicked();
}
void Librarian::delete_librarian_clicked(int id){
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Delete", "Are you sure you want to delete this librarian?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No) return;
}
void Librarian::delete_book_clicked(int id){
    Book book = me.get_book(id);
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Delete", "Are you sure you want to delete this book?\ntitle: " + book.title, QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No) return;
    me.delete_book(id);
    on_button_search_books_clicked();
}
void Librarian::delete_article_clicked(int id){
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Delete", "Are you sure you want to delete this article?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No) return;
    me.delete_article(id);
    on_button_search_articles_clicked();
}
void Librarian::delete_va_clicked(int id){
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Delete", "Are you sure you want to delete this va?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No) return;
    me.delete_va(id);
    on_button_search_vas_clicked();
}

void Librarian::return_book(int check_out_id){
    std::pair<int, int> ret = me.return_book(check_out_id);
    int fine = ret.first, user_id = ret.second;
    if (fine == -1)
        ui->status->setText("Error returning document");
    else if (fine > 0)
        QMessageBox::information(0, "Fine", "Fine size: " + QString::number(fine));
    if (user_id == -1)
        ui->status->setText("Document returned successfully");
    else
        ui->status->setText("Document returned successfully; Patron " + QString::number(user_id) + " wants this document");
    on_button_show_checked_out_books_clicked();
}

void Librarian::return_article(int check_out_id){
    std::pair<int, int> ret = me.return_article(check_out_id);
    int fine = ret.first, user_id = ret.second;
    if (fine == -1)
        ui->status->setText("Error returning document");
    else if (fine > 0)
        QMessageBox::information(0, "Fine", "Fine size: " + QString::number(fine));
    if (user_id == -1)
        ui->status->setText("Document returned successfully");
    else
        ui->status->setText("Document returned successfully; Patron " + QString::number(user_id) + " wants this document");
    on_button_show_checked_out_articles_clicked();
}

void Librarian::return_va(int check_out_id){
    std::pair<int, int> ret = me.return_va(check_out_id);
    int fine = ret.first, user_id = ret.second;
    if (fine == -1)
        ui->status->setText("Error returning document");
    else if (fine > 0)
        QMessageBox::information(0, "Fine", "Fine size: " + QString::number(fine));
    if (user_id == -1)
        ui->status->setText("Document returned successfully");
    else
        ui->status->setText("Document returned successfully; Patron " + QString::number(user_id) + " wants this document");
    on_button_show_checked_out_vas_clicked();
}

void Librarian::on_button_show_checked_out_books_clicked(){
    ui->table_checked_out_books->setRowCount(0);
    QString authors = ui->line_authors_books_checked->text();
    QString title = ui->line_title_books_checked->text();
    QString keywords = ui->line_keywords_books_checked->text();
    QString publisher = ui->line_publisher_books_checked->text();
    int year = (ui->line_year_books_checked->text() == "" ? 0 : ui->line_year_books_checked->text().toInt());
    bool bestseller = ui->checkbox_bestseller_books_checked->isChecked();
    bool overdue = ui->checkbox_overdue_books_checked->isChecked();
    bool or_and = ui->checkbox_criteria_books_checked->isChecked();
    int user_id = ui->line_user_id_books_checked->text().toInt();

    QVector<std::pair<Check_out, Book> > found = me.search_books_checked_out(user_id, authors, title, keywords, publisher, year, bestseller, overdue, or_and);
    for (int i = 0; i < found.size(); i++){
        ui->table_checked_out_books->insertRow(i);

        QPushButton *btn_return = new QPushButton(this);
        btn_return->setText("return");
        QSignalMapper *sm2 = new QSignalMapper(this);
        connect(sm2, SIGNAL(mapped(int)), this, SLOT(return_book(int)));
        connect(btn_return, SIGNAL(clicked()), sm2, SLOT(map()));
        sm2->setMapping(btn_return, found[i].first.check_out_id);

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
        ui->table_checked_out_books->setItem(i, 8, new QTableWidgetItem(found[i].second.bestseller ? "yes" : "no"));\
        ui->table_checked_out_books->setItem(i, 9, new QTableWidgetItem(date_start));
        ui->table_checked_out_books->setItem(i, 10, new QTableWidgetItem(date_end));
        ui->table_checked_out_books->setItem(i, 11, new QTableWidgetItem(QString::number(found[i].first.fine)));
        ui->table_checked_out_books->setCellWidget(i, 12, btn_return);
    }
    ui->table_checked_out_books->resizeColumnsToContents();
}
void Librarian::on_button_show_checked_out_articles_clicked(){
    ui->table_checked_out_articles->setRowCount(0);

    QString authors = ui->line_authors_checked_out_articles->text();
    QString title = ui->line_title_checked_out_articles->text();
    QString journal = ui->line_journal_checked_out_articles->text();
    QString keywords = ui->line_keywords_checked_out_articles->text();
    QString publisher = ui->line_publisher_checked_out_articles->text();
    QString editors = ui->line_editors_checked_out_articles->text();
    int user_id = ui->line_user_id_checked_out_articles->text().toInt();
    int year = (ui->line_year_checked_out_articles->text() == "" ? 0 : ui->line_year_checked_out_articles->text().toInt());
    int month = (ui->combobox_checked_out_articles->currentText() == "" ? 0 : ui->combobox_checked_out_articles->currentText().toInt());
    bool overdue = ui->checkbox_overdue_checked_out_articles->isChecked();
    bool or_and = ui->checkbox_criteria_checked_out_articles->isChecked();

    QVector<std::pair<Check_out, Article> > found = me.search_articles_checked_out(user_id, authors, title, keywords, journal, publisher, editors, year, month, overdue, or_and);
    for (int i = 0; i < found.size(); i++){
        ui->table_checked_out_articles->insertRow(i);

        QPushButton *btn_return = new QPushButton(this);
        btn_return->setText("return");
        QSignalMapper *sm2 = new QSignalMapper(this);
        connect(sm2, SIGNAL(mapped(int)), this, SLOT(return_article(int)));
        connect(btn_return, SIGNAL(clicked()), sm2, SLOT(map()));
        sm2->setMapping(btn_return, found[i].first.check_out_id);

        QString date_start = QString::number(found[i].first.day_start)+"."+QString::number(found[i].first.month_start)+"."+QString::number(found[i].first.year_start);
        QString date_end = QString::number(found[i].first.day_end)+"."+QString::number(found[i].first.month_end)+"."+QString::number(found[i].first.year_end);
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
        ui->table_checked_out_articles->setCellWidget(i, 14, btn_return);
    }
    ui->table_checked_out_articles->resizeColumnsToContents();
}
void Librarian::on_button_show_checked_out_vas_clicked(){
    ui->table_checked_out_vas->setRowCount(0);

    QString authors = ui->line_authors_checked_out_vas->text();
    QString title = ui->line_title_checked_out_vas->text();
    QString keywords = ui->line_keywords_checked_out_vas->text();
    int user_id = ui->line_user_id_checked_out_vas->text().toInt();
    bool overdue = ui->checkbox_overdue_checked_out_vas->isChecked();
    bool or_and = ui->checkbox_criteria_checked_out_vas->isChecked();

    QVector<std::pair<Check_out, VA> > found = me.search_vas_checked_out(user_id, authors, title, keywords, overdue, or_and);
    for (int i = 0; i < found.size(); i++){

        QPushButton *btn_return = new QPushButton(this);
        btn_return->setText("return");
        QSignalMapper *sm2 = new QSignalMapper(this);
        connect(sm2, SIGNAL(mapped(int)), this, SLOT(return_va(int)));
        connect(btn_return, SIGNAL(clicked()), sm2, SLOT(map()));
        sm2->setMapping(btn_return, found[i].first.check_out_id);

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
        ui->table_checked_out_vas->setCellWidget(i, 9, btn_return);
    }
    ui->table_checked_out_vas->resizeColumnsToContents();
}

void Librarian::modifyPatron(){
    int id = line1->text().toInt();
    QString name = line2->text();
    QString address = line3->text();
    QString phone = line4->text();
    QString login = line5->text();
    QString password = line6->text();
    bool faculty = check->isChecked();

    me.modify_patron(id, name, address, phone, faculty, login, password);
    closeWidget();
    on_button_search_patrons_clicked();
}
void Librarian::modifyLibrarian(){
    QString name = line2->text();
    QString address = line3->text();
    QString phone = line4->text();
    closeWidget();
}
void Librarian::modifyBook(){
    int id = line1->text().toInt();
    QString title = line2->text();
    QString authors = line3->text();
    QString publisher = line4->text();
    QString keywords = line5->text();
    int year = line6->text().toInt();
    int price = line7->text().toInt();
    int room = line8->text().toInt();
    int level = line9->text().toInt();
    int copies = line10->text().toInt();
    bool bestseller = check->isChecked();
    bool reference = check2->isChecked();
    me.modify_book(id, title, authors, publisher, keywords, year, price, room, level, copies, bestseller, reference);
    on_button_search_books_clicked();
    closeWidget();
}
void Librarian::modifyArticle(){
    int id = line1->text().toInt();
    QString title = line2->text();
    QString authors = line3->text();
    QString publisher = line4->text();
    QString journal = line5->text();
    QString editors = line6->text();
    QString keywords = line7->text();
    int year = line8->text().toInt();
    int month = combo->currentIndex() + 1;
    int price = line9->text().toInt();
    int room = line10->text().toInt();
    int level = line11->text().toInt();
    int copies = line12->text().toInt();
    bool reference = check->isChecked();
    me.modify_article(id, title, authors, journal, keywords, publisher, editors, year, month, price, room, level, copies, reference);
    on_button_search_articles_clicked();
    closeWidget();
}
void Librarian::modifyVA(){
    int id = line1->text().toInt();
    QString title = line2->text();
    QString authors = line3->text();
    QString keywords = line4->text();
    int price = line5->text().toInt();
    int room = line6->text().toInt();
    int level = line7->text().toInt();
    int copies = line8->text().toInt();
    bool reference = check->isChecked();
    me.modify_va(id, title, authors, keywords, price, room, level, copies, reference);
    on_button_search_vas_clicked();
    closeWidget();
}

void Librarian::createPatron(){
    QString name = line1->text();
    QString address = line2->text();
    QString phone = line3->text();
    QString login = line4->text();
    QString password = line5->text();
    bool faculty = check->isChecked();
    closeWidget();
    me.add_patron(name, address, phone, faculty, login, password);
    on_button_search_patrons_clicked();
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
    QString keywords = line4->text();
    int year = line5->text().toInt();
    int price = line6->text().toInt();
    int room = line7->text().toInt();
    int level = line8->text().toInt();
    int copies = line9->text().toInt();
    bool bestseller = check->isChecked();
    bool reference = check2->isChecked();
    me.add_book(title, authors, publisher, keywords, year, price, room, level, copies, bestseller, reference);
    on_button_search_books_clicked();
    closeWidget();
}
void Librarian::createArticle(){
    QString title = line1->text();
    QString authors = line2->text();
    QString journal = line3->text();
    QString publisher = line4->text();
    QString keywords = line5->text();
    QString editors = line6->text();
    int year = line7->text().toInt();
    int month = combo->currentIndex() + 1;
    int price = line8->text().toInt();
    int room = line9->text().toInt();
    int level = line10->text().toInt();
    int copies = line11->text().toInt();
    bool reference = check->isChecked();
    me.add_article(title, authors, journal, publisher, keywords, editors, year, month, price, room, level, copies, reference);
    on_button_search_articles_clicked();
    closeWidget();
}
void Librarian::createVA(){
    QString title = line1->text();
    QString authors = line2->text();
    QString keywords = line3->text();
    int price = line4->text().toInt();
    int room = line5->text().toInt();
    int level = line6->text().toInt();
    int copies = line7->text().toInt();
    bool reference = check->isChecked();
    me.add_va(title, authors, keywords, price, room, level, copies, reference);
    on_button_search_vas_clicked();
    closeWidget();
}

void Librarian::on_button_new_patron_clicked(){
    if (widget != NULL && !widget->isHidden()) return;
    widget = new QWidget();
    QLabel *name = new QLabel("name:");
    QLabel *address = new QLabel("address:");
    QLabel *phone = new QLabel("phone:");
    QLabel *faculty = new QLabel("faculty:");
    QLabel *login = new QLabel("login:");
    QLabel *password = new QLabel("password:");
    clearObjects();
    w_layout->addRow(name, line1);
    w_layout->addRow(address, line2);
    w_layout->addRow(phone, line3);
    w_layout->addRow(faculty, check);
    w_layout->addRow(login, line4);
    w_layout->addRow(password, line5);
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
    QLabel *keywords = new QLabel("keywords:");
    QLabel *year = new QLabel("year:");
    QLabel *price = new QLabel("price:");
    QLabel *room = new QLabel("room:");
    QLabel *level = new QLabel("level:");
    QLabel *copies = new QLabel("copies:");
    QLabel *bestseller = new QLabel("bestseller:");
    QLabel *reference = new QLabel("reference:");
    clearObjects();
    w_layout = new QFormLayout();
    w_layout->setAlignment(Qt::AlignRight);
    w_layout->addRow(title, line1);
    w_layout->addRow(authors, line2);
    w_layout->addRow(publisher, line3);
    w_layout->addRow(keywords, line4);
    w_layout->addRow(year, line5);
    w_layout->addRow(price, line6);
    w_layout->addRow(room, line7);
    w_layout->addRow(level, line8);
    w_layout->addRow(copies, line9);
    w_layout->addRow(bestseller, check);
    w_layout->addRow(reference, check2);
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
    QLabel *journal = new QLabel("journal:");
    QLabel *publisher = new QLabel("publisher:");
    QLabel *keywords = new QLabel("keywords:");
    QLabel *editors = new QLabel("editors:");
    QLabel *year = new QLabel("year:");
    QLabel *month = new QLabel("month:");
    QLabel *price = new QLabel("price:");
    QLabel *room = new QLabel("room:");
    QLabel *level = new QLabel("level:");
    QLabel *copies = new QLabel("copies:");
    QLabel *reference = new QLabel("reference:");
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
    w_layout->addRow(journal, line3);
    w_layout->addRow(publisher, line4);
    w_layout->addRow(keywords, line5);
    w_layout->addRow(editors, line6);
    w_layout->addRow(year, line7);
    w_layout->addRow(month, combo);
    w_layout->addRow(price, line8);
    w_layout->addRow(room, line9);
    w_layout->addRow(level, line10);
    w_layout->addRow(copies, line11);
    w_layout->addRow(reference, check);
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
    QLabel *keywords = new QLabel("keywords:");
    QLabel *price = new QLabel("price:");
    QLabel *room = new QLabel("room:");
    QLabel *level = new QLabel("level:");
    QLabel *copies = new QLabel("copies:");
    QLabel *reference = new QLabel("reference:");
    clearObjects();
    w_layout->addRow(title, line1);
    w_layout->addRow(authors, line2);
    w_layout->addRow(keywords, line3);
    w_layout->addRow(price, line4);
    w_layout->addRow(room, line5);
    w_layout->addRow(level, line6);
    w_layout->addRow(copies, line7);
    w_layout->addRow(reference, check);
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

void Librarian::showName(){
    ui->status->setText("Logged in as librarian: " + me.name);
}
void Librarian::modifySettings(){
    int days = line1->text().toInt();
    me.set_settings(days);
    closeWidget();
}
void Librarian::on_button_settings_clicked(){
    if (widget != NULL && !widget->isHidden()) return;
    widget = new QWidget();
    QLabel *days = new QLabel("days to add for renew:");
    clearObjects();
    line1->setText(QString::number(me.get_days_add_renew()));
    w_layout->addRow(days, line1);
    w_layout->addRow(cancel, ok);
    connect(ok, SIGNAL (clicked()),this, SLOT (modifySettings()));
    widget->setLayout(w_layout);
    widget->show();
}
//need special function as slot to connect with button
void Librarian::closeWidget(){
    widget->close();
}
