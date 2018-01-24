#ifndef PATRON_H
#define PATRON_H
#include <QWidget>
#include <QDebug>

namespace Ui {
class Patron;
}
class Patron : public QWidget
{
    Q_OBJECT
public:
    explicit Patron(QWidget *parent = 0);
    ~Patron();
private:
    Ui::Patron *ui;
private slots:
    void on_button_search_books_clicked();
    void on_button_search_articles_clicked();
    void on_button_search_va_clicked();
    void show_my_books();
    void show_my_articles();
    void show_my_vas();
    void on_pushButton_clicked();
};

#endif // PATRON_H
