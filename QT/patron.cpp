#include "patron.h"
#include "ui_patron.h"

Patron::Patron(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Patron)
{
    ui->setupUi(this);
}

Patron::~Patron()
{
    delete ui;
}
