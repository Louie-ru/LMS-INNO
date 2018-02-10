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
    void on_button_logout_clicked();

    void on_modify_patron_clicked(int id);
    void on_modify_librarian_clicked(int id);
    void on_modify_book_clicked(int id);
    void on_modify_article_clicked(int id);
    void on_modify_va_clicked(int id);

    void on_delete_patron_clicked(int id);
    void on_delete_librarian_clicked(int id);
    void on_delete_book_clicked(int id);
    void on_delete_article_clicked(int id);
    void on_delete_va_clicked(int id);

    void modifyBook();
    void modifyArticle();
    void modifyVA();
    void modifyPatron();
    void modifyLibrarian();

    void createBook();
    void createArticle();
    void createVA();
    void createPatron();
    void createLibrarian();

    void closeWidget();

private:
    Ui::Librarian *ui;
    void clearObjects();
};

#endif // LIBRARIAN_H