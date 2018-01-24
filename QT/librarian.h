#ifndef LIBRARIAN_H
#define LIBRARIAN_H
#include <QWidget>

namespace Ui {
class Librarian;
}
class Librarian : public QWidget
{
    Q_OBJECT
public:
    explicit Librarian(QWidget *parent = 0);
    ~Librarian();
private slots:
    void on_button_search_patrons_clicked();
    void on_button_search_librarians_clicked();
    void on_button_search_books_clicked();
    void on_button_search_articles_clicked();
    void on_button_search_vas_clicked();
    void on_button_show_checked_out_books_clicked();
    void on_button_show_checked_out_articles_clicked();
    void on_button_show_checked_out_vas_clicked();
    void on_pushButton_clicked();

private:
    Ui::Librarian *ui;
};

#endif // LIBRARIAN_H
