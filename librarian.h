#ifndef LIBRARIAN_H
#define LIBRARIAN_H
#include <QWidget>
#include "skeleton.cpp"
#include <librarian.h>

namespace Ui {
class Librarian;
}
class Librarian : public QWidget
{
    Q_OBJECT

public:
    explicit Librarian(QWidget *parent = 0);
    ~Librarian();
    LibrarianUser me;
    void showName();
private slots:
    void on_button_search_patrons_clicked();
    void on_button_search_librarians_clicked();
    void on_button_search_books_clicked();
    void on_button_search_articles_clicked();
    void on_button_search_vas_clicked();

    void on_button_show_checked_out_books_clicked();
    void on_button_show_checked_out_articles_clicked();
    void on_button_show_checked_out_vas_clicked();

    void on_button_new_patron_clicked();
    void on_button_new_librarian_clicked();
    void on_button_new_book_clicked();
    void on_button_new_article_clicked();
    void on_button_new_va_clicked();

    void modify_patron_clicked(int);
    void modify_librarian_clicked(int);
    void modify_book_clicked(int);
    void modify_article_clicked(int);
    void modify_va_clicked(int);

    void delete_patron_clicked(int);
    void delete_librarian_clicked(int);
    void delete_book_clicked(int);
    void delete_article_clicked(int);
    void delete_va_clicked(int);

    void modifyBook();
    void modifyArticle();
    void modifyVA();
    void modifyPatron();
    void modifyLibrarian();
    void modifySettings();

    void createBook();
    void createArticle();
    void createVA();
    void createPatron();
    void createLibrarian();

    void return_book(int);
    void return_article(int);
    void return_va(int);

    void closeWidget();
    void on_button_logout_clicked();
    void on_button_settings_clicked();

    void request_book_clicked(int);
    void request_article_clicked(int);
    void request_va_clicked(int);
private:
    Ui::Librarian *ui;
    void clearObjects();
};

#endif // LIBRARIAN_H
