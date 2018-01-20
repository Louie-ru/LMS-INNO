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

private:
    Ui::Librarian *ui;
};

#endif // LIBRARIAN_H
