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
private:
    Ui::Patron *ui;
private slots:
    void on_button_search_books_clicked();
    void on_button_search_articles_clicked();
    void on_button_search_va_clicked();
    void on_button_logout_clicked();

    void check_out_book(int id);
    void renew_book(int id);
    void return_book(int id);

    void on_table_my_vas_clicked(const QModelIndex &index);
    void on_table_my_articles_clicked(const QModelIndex &index);
    void on_tabWidget_tabBarClicked(int index);
};

#endif // PATRON_H
