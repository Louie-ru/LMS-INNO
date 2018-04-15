#include "admin.h"
#include "ui_admin.h"
#include <QMessageBox>
#include <mainwindow.h>

Admin::Admin(QWidget *parent) : QWidget(parent), ui(new Ui::Admin){
    ui->setupUi(this);
}

Admin::~Admin(){
    delete ui;
}

//those are widgets for data input(new/modify documents and users)
QWidget *widget_;
QLineEdit *line1_;
QLineEdit *line2_;
QLineEdit *line3_;
QLineEdit *line4_;
QLineEdit *line5_;
QLineEdit *line6_;
QLineEdit *line7_;

QComboBox *combo_;
QPushButton *ok_;
QPushButton *cancel_;
QFormLayout *w_layout_;

//make new input fields
void Admin::clearObjects(){
    line1_ = new QLineEdit();
    line2_ = new QLineEdit();
    line3_ = new QLineEdit();
    line4_ = new QLineEdit();
    line5_ = new QLineEdit();
    line6_ = new QLineEdit();
    line7_ = new QLineEdit();
    combo_ = new QComboBox();
    ok_ = new QPushButton("Ok");
    cancel_ = new QPushButton("Cancel");
    w_layout_ = new QFormLayout();
    w_layout_->setLabelAlignment(Qt::AlignRight);
    connect(cancel_, SIGNAL (clicked()),this, SLOT (closeWidget()));
    widget_->setWindowFlags(Qt::WindowStaysOnTopHint);
}

void Admin::on_button_search_librarians_clicked(){
    ui->table_librarian->setRowCount(0);

    int user_id = ui->line_librarian_id->text().toInt();
    QString name = ui->line_librarian_name->text();
    QString phone = ui->line_librarian_phone->text();
    QString address = ui->line_librarian_adress->text();
    int privel = ui->combo_privileges->currentIndex();
    bool or_and = ui->checkbox_librarian_criteria->isChecked();

    QVector<LibrarianUser> found = me.search_librarians(user_id, name, address, phone, privel, or_and);
    for (int i = 0; i < found.size(); i++){
        ui->table_librarian->insertRow(i);
        QPushButton *btn_modify = new QPushButton(this);
        btn_modify->setText("modify");
        QSignalMapper *sm = new QSignalMapper(this);
        connect(sm, SIGNAL(mapped(int)), this, SLOT(modify_librarian_clicked(int)));
        connect(btn_modify, SIGNAL(clicked()), sm, SLOT(map()));
        sm->setMapping(btn_modify, found[i].id);

        QPushButton *btn_delete = new QPushButton(this);
        btn_delete->setText("delete");
        QSignalMapper *sm2 = new QSignalMapper(this);
        connect(sm2, SIGNAL(mapped(int)), this, SLOT(delete_librarian_clicked(int)));
        connect(btn_delete, SIGNAL(clicked()), sm2, SLOT(map()));
        sm2->setMapping(btn_delete, found[i].id);

        QString priveleges = "";
        switch(found[i].privileges){
            case 1: priveleges = "Priv1"; break;
            case 2: priveleges = "Priv2"; break;
            case 3: priveleges = "Priv3"; break;
        }

        ui->table_librarian->setItem(i, 0, new QTableWidgetItem(QString::number(found[i].id)));
        ui->table_librarian->setItem(i, 1, new QTableWidgetItem(found[i].name));
        ui->table_librarian->setItem(i, 2, new QTableWidgetItem(found[i].address));
        ui->table_librarian->setItem(i, 3, new QTableWidgetItem(found[i].phone));
        ui->table_librarian->setItem(i, 4, new QTableWidgetItem(found[i].login));
        ui->table_librarian->setItem(i, 5, new QTableWidgetItem(priveleges));
        ui->table_librarian->setCellWidget(i, 6, btn_modify);
        ui->table_librarian->setCellWidget(i, 7, btn_delete);
    }
    set_status("Search complete. " + QString::number(found.size()) + " librarians found");
    ui->table_librarian->resizeColumnsToContents();
}

void Admin::modify_librarian_clicked(int user_id){
    if (widget_ != NULL && !widget_->isHidden()) return;
    LibrarianUser librarian = me.get_librarian(user_id);
    widget_ = new QWidget();
    QLabel *id = new QLabel("card id:");
    QLabel *name = new QLabel("name:");
    QLabel *address = new QLabel("address:");
    QLabel *phone = new QLabel("phone:");
    QLabel *login = new QLabel("login:");
    QLabel *password = new QLabel("password:");
    QLabel *privileges = new QLabel("privileges:");
    clearObjects();
    line1_->setText(QString::number(librarian.id));
    line2_->setText(librarian.name);
    line3_->setText(librarian.address);
    line4_->setText(librarian.phone);
    line5_->setText(librarian.login);
    line6_->setText("");
    combo_->addItem("Priv1");
    combo_->addItem("Priv2");
    combo_->addItem("Priv3");
    combo_->setCurrentIndex(librarian.privileges - 1);
    line1_->setEnabled(false);
    w_layout_->addRow(id, line1_);
    w_layout_->addRow(name, line2_);
    w_layout_->addRow(address, line3_);
    w_layout_->addRow(phone, line4_);
    w_layout_->addRow(login, line5_);
    w_layout_->addRow(password, line6_);
    w_layout_->addRow(privileges, combo_);
    w_layout_->addRow(cancel_, ok_);
    connect(ok_, SIGNAL (clicked()),this, SLOT (modifyLibrarian()));
    widget_->setLayout(w_layout_);
    widget_->show();
}

void Admin::delete_librarian_clicked(int id){
    LibrarianUser librarian = me.get_librarian(id);
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Delete", "Are you sure you want to delete this librarian?\nname: " + librarian.name + "\nlogin: " + librarian.login, QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No) return;
    if (!me.delete_librarian(id)){
        set_status("Error deleting librarian");
        return;
    }
    set_status("Librarian deleted successfully");
    on_button_search_librarians_clicked();
}

void Admin::modifyLibrarian(){
    int user_id = line1_->text().toInt();
    QString name = line2_->text();
    QString address = line3_->text();
    QString phone = line4_->text();
    QString login = line5_->text();
    QString password = line6_->text();
    int privileges = combo_->currentIndex() + 1;

    me.modify_librarian(user_id, name, address, phone, login, password, privileges);
    closeWidget();
    on_button_search_librarians_clicked();
    set_status("Librarian modified successfully");
}

void Admin::createLibrarian(){
    QString name = line1_->text();
    QString address = line2_->text();
    QString phone = line3_->text();
    QString login = line4_->text();
    QString password = line5_->text();
    int privileges = combo_->currentIndex() + 1;
    closeWidget();
    me.add_librarian(name, address, phone, login, password, privileges);
    on_button_search_librarians_clicked();
    set_status("Librarian " + login + " added successfully");
}

void Admin::on_button_new_librarian_clicked(){
    if (widget_ != NULL && !widget_->isHidden()) return;
    widget_ = new QWidget();
    QLabel *name = new QLabel("name:");
    QLabel *address = new QLabel("address:");
    QLabel *phone = new QLabel("phone:");
    QLabel *login = new QLabel("login:");
    QLabel *password = new QLabel("password:");
    QLabel *privileges = new QLabel("privileges:");
    clearObjects();
    combo_->addItem("Priv1");
    combo_->addItem("Priv2");
    combo_->addItem("Priv3");
    w_layout_->addRow(name, line1_);
    w_layout_->addRow(address, line2_);
    w_layout_->addRow(phone, line3_);
    w_layout_->addRow(login, line4_);
    w_layout_->addRow(password, line5_);
    w_layout_->addRow(privileges, combo_);
    w_layout_->addRow(cancel_, ok_);
    connect(ok_, SIGNAL (clicked()),this, SLOT (createLibrarian()));
    widget_->setLayout(w_layout_);
    widget_->show();
}

void Admin::closeWidget(){
    widget_->close();
}

void Admin::set_status(QString status){
    ui->status->setText(status);
    append_log(status);
}


void Admin::showName(){
    set_status("Logged in as admin");
}
void Admin::on_button_logout_clicked(){
    MainWindow *mainwindow = new MainWindow();
    set_status("Admin log out");
    mainwindow->show();
    this->close();
}

void Admin::change_profile(){
    QString name = line1_->text();
    QString address = line2_->text();
    QString phone = line3_->text();
    QString login = line4_->text();
    QString password = line5_->text();
    closeWidget();
    me.set_profile(name, address, phone, login, password);
    set_status("Admin profile changed successfully");
}

void Admin::on_button_my_profile_clicked(){
    if (widget_ != NULL && !widget_->isHidden()) return;
    widget_ = new QWidget();
    QLabel *name = new QLabel("name:");
    QLabel *address = new QLabel("address:");
    QLabel *phone = new QLabel("phone:");
    QLabel *login = new QLabel("login:");
    QLabel *password = new QLabel("password:");
    clearObjects();
    line1_->setText(me.name);
    line2_->setText(me.address);
    line3_->setText(me.phone);
    line4_->setText(me.login);
    line5_->setText("");
    w_layout_->addRow(name, line1_);
    w_layout_->addRow(address, line2_);
    w_layout_->addRow(phone, line3_);
    w_layout_->addRow(login, line4_);
    w_layout_->addRow(password, line5_);
    w_layout_->addRow(cancel_, ok_);
    connect(ok_, SIGNAL (clicked()),this, SLOT (change_profile()));
    widget_->setLayout(w_layout_);
    widget_->show();
}
