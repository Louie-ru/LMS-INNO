#pragma once
#include <QString>
#include <QVector>
#include <QtSql>
#include <unistd.h>


using namespace std;

#define BOOK       1
#define ARTICLE    2
#define AV         3

class Document{
public:
    QString title, keywords;
    int copies, id, price, room, level;
};

class Book : public Document{
public:
    QString publisher, authors;
    int year;
    bool bestseller, reference;
    Book(QString authors_, QString title_, QString keywords_, QString publisher_, int id_, int year_, int copies_, int price_, int room_, int level_, bool bestseller_, bool reference_){
        authors = authors_;
        title = title_;
        keywords = keywords_;
        publisher = publisher_;
        year = year_;
        copies = copies_;
        bestseller = bestseller_;
        id = id_;
        price = price_;
        room = room_;
        level = level_;
        reference = reference_;
    }
    Book(){};
};

class Article : public Document{
public:
    QString journal_title, publisher, editors, authors;
    int year, month;
    Article(QString authors_, QString title_, QString journal_title_, QString keywords_, QString publisher_, QString editors_, int id_, int year_, int month_, int copies_, int price_, int room_, int level_){
        authors = authors_;
        title = title_;
        journal_title = journal_title_;
        keywords = keywords_;
        publisher = publisher_;
        editors = editors_;
        year = year_;
        month = month_;
        copies = copies_;
        id = id_;
        price = price_;
        room = room_;
        level = level_;
    }
    Article(){};
};

class VA : public Document{
public:
    QString authors;
    VA(QString authors_, QString title_, QString keywords_, int id_, int copies_, int price_, int room_, int level_){
        authors = authors_;
        title = title_;
        keywords = keywords_;
        copies = copies_;
        id = id_;
        price = price_;
        room = room_;
        level = level_;
    }
    VA(){};
};

class Check_out {
public:
    int document_type;
    int document_id;
    int check_out_id;
    int year_start, month_start, day_start;
    int year_end, month_end, day_end;
    int fine, user_id;
    Check_out(int user_id_, int document_type_, int document_id_, int check_out_id_, int year_start_, int month_start_, int day_start_, int year_end_, int month_end_, int day_end_, int fine_){
        user_id = user_id_;
        document_type = document_type_;
        document_id = document_id_;
        check_out_id = check_out_id_;
        year_start = year_start_;
        month_start = month_start_;
        day_start = day_start_;
        year_end = year_end_;
        month_end = month_end_;
        day_end = day_end_;
        fine = fine_;
    }
    Check_out(){};
};

class User{
public:
    QString name, address, phone, login, password;
    int id;
    std::pair<QDate, int> calculate_check_out(int document_type, int year_start, int month_start, int day_start, bool faculty, bool bestseller, int price){
        QDate date_start;
        date_start.setDate(year_start, month_start, day_start);
        QDate date_end;
        if (document_type == 1){
            if (faculty)
                date_end = date_start.addDays(28);
            else if (bestseller)
                date_end = date_start.addDays(14);
            else
                date_end = date_start.addDays(21);
        }
        else date_end = date_start.addDays(14);
        QDate date_now = QDate::currentDate();
        int days_overdue = date_end.daysTo(date_now);
        int fine = 0;
        if (days_overdue > 0)
            fine = min(days_overdue * 100, price);
        return make_pair(date_end, fine);
    }
    QVector<Book> search_books(QString authors, QString title, QString keywords, QString publisher, int year, bool bestseller, bool available, bool or_and){
        QString ins = or_and ? " AND " : " OR ";
        QString req = "SELECT * FROM books WHERE instr(authors, '"+authors+"') > 0" + ins +
                "instr(title, '"+title+"') > 0" + ins +
                "instr(keywords, '"+keywords+"') > 0" + ins +
                "instr(publisher, '"+publisher+"') > 0" + ins +
                "year="+QString::number(year) + ins;
        if (bestseller) req += "bestseller = 1" + ins;
        if (available) req += "copies > 0 " + ins;
        req += " 1 = 1";
        QSqlQuery query;
        query.exec(req);
        QVector<Book> ans;
        while (query.next()) {
            int id = query.value(0).toInt();
            QString title = query.value(1).toString();
            QString authors = query.value(2).toString();
            QString publisher = query.value(3).toString();
            QString keywords = query.value(4).toString();
            int year = query.value(5).toInt();
            int price = query.value(6).toInt();
            int room = query.value(7).toInt();
            int level = query.value(8).toInt();
            int copies = query.value(9).toInt();
            bool bestseller = query.value(10).toInt();
            bool reference = query.value(11).toInt();

            ans.push_back(Book(authors, title, keywords, publisher, id, year, copies, price, room ,level, bestseller, reference));
        }
        return ans;
    }

    Book get_book(int id){
        QSqlQuery query;
        query.exec("SELECT * FROM books WHERE id = " + QString::number(id));
        query.next();
        QString title = query.value(1).toString();
        QString authors = query.value(2).toString();
        QString publisher = query.value(3).toString();
        QString keywords = query.value(4).toString();
        int year = query.value(5).toInt();
        int price = query.value(6).toInt();
        int room = query.value(7).toInt();
        int level = query.value(8).toInt();
        int copies = query.value(9).toInt();
        bool bestseller = query.value(10).toInt();
        bool reference = query.value(11).toInt();
        return Book(authors, title, keywords, publisher, id, year, copies, price, room ,level, bestseller, reference);
    }

    QVector<Article> search_articles(QString authors, QString title, QString keywords, QString journal_title, QString publisher, QString editors, int year, int month, bool available, bool or_and);
    QVector<VA> search_av(QString authors, QString title, QString keywords, bool available, bool or_and);
};

//search/check_out documents, renew/return checked_out documents
class PatronUser : public User{
public:
    bool faculty;
    QVector<int> check_outs;

    void check_out_book(int document_id){
        QSqlQuery query;
        query.prepare("SELECT COUNT(*) FROM check_outs WHERE user_id = :user_id AND document_type = 1 AND document_id = :document_id");
        query.bindValue(":user_id", id);
        query.bindValue(":document_id", document_id);
        query.exec();
        query.next();
        bool already = query.value(0).toInt();
        if(already) return; //cant check out twice

        query.exec("SELECT copies, reference from books WHERE id = " + QString::number(document_id));
        if (!query.next()) return;//book doesnt exist
        int copies = query.value(0).toInt();
        bool reference = query.value(1).toInt();
        if (copies <= 0 || reference) return;


        QDate date = QDate::currentDate();
        int year_start = date.year();
        int month_start = date.month();
        int day_start = date.day();

        query.prepare("UPDATE books SET copies = copies - 1 WHERE id = :document_id");
        query.bindValue(":document_id", document_id);
        query.exec();

        query.prepare("INSERT INTO check_outs (user_id,document_type,document_id,year_start,month_start,day_start) VALUES(:user_id,:document_type,:document_id,:year_start,:month_start,:day_start)");
        query.bindValue(":user_id", id);
        query.bindValue(":document_type", BOOK);
        query.bindValue(":document_id", document_id);
        query.bindValue(":year_start", year_start);
        query.bindValue(":month_start", month_start);
        query.bindValue(":day_start", day_start);
        query.exec();

        query.exec("SELECT COUNT(*) FROM check_outs");
        query.next();
        int check_out_id = query.value(0).toInt();

        query.prepare("UPDATE patrons SET check_outs = check_outs || :check_out_id WHERE id = :user_id");
        query.bindValue(":check_out_id", QString::number(check_out_id)+";");
        query.bindValue(":user_id", id);
        query.exec();
    }
    void add_check_outs(QString str){
        QString id = "";
        for (int i = 0; i < str.size(); i++){
            if (str[i] == ';'){
                check_outs.push_back(id.toInt());
                id = "";
            }
            else id += str[i];
        }
    }

    int check_out_article(int id);
    int check_out_av(int id);
    int return_book(int id){return 1;}
    int return_article(int id);
    int return_av(int id);

    bool renew_book(int id){return 1;}
    bool renew_article(int id);
    bool renew_av(int id);
    QVector<std::pair<Check_out, Book> > get_checked_out_books(){
        QVector<std::pair<Check_out, Book> > ans;
        QSqlQuery query;
        query.exec("SELECT * FROM check_outs WHERE document_type = 1 AND year_end IS NULL AND user_id = "+QString::number(id));
        while (query.next()) {
            int book_id = query.value(3).toInt();
            int year_start = query.value(4).toInt();
            int month_start = query.value(5).toInt();
            int day_start = query.value(6).toInt();
            Book book = get_book(book_id);
            std::pair<QDate, int> end = calculate_check_out(1, year_start, month_start, day_start, faculty, book.bestseller, book.price);
            ans.push_back(make_pair(Check_out(id, 1, book_id, -1, year_start, month_start, day_start, end.first.year(), end.first.month(), end.first.day(), end.second), book));
        }
        return ans;
    }
    QVector<std::pair<Check_out, Article> > get_checked_out_articles();
    QVector<std::pair<Check_out, VA> > get_checked_out_avs();

    PatronUser(int id_, QString name_, QString address_, QString phone_, bool faculty_, QString login_, QString password_, QString check_outs_){
        id = id_;
        name = name_;
        address = address_;
        phone = phone_;
        faculty = faculty_;
        login = login_;
        password = password_;
        add_check_outs(check_outs_);
    }
    PatronUser(){};
};

//search/add/delete/modify users, search/add/delete/modify documents, search overdue documents
class LibrarianUser : public User{
public:
    QVector<std::pair<Check_out, Book> > search_books_checked_out(int user_id, QString authors, QString title, QString keywords, QString publisher, int year, bool bestseller, bool or_and){
        QVector<std::pair<Check_out, Book> > ans;
        QSqlQuery query;
        QString ins = or_and ? " AND " : " OR ";
        QString req = "select * from check_outs where document_type = 1 and user_id = 1 and "
                "instr((SELECT authors FROM books WHERE id = document_id),'Kevin') > 0 and"
                "instr((SELECT title FROM books WHERE id = document_id),'Hack') > 0 and"
                "instr((SELECT keywords FROM books WHERE id = document_id),'hack') > 0 and"
                "instr((SELECT publisher FROM books WHERE id = document_id),'MIT') > 0 and"
                "(SELECT year FROM books WHERE id = document_id) = 2011 and"
                "(SELECT bestseller FROM books WHERE id = document_id) = 1";
        if (bestseller) req += "bestseller = 1" + ins;
        /*
        query.exec("SELECT * FROM check_outs WHERE document_type = 1 AND year_end IS NULL AND user_id = "+QString::number(id));
        while (query.next()) {
            int book_id = query.value(3).toInt();
            int year_start = query.value(4).toInt();
            int month_start = query.value(5).toInt();
            int day_start = query.value(6).toInt();
            Book book = get_book(book_id);
            std::pair<QDate, int> end = calculate_check_out(1, year_start, month_start, day_start, faculty, book.bestseller, book.price);
            ans.push_back(make_pair(Check_out(id, 1, book_id, -1, year_start, month_start, day_start, end.first.year(), end.first.month(), end.first.day(), end.second), book));
        }*/
        return ans;
    }
    QVector<std::pair<Check_out, Article> > search_articles_checked_out(int user_id, QString authors, QString title, QString keywords, QString journal_title, QString publisher, QString editors, int year, int month, bool or_and);
    QVector<std::pair<Check_out, VA> > search_av_checked_out(int user_id, QString authors, QString title, QString keywords, bool or_and);

    QVector<PatronUser> search_patrons(QString name, QString address, QString phone, bool faculty, bool has_overdues, bool or_and);
    QVector<LibrarianUser> search_librarians(QString name, QString address, QString phone, bool or_and);

    bool add_patron(QString name, QString address, QString phone, int id, bool faculty, QString login, QString password);
    bool add_librarian(QString name, QString address, QString phone);

    bool modify_patron(int user_id, QString name, QString address, QString phone, bool faculty, QString login, QString password);
    bool modify_librarian(int user_id, QString name, QString address, QString phone);

    bool delete_patron(int user_id);
    bool delete_librarian(int user_id);

    bool add_book(QString authors, QString title, QString keywords, QString publisher, QString editors, int year, bool bestseller, int copies);
    bool add_article(QString authors, QString title, QString keywords, QString journal_title, QString publisher, QString editors, int year, int month, int copies);
    bool add_av(QString authors, QString title, QString keywords, int available, int copies);

    bool delete_book(int id, int copies);
    bool delete_article(int id, int copies);
    bool delete_av(int id, int copies);

    bool modify_book(int id, QString authors, QString title, QString keywords, QString publisher, int year, bool bestseller, int copies);
    bool modify_article(int id, QString authors, QString title, QString keywords, QString journal_title, QString publisher, QString editors, int year, int month, int copies);
    bool modify_av(int id, QString authors, QString title, QString keywords, bool available, int copies);

    LibrarianUser(int id, QString name_, QString address_, QString phone_, QString login_, QString password_){
        name = name_;
        address = address_;
        phone = phone_;
        login = login_;
        password = password_;
    }
    LibrarianUser(){};
};


class Login{
public:
    PatronUser login_patron(QString login, QString password){
        QSqlQuery query;
        query.prepare("SELECT * from patrons WHERE login = :login and password = :password");
        query.bindValue(":login", login);
        query.bindValue(":password", password);
        query.exec();
        if (query.next()){
            int user_id = query.value(0).toInt();
            QString name = query.value(1).toString();
            QString address = query.value(2).toString();
            QString phone = query.value(3).toString();
            bool faculty = query.value(4).toInt();
            QString check_outs = query.value(5).toString();
            QString user_login = query.value(6).toString();
            QString user_password = query.value(7).toString();
            return PatronUser(user_id, name, address, phone, faculty, user_login, user_password, check_outs);
        }
        return PatronUser(-1,"","","",0,"","","");
    }
    LibrarianUser login_librarian(QString login, QString password){
        QSqlQuery query;
        query.prepare("SELECT * from librarians WHERE login = :login and password = :password");
        query.bindValue(":login", login);
        query.bindValue(":password", password);
        query.exec();
        if (query.next()){
            int user_id = query.value(0).toInt();
            QString name = query.value(1).toString();
            QString address = query.value(2).toString();
            QString phone = query.value(3).toString();
            QString user_login = query.value(4).toString();
            QString user_password = query.value(5).toString();
            return LibrarianUser(user_id, name, address, phone, user_login, user_password);
        }
        return LibrarianUser(-1,"","","","","");
    }
    void make_database(){
        QSqlQuery query;
        query.exec("CREATE TABLE IF NOT EXISTS books ("
                     "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                     "title VARCHAR(255), "
                     "authors VARCHAR(255), "
                     "publisher VARCHAR(255), "
                     "keywords VARCHAR(255), "
                     "year INTEGER, "
                     "price INTEGER, "
                     "room INTEGER, "
                     "level INTEGER, "
                     "copies INTEGER, "
                     "bestseller INTEGER, "
                     "reference INTEGER);");

        query.exec("CREATE TABLE IF NOT EXISTS articles ("
                     "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                     "title VARCHAR(255), "
                     "authors VARCHAR(255), "
                     "publisher VARCHAR(255), "
                     "journal VARCHAR(255), "
                     "editors VARCHAR(255), "
                     "keywords VARCHAR(255), "
                     "year INTEGER, "
                     "month INTEGER, "
                     "price INTEGER, "
                     "room INTEGER, "
                     "level INTEGER, "
                     "copies INTEGER);");

        query.exec("CREATE TABLE IF NOT EXISTS vas ("
                     "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                     "title VARCHAR(255), "
                     "authors VARCHAR(255), "
                     "keywords VARCHAR(255), "
                     "price INTEGER, "
                     "room INTEGER, "
                     "level INTEGER, "
                     "copies INTEGER);");

        query.exec("CREATE TABLE IF NOT EXISTS patrons ("
                     "id INTEGER PRIMARY KEY, "
                     "name VARCHAR(255), "
                     "address VARCHAR(255), "
                     "phone VARCHAR(255), "
                     "faculty INTEGER, "
                     "check_outs VARCHAR(255) DEFAULT ';', "
                     "login VARCHAR(255), "
                     "password VARCHAR(255));");

        query.exec("CREATE TABLE IF NOT EXISTS librarians ("
                     "id INTEGER PRIMARY KEY, "
                     "name VARCHAR(255), "
                     "address VARCHAR(255), "
                     "phone VARCHAR(255), "
                     "login VARCHAR(255), "
                     "password VARCHAR(255));");

        query.exec("CREATE TABLE IF NOT EXISTS check_outs ("
                     "check_out_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                     "user_id INTEGER, "
                     "document_type INTEGER, "
                     "document_id INTEGER, "
                     "year_start INTEGER, "
                     "month_start INTEGER, "
                     "day_start INTEGER, "
                     "year_end INTEGER, "
                     "month_end INTEGER, "
                     "day_end INTEGER);");
    }
};

class Testing{
public:
    Login mainLogin;
    bool testTC1(){
        PatronUser patron = mainLogin.login_patron("1", "1");
        patron.check_out_book(1);//book exist
        QVector<pair<Check_out, Book> > check_outs = patron.get_checked_out_books();
        for (int i = 0; i < check_outs.size(); i++)
            if (check_outs[i].second.id == 1)
                return 1;
        return 0;
    }

    bool testTC2(){
        PatronUser patron = mainLogin.login_patron("1", "1");
        patron.check_out_book(1009);//book does not exist
        QVector<pair<Check_out, Book> > check_outs = patron.get_checked_out_books();
        for (int i = 0; i < check_outs.size(); i++)
            if (check_outs[i].second.id == 1009)
                return 0;
        return 1;
    }

    bool testTC5(){
        PatronUser patron1 = mainLogin.login_patron("1", "1");
        PatronUser patron2 = mainLogin.login_patron("11", "11");
        PatronUser patron3 = mainLogin.login_patron("111", "111");
        patron1.check_out_book(2);
        patron2.check_out_book(2);
        patron3.check_out_book(2);

        QVector<Book> books = patron1.search_books("","","","",2018,0,0,0);
        for (int i = 0; i < books.size(); i++)
            if (books[i].id == 2 && books[i].copies == 0)
                return 1;
        return 0;
    }

    bool testTC6(){
        PatronUser patron = mainLogin.login_patron("1", "1");
        patron.check_out_book(1);
        QVector<pair<Check_out, Book> > check_outs = patron.get_checked_out_books();
        return check_outs.size() == 2;
    }

    bool testTC7(){
        PatronUser patron1 = mainLogin.login_patron("1", "1");
        PatronUser patron2 = mainLogin.login_patron("11", "11");
        patron2.check_out_book(3);
        QThread::sleep(3);
        patron1.check_out_book(3);

        QVector<Book> books = patron1.search_books("","","","",1983,0,0,1);
        return books[0].copies == 0;
    }

    bool testTC10(){
        PatronUser patron = mainLogin.login_patron("1", "1");
        patron.check_out_book(4);
        QVector<pair<Check_out, Book> > check_outs = patron.get_checked_out_books();
        for (int i = 0; i < check_outs.size(); i++)
            if (check_outs[i].second.id == 4)
                return 0;
        return 1;
    }

    void testAll(){
        qDebug() << "testTC1: " << (testTC1() ? "OK" : "FAIL");
        qDebug() << "testTC2: " << (testTC2() ? "OK" : "FAIL");
        qDebug() << "testTC5: " << (testTC5() ? "OK" : "FAIL");
        qDebug() << "testTC6: " << (testTC6() ? "OK" : "FAIL");
        //qDebug() << "testTC7: " << (testTC7() ? "OK" : "FAIL");
        qDebug() << "testTC10: " << (testTC10() ? "OK" : "FAIL");
    }

};