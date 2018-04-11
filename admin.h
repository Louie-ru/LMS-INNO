#ifndef ADMIN_H
#define ADMIN_H

#include <QWidget>
#include <skeleton.cpp>

namespace Ui {
class Admin;
}

class Admin : public QWidget
{
    Q_OBJECT

public:
    explicit Admin(QWidget *parent = 0);
    ~Admin();
    AdminUser me;
    void showName();
private slots:
    void on_button_search_librarians_clicked();
    void on_button_new_librarian_clicked();
    void modify_librarian_clicked(int);
    void delete_librarian_clicked(int);
    void modifyLibrarian();
    void createLibrarian();
    void closeWidget();
private:
    Ui::Admin *ui;
    void clearObjects();
};

#endif // ADMIN_H
