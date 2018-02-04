#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <patron.h>
#include <librarian.h>

namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_button_login_clicked();
private:
    Ui::MainWindow *ui;
    Patron *patron;
    Librarian *librarian;
};

#endif // MAINWINDOW_H
