#ifndef ADMIN_H
#define ADMIN_H

#include <QWidget>
#include "skeleton.cpp"

namespace Ui {
class admin;
}

class admin : public QWidget
{
    Q_OBJECT

public:
    explicit admin(QWidget *parent = 0);
    ~admin();
    AdminUser me;
private slots:
    void on_button_search_librarians_clicked();
    void on_button_new_librarian_clicked();
    void modify_librarian_clicked(int);
    void delete_librarian_clicked(int);
    void modifyLibrarian();
    void createLibrarian();
    void closeWidget();
private:
    Ui::admin *ui;
    void clearObjects();
};

#endif // ADMIN_H
