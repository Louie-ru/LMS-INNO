#ifndef PATRON_H
#define PATRON_H
#include <QWidget>
#include <QDebug>
#include "skeleton.cpp"

namespace Ui {
class Patron;
}
class Patron : public QWidget{
    Q_OBJECT

public:

    explicit Patron(QWidget *parent = 0);
    ~Patron();
    PatronUser me;
    void showName();
private:
    Ui::Patron *ui;
private slots:
    void on_button_search_books_clicked();
    void on_button_search_articles_clicked();
    void on_button_search_va_clicked();

    void check_out_book(int id);
    void check_out_article(int id);
    void check_out_va(int id);

    void renew_book(int check_out_id);
    void renew_article(int check_out_id);
    void renew_va(int check_out_id);

    void want_book(int);
    void want_article(int);
    void want_va(int);

    void take_reserved(int document_id, int document_type);

    void on_tabWidget_tabBarClicked(int index);
    void on_button_logout_clicked();
    void on_button_delete_account_clicked();
};

#endif // PATRON_H
