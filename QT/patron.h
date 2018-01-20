#ifndef PATRON_H
#define PATRON_H

#include <QWidget>

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
};

#endif // PATRON_H
