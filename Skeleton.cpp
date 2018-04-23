#pragma once
#include <QString>
#include <QVector>
#include <QtSql>
#include <unistd.h>
#include <hasher.h>
#include <fstream>
#include <queue.h>


using namespace std;

#define BOOK       1
#define ARTICLE    2
#define AV         3

#define PATRON_STUDENT              1
#define PATRON_INSTRUCTOR           2
#define PATRON_TA                   3
#define PATRON_PROFESSOR            4
#define PATRON_VP                   5

class Document{
public:
    QString title, keywords;
    int copies, id, price, room, level;
    QVector<int> wants;
    bool reference;
    //add users, who wants this book from database
    void add_wants(QString str){
        QString id = "";
        for (int i = 0; i < str.size(); i++){
            if (str[i] == ';'){
                if (id != "")
                    wants.push_back(id.toInt());
                id = "";
            }
            else id += str[i];
        }
    }
};

class Book : public Document{
public:
    QString publisher, authors;
    int year;
    bool bestseller;
    Book(QString authors_, QString title_, QString keywords_, QString publisher_, int id_, int year_, int copies_, int price_, int room_, int level_, bool bestseller_, bool reference_, QString wants_str){
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
        add_wants(wants_str);
    }
    Book(){};
};


class Article : public Document{
public:
    QString journal_title, publisher, editors, authors;
    int year, month;
    Article(QString authors_, QString title_, QString journal_title_, QString keywords_, QString publisher_, QString editors_, int id_, int year_, int month_, int copies_, int price_, int room_, int level_, bool reference_, QString wants_str){
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
        reference = reference_;
        add_wants(wants_str);
    }
    Article(){};
};

class VA : public Document{
public:
    QString authors;
    VA(QString authors_, QString title_, QString keywords_, int id_, int copies_, int price_, int room_, int level_, bool reference_, QString wants_str){
        authors = authors_;
        title = title_;
        keywords = keywords_;
        copies = copies_;
        id = id_;
        price = price_;
        room = room_;
        level = level_;
        reference = reference_;
        add_wants(wants_str);
    }
    VA(){};
};

class Check_out {
public:
    int user_id;
    int document_type, document_id;
    int check_out_id, fine;
    int year_start, month_start, day_start;
    int year_end, month_end, day_end;
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

    //bestseller can be any if not book given
    std::pair<QDate, int> calculate_check_out(int document_type, int year_start, int month_start, int day_start, int role, bool bestseller, int price, int renew_state){
        QDate date_start;
        date_start.setDate(year_start, month_start, day_start);
        QDate date_end;
        int days_to_add = 0;
        if (document_type == 1){
            if (role >= 2 && role <= 4)
                days_to_add = 28;
            else if (role == 5)
                days_to_add = 7;
            else if (bestseller)
                days_to_add = 14;
            else
                days_to_add = 21;
        }
        else days_to_add = 14;
        if (renew_state == 2){//if renewed
            QSqlQuery query;
            query.exec("SELECT * FROM settings");//need to know how many days to add
            query.next();
            days_to_add += query.value(0).toInt();
        }
        date_end = date_start.addDays(days_to_add);

        QDate date_now = QDate::currentDate();
        int days_overdue = date_end.daysTo(date_now);
        int fine = 0;
        if (days_overdue > 0)
            fine = min(days_overdue * 100, price);
        return make_pair(date_end, fine);
    }
    QVector<Book> search_books(QString authors, QString title, QString keywords, QString publisher, int year, bool bestseller, bool available, bool or_and){
        QSqlQuery query;
        QString ins = or_and ? " AND " : " OR ";
        authors = authors.toLower();
        title = title.toLower();
        keywords = keywords.toLower();
        publisher = publisher.toLower();
        QString req = "SELECT * FROM books WHERE ";
        if (authors != "") req += "instr(lower(authors), '"+authors+"') > 0" + ins;
        if (title != "") req += "instr(lower(title), '"+title+"') > 0" + ins;
        if (keywords != "") req += "instr(lower(keywords), '"+keywords+"') > 0" + ins;
        if (publisher != "") req += "instr(lower(publisher), '"+publisher+"') > 0" + ins;
        if (year != 0) req += "instr(year, '"+QString::number(year)+"') > 0" + ins;
        if (bestseller) req += "bestseller = 1" + ins;
        if (available) req += "copies > 0" + ins;
        req += "1 = " + QString(or_and ? "1" : "0");//nice hack to finish statement correctly
        if (req.length() == 31)//no parameters given
            req = "SELECT * FROM books";
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
            QString wants_str = query.value(12).toString();
            ans.push_back(Book(authors, title, keywords, publisher, id, year, copies, price, room ,level, bestseller, reference, wants_str));
        }
        return ans;
    }
    QVector<Article> search_articles(QString authors, QString title, QString keywords, QString journal, QString publisher, QString editors, int year, int month, bool available, bool or_and){
        QSqlQuery query;
        QString ins = or_and ? " AND " : " OR ";
        title = title.toLower();
        authors = authors.toLower();
        keywords = keywords.toLower();
        publisher = publisher.toLower();
        editors = editors.toLower();
        journal = journal.toLower();
        QString req = "SELECT * FROM articles WHERE ";
        if (authors != "") req += "instr(lower(authors), '"+authors+"') > 0" + ins;
        if (title != "") req += "instr(lower(title), '"+title+"') > 0" + ins;
        if (keywords != "") req += "instr(lower(keywords), '"+keywords+"') > 0" + ins;
        if (publisher != "") req += "instr(lower(publisher), '"+publisher+"') > 0" + ins;
        if (editors != "") req += "instr(lower(editors), '"+editors+"') > 0" + ins;
        if (journal != "") req += "instr(lower(journal), '"+journal+"') > 0" + ins;
        if (year != 0) req += "instr(year, '"+QString::number(year)+"') > 0" + ins;
        if (month != 0) req += "instr(month, '"+QString::number(month)+"') > 0" + ins;
        if (available) req += "copies > 0" + ins;
        req += "1 = " + QString(or_and ? "1" : "0");
        if (req.length() == 34)
            req = "SELECT * FROM articles";
        query.exec(req);
        QVector<Article> ans;
        while (query.next()) {
            int id = query.value(0).toInt();
            QString title = query.value(1).toString();
            QString authors = query.value(2).toString();
            QString publisher = query.value(3).toString();
            QString journal = query.value(4).toString();
            QString editors = query.value(5).toString();
            QString keywords = query.value(6).toString();
            int year = query.value(7).toInt();
            int month = query.value(8).toInt();
            int price = query.value(9).toInt();
            int room = query.value(10).toInt();
            int level = query.value(11).toInt();
            int copies = query.value(12).toInt();
            bool reference = query.value(13).toInt();
            QString wants_str = query.value(14).toString();
            ans.push_back(Article(authors, title, journal, keywords, publisher, editors, id, year, month, copies, price, room, level, reference, wants_str));
        }
        return ans;
    }
    QVector<VA> search_vas(QString authors, QString title, QString keywords, bool available, bool or_and){
        QSqlQuery query;

        QString ins = or_and ? " AND " : " OR ";
        title = title.toLower();
        authors = authors.toLower();
        keywords = keywords.toLower();

        QString req = "SELECT * FROM vas WHERE ";
        if (authors != "") req += "instr(lower(authors), '"+authors+"') > 0" + ins;
        if (title != "") req += "instr(lower(title), '"+title+"') > 0" + ins;
        if (keywords != "") req += "instr(lower(keywords), '"+keywords+"') > 0" + ins;
        if (available) req += "copies > 0" + ins;
        req += "1 = " + QString(or_and ? "1" : "0");
        if (req.length() == 29)
            req = "SELECT * FROM vas";
        query.exec(req);
        QVector<VA> ans;
        while (query.next()) {
            int id = query.value(0).toInt();
            QString title = query.value(1).toString();
            QString authors = query.value(2).toString();
            QString keywords = query.value(3).toString();
            int price = query.value(4).toInt();
            int room = query.value(5).toInt();
            int level = query.value(6).toInt();
            int copies = query.value(7).toInt();
            bool reference = query.value(8).toInt();
            QString wants_str = query.value(9).toString();
            ans.push_back(VA(authors, title, keywords, id, copies, price, room, level, reference, wants_str));
        }
        return ans;
    }

    Book get_book(int book_id){
        QSqlQuery query;
        query.exec("SELECT * FROM books WHERE id = " + QString::number(book_id));
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
        QString wants_str = query.value(12).toString();
        return Book(authors, title, keywords, publisher, book_id, year, copies, price, room ,level, bestseller, reference, wants_str);
    }
    Article get_article(int article_id){
        QSqlQuery query;
        query.exec("SELECT * FROM articles WHERE id = " + QString::number(article_id));
        query.next();
        QString title = query.value(1).toString();
        QString authors = query.value(2).toString();
        QString publisher = query.value(3).toString();
        QString journal = query.value(4).toString();
        QString editors = query.value(5).toString();
        QString keywords = query.value(6).toString();
        int year = query.value(7).toInt();
        int month = query.value(8).toInt();
        int price = query.value(9).toInt();
        int room = query.value(10).toInt();
        int level = query.value(11).toInt();
        int copies = query.value(12).toInt();
        bool reference = query.value(13).toInt();
        QString wants_str = query.value(14).toString();
        return Article(authors, title, journal, keywords, publisher, editors, article_id, year, month, copies, price, room, level, reference, wants_str);
    }
    VA get_va(int va_id){
        QSqlQuery query;
        query.exec("SELECT * FROM vas WHERE id = " + QString::number(va_id));
        query.next();
        QString title = query.value(1).toString();
        QString authors = query.value(2).toString();
        QString keywords = query.value(3).toString();
        int price = query.value(4).toInt();
        int room = query.value(5).toInt();
        int level = query.value(6).toInt();
        int copies = query.value(7).toInt();
        bool reference = query.value(8).toInt();
        QString wants_str = query.value(9).toString();
        return VA(authors, title, keywords, va_id, copies, price, room, level, reference, wants_str);
    }
};

//search/check_out documents, renew/return checked_out documents
class PatronUser : public User{
public:
    bool faculty;
    int role;
    QVector<int> check_outs;

    void check_out_book(int document_id){
        QSqlQuery query;
        query.prepare("SELECT COUNT(*) FROM check_outs WHERE user_id = :user_id AND document_type = 1 AND document_id = :document_id AND year_end IS NULL");
        query.bindValue(":user_id", id);
        query.bindValue(":document_id", document_id);
        query.exec();
        query.next();
        bool already = query.value(0).toInt();
        if(already) return; //cant check out twice

        query.exec("SELECT copies, reference from books WHERE id = " + QString::number(document_id));
        if (!query.next()) return;//book doesnt exist
        int copies = query.value(0).toInt();
        bool reference = query.value(1).toInt() ;
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

        //get last check out id
        query.exec("SELECT check_out_id FROM check_outs ORDER BY check_out_id DESC LIMIT 1");
        query.next();
        int check_out_id = query.value(0).toInt();

        query.prepare("UPDATE patrons SET check_outs = check_outs || :check_out_id WHERE id = :user_id");
        query.bindValue(":check_out_id", QString::number(check_out_id)+";");
        query.bindValue(":user_id", id);
        query.exec();
    }
    void check_out_article(int document_id) {
        QSqlQuery query;
        query.prepare("SELECT COUNT(*) FROM check_outs WHERE user_id = :user_id AND document_type = 2 AND document_id = :document_id AND year_end IS NULL");
        query.bindValue(":user_id", id);
        query.bindValue(":document_id", document_id);
        query.exec();
        query.next();
        bool already = query.value(0).toInt();
        if(already) return; //cant check out twice

        query.exec("SELECT copies, reference from articles WHERE id = " + QString::number(document_id));
        if (!query.next()) return;//book doesnt exist
        int copies = query.value(0).toInt();
        bool reference = query.value(1).toInt() ;
        if (copies <= 0 || reference) return;


        QDate date = QDate::currentDate();
        int year_start = date.year();
        int month_start = date.month();
        int day_start = date.day();

        query.prepare("UPDATE articles SET copies = copies - 1 WHERE id = :document_id");
        query.bindValue(":document_id", document_id);
        query.exec();

        query.prepare("INSERT INTO check_outs (user_id,document_type,document_id,year_start,month_start,day_start) VALUES(:user_id,:document_type,:document_id,:year_start,:month_start,:day_start)");
        query.bindValue(":user_id", id);
        query.bindValue(":document_type", ARTICLE);
        query.bindValue(":document_id", document_id);
        query.bindValue(":year_start", year_start);
        query.bindValue(":month_start", month_start);
        query.bindValue(":day_start", day_start);
        query.exec();

        //get last check out id
        query.exec("SELECT check_out_id FROM check_outs ORDER BY check_out_id DESC LIMIT 1");
        query.next();
        int check_out_id = query.value(0).toInt();

        query.prepare("UPDATE patrons SET check_outs = check_outs || :check_out_id WHERE id = :user_id");
        query.bindValue(":check_out_id", QString::number(check_out_id)+";");
        query.bindValue(":user_id", id);
        query.exec();
    }
    void check_out_av(int document_id){
        QSqlQuery query;
        query.prepare("SELECT COUNT(*) FROM check_outs WHERE user_id = :user_id AND document_type = 3 AND document_id = :document_id AND year_end IS NULL");
        query.bindValue(":user_id", id);
        query.bindValue(":document_id", document_id);
        query.exec();
        query.next();
        bool already = query.value(0).toInt();
        if(already) return; //cant check out twice

        query.exec("SELECT copies, reference from vas WHERE id = " + QString::number(document_id));
        if (!query.next()) return;//book doesnt exist
        int copies = query.value(0).toInt();
        bool reference = query.value(1).toInt() ;
        if (copies <= 0 || reference) return;


        QDate date = QDate::currentDate();
        int year_start = date.year();
        int month_start = date.month();
        int day_start = date.day();

        query.prepare("UPDATE vas SET copies = copies - 1 WHERE id = :document_id");
        query.bindValue(":document_id", document_id);
        query.exec();

        query.prepare("INSERT INTO check_outs (user_id,document_type,document_id,year_start,month_start,day_start) VALUES(:user_id,:document_type,:document_id,:year_start,:month_start,:day_start)");
        query.bindValue(":user_id", id);
        query.bindValue(":document_type", AV);
        query.bindValue(":document_id", document_id);
        query.bindValue(":year_start", year_start);
        query.bindValue(":month_start", month_start);
        query.bindValue(":day_start", day_start);
        query.exec();

        //get last check out id
        query.exec("SELECT check_out_id FROM check_outs ORDER BY check_out_id DESC LIMIT 1");
        query.next();
        int check_out_id = query.value(0).toInt();

        query.prepare("UPDATE patrons SET check_outs = check_outs || :check_out_id WHERE id = :user_id");
        query.bindValue(":check_out_id", QString::number(check_out_id)+";");
        query.bindValue(":user_id", id);
        query.exec();
    }


    //parse string and add my check outs
    void add_check_outs(QString str){
        QString id = "";
        for (int i = 0; i < str.size(); i++){
            if (str[i] == ';'){
                if (id != "")
                    check_outs.push_back(id.toInt());
                id = "";
            }
            else id += str[i];
        }
    }

    void want_book(int document_id){
        //set renew_state to 1
        QSqlQuery query;
        query.prepare("UPDATE check_outs SET renew_state = 1 WHERE document_type = 1 AND renew_state = 0 AND year_end IS NULL AND document_id = :document_id");
        query.bindValue(":document_id", document_id);
        query.exec();

        query.exec("SELECT wants FROM books WHERE id = " + document_id);
        query.next();
        QString wants = query.value(0).toString();

        Queue queue;
        queue.from_db(wants);
        queue.add_patron(qMakePair(role, id));

        Book book = get_book(document_id);
        if (!book.wants.contains(id)){
            query.prepare("UPDATE books SET wants = :wants WHERE id = :document_id");
            query.bindValue(":wants", queue.to_db());
            query.bindValue(":document_id", document_id);
            query.exec();
        }
    }
    void want_article(int document_id){
        //set renew_state to 1
        QSqlQuery query;
        query.prepare("UPDATE check_outs SET renew_state = 1 WHERE document_type = 2 AND renew_state = 0 AND year_end IS NULL AND document_id = :document_id");
        query.bindValue(":document_id", document_id);
        query.exec();

        query.exec("SELECT wants FROM articles WHERE id = " + document_id);
        query.next();
        QString wants = query.value(0).toString();

        Queue queue;
        queue.from_db(wants);
        queue.add_patron(qMakePair(role, id));


        Article article = get_article(document_id);
        if (!article.wants.contains(id)){
            query.prepare("UPDATE articles SET wants = :wants WHERE id = :document_id");
            query.bindValue(":wants", queue.to_db());
            query.bindValue(":document_id", document_id);
            query.exec();
        }
    }
    void want_va(int document_id){
        //set renew_state to 1
        QSqlQuery query;
        query.prepare("UPDATE check_outs SET renew_state = 1 WHERE document_type = 3 AND renew_state = 0 AND year_end IS NULL AND document_id = :document_id");
        query.bindValue(":document_id", document_id);
        query.exec();

        query.exec("SELECT wants FROM vas WHERE id = " + document_id);
        query.next();
        QString wants = query.value(0).toString();

        Queue queue;
        queue.from_db(wants);
        queue.add_patron(qMakePair(role, id));

        VA va = get_va(document_id);
        if (!va.wants.contains(id)){
            query.prepare("UPDATE vas SET wants = :wants WHERE id = :document_id");
            query.bindValue(":wants", queue.to_db());
            query.bindValue(":document_id", document_id);
            query.exec();
        }
    }

    int renew_book(int check_out_id){
        QSqlQuery query;
        QDate today = QDate::currentDate();

        query.prepare("SELECT * FROM check_outs WHERE check_out_id = :check_out_id");
        query.bindValue(":check_out_id", check_out_id);
        query.exec();
        if (!query.next()) return 0; //no such check out

        int book_id = query.value(3).toInt();
        int year_start = query.value(4).toInt();
        int month_start = query.value(5).toInt();
        int day_start = query.value(6).toInt();
        int renew_state = query.value(10).toInt();

        if (id != PATRON_VP && renew_state != 0) return 1; //someone wants

        Book book = get_book(book_id);
        std::pair<QDate, int> end = calculate_check_out(1, year_start, month_start, day_start, role, book.bestseller, book.price, renew_state);

        if (id != PATRON_VP && today.daysTo(end.first) != 0 && today.daysTo(end.first) != 1)
            return 2; //too late or early

        query.prepare("UPDATE check_outs SET renew_state = 2 WHERE check_out_id = :check_out_id");
        query.bindValue(":check_out_id", check_out_id);
        query.exec();
        return 3;
    }
    int renew_article(int check_out_id){
        QSqlQuery query;
        QDate today = QDate::currentDate();

        query.prepare("SELECT * FROM check_outs WHERE check_out_id = :check_out_id");
        query.bindValue(":check_out_id", check_out_id);
        query.exec();
        if (!query.next()) return 0; //no such check out

        int article_id = query.value(3).toInt();
        int year_start = query.value(4).toInt();
        int month_start = query.value(5).toInt();
        int day_start = query.value(6).toInt();
        int renew_state = query.value(10).toInt();

        if (renew_state != 0) return 1; //someone wants

        Article article = get_article(article_id);
        std::pair<QDate, int> end = calculate_check_out(2, year_start, month_start, day_start, role, 0, article.price, renew_state);

        if (today.daysTo(end.first) != 0 && today.daysTo(end.first) != 1)
            return 2; //too late or early

        query.prepare("UPDATE check_outs SET renew_state = 2 WHERE check_out_id = :check_out_id");
        query.bindValue(":check_out_id", check_out_id);
        query.exec();
        return 3;
    }
    int renew_va(int check_out_id){
        QSqlQuery query;
        QDate today = QDate::currentDate();

        query.prepare("SELECT * FROM check_outs WHERE check_out_id = :check_out_id");
        query.bindValue(":check_out_id", check_out_id);
        query.exec();
        if (!query.next()) return 0; //no such check out

        int va_id = query.value(3).toInt();
        int year_start = query.value(4).toInt();
        int month_start = query.value(5).toInt();
        int day_start = query.value(6).toInt();
        int renew_state = query.value(10).toInt();

        if (renew_state != 0) return 1; //someone wants

        VA va = get_va(va_id);
        std::pair<QDate, int> end = calculate_check_out(3, year_start, month_start, day_start, role, 0, va.price, renew_state);

        if (today.daysTo(end.first) != 0 && today.daysTo(end.first) != 1)
            return 2; //too late or early

        query.prepare("UPDATE check_outs SET renew_state = 2 WHERE check_out_id = :check_out_id");
        query.bindValue(":check_out_id", check_out_id);
        query.exec();
        return 3;
    }
    QVector<std::pair<Check_out, Book> > get_checked_out_books(){
        QSqlQuery query;
        QVector<std::pair<Check_out, Book> > ans;
        query.exec("SELECT * FROM check_outs WHERE document_type = 1 AND year_end IS NULL AND user_id = " + QString::number(id));
        while (query.next()) {
            int check_out_id = query.value(0).toInt();
            int book_id = query.value(3).toInt();
            int year_start = query.value(4).toInt();
            int month_start = query.value(5).toInt();
            int day_start = query.value(6).toInt();
            int state_renew = query.value(10).toInt();
            Book book = get_book(book_id);
            std::pair<QDate, int> end = calculate_check_out(1, year_start, month_start, day_start, role, book.bestseller, book.price, state_renew);
            ans.push_back(make_pair(Check_out(id, 1, book_id, check_out_id, year_start, month_start, day_start, end.first.year(), end.first.month(), end.first.day(), end.second), book));
        }
        return ans;
    }
    QVector<std::pair<Check_out, Article> > get_checked_out_articles(){
        QSqlQuery query;
        QVector<std::pair<Check_out, Article> > ans;
        query.exec("SELECT * FROM check_outs WHERE document_type = 2 AND year_end IS NULL AND user_id = " + QString::number(id));
        while (query.next()) {
            int check_out_id = query.value(0).toInt();
            int article_id = query.value(3).toInt();
            int year_start = query.value(4).toInt();
            int month_start = query.value(5).toInt();
            int day_start = query.value(6).toInt();
            int state_renew = query.value(10).toInt();
            Article article = get_article(article_id);
            std::pair<QDate, int> end = calculate_check_out(2, year_start, month_start, day_start, role, 0, article.price, state_renew);
            ans.push_back(make_pair(Check_out(id, 2, article_id, check_out_id, year_start, month_start, day_start, end.first.year(), end.first.month(), end.first.day(), end.second), article));
        }
        return ans;
    }
    QVector<std::pair<Check_out, VA> > get_checked_out_vas(){
        QSqlQuery query;
        QVector<std::pair<Check_out, VA> > ans;
        query.exec("SELECT * FROM check_outs WHERE document_type = 3 AND year_end IS NULL AND user_id = " + QString::number(id));
        while (query.next()) {
            int check_out_id = query.value(0).toInt();
            int va_id = query.value(3).toInt();
            int year_start = query.value(4).toInt();
            int month_start = query.value(5).toInt();
            int day_start = query.value(6).toInt();
            int state_renew = query.value(10).toInt();
            VA va = get_va(va_id);
            std::pair<QDate, int> end = calculate_check_out(3, year_start, month_start, day_start, role, 0, va.price, state_renew);
            ans.push_back(make_pair(Check_out(id, 3, va_id, check_out_id, year_start, month_start, day_start, end.first.year(), end.first.month(), end.first.day(), end.second), va));
        }
        return ans;
    }

    bool delete_me(){
        QSqlQuery query;
        if (check_outs.size() > 0)
            return 0; //cant delete if has check outs
        query.exec("DELETE FROM patrons WHERE id = " + QString::number(id));
        return 1;
    }

    PatronUser(int id_, QString name_, QString address_ , QString phone_, int role_, QString login_, QString password_, QString check_outs_){
        id = id_;
        name = name_;
        address = address_;
        phone = phone_;
        role = role_;
        login = login_;
        password = password_;
        add_check_outs(check_outs_);
    }
    PatronUser(){};
};

//search/add/delete/modify users, search/add/delete/modify documents, search overdue documents
class LibrarianUser : public User{
public:
    int privileges;
    QVector<std::pair<Check_out, Book> > search_books_checked_out(int user_id, QString authors, QString title, QString keywords, QString publisher, int year, bool bestseller, bool overdue, bool or_and){
        QSqlQuery query;
        QVector<Book> books = search_books(authors, title, keywords, publisher, year, bestseller, 0, or_and);
        QVector<std::pair<Check_out, Book> > ans;
        for (int i = 0; i < books.size(); i++){
            QString req = "SELECT * FROM check_outs WHERE document_type = 1 AND year_end IS NULL AND document_id = " + QString::number(books[i].id);
            if (user_id != 0) req += QString(or_and ? " AND" : " OR") + " user_id = " + QString::number(user_id);
            query.exec(req);
            while (query.next()) {
                int check_out_id = query.value(0).toInt();
                int current_user_id = query.value(1).toInt();
                int book_id = query.value(3).toInt();
                int year_start = query.value(4).toInt();
                int month_start = query.value(5).toInt();
                int day_start = query.value(6).toInt();
                int state_renew = query.value(10).toInt();
                Book book = get_book(book_id);
                PatronUser patron = get_patron(current_user_id);
                std::pair<QDate, int> end = calculate_check_out(1, year_start, month_start, day_start, patron.role, book.bestseller, book.price, state_renew);
                QDate return_date;
                return_date.setDate(end.first.year(), end.first.month(), end.first.day());
                if (overdue && QDate::currentDate().daysTo(return_date) >= 0) continue;
                ans.push_back(make_pair(Check_out(current_user_id, 1, book_id, check_out_id, year_start, month_start, day_start, end.first.year(), end.first.month(), end.first.day(), end.second), book));
            }
        }
        return ans;
    }
    QVector<std::pair<Check_out, Article> > search_articles_checked_out(int user_id, QString authors, QString title, QString keywords, QString journal, QString publisher, QString editors, int year, int month, bool overdue, bool or_and){
        QSqlQuery query;
        QVector<Article> articles = search_articles(authors, title, keywords, journal, publisher, editors, year, month, 0, or_and);
        QVector<std::pair<Check_out, Article> > ans;
        for (int i = 0; i < articles.size(); i++){
            QString req = "SELECT * FROM check_outs WHERE document_type = 2 AND year_end IS NULL AND document_id = " + QString::number(articles[i].id);
            if (user_id != 0) req += QString(or_and ? " AND" : " OR") + " user_id = " + QString::number(user_id);
            query.exec(req);
            while (query.next()) {
                int check_out_id = query.value(0).toInt();
                int current_user_id = query.value(1).toInt();
                int article_id = query.value(3).toInt();
                int year_start = query.value(4).toInt();
                int month_start = query.value(5).toInt();
                int day_start = query.value(6).toInt();
                int state_renew = query.value(10).toInt();
                Article article = get_article(article_id);
                PatronUser patron = get_patron(current_user_id);
                std::pair<QDate, int> end = calculate_check_out(2, year_start, month_start, day_start, patron.role, 0, article.price, state_renew);
                QDate return_date;
                return_date.setDate(end.first.year(), end.first.month(), end.first.day());
                if (overdue && QDate::currentDate().daysTo(return_date) >= 0) continue;
                ans.push_back(make_pair(Check_out(current_user_id, 2, article_id, check_out_id, year_start, month_start, day_start, end.first.year(), end.first.month(), end.first.day(), end.second), article));
            }
        }
        return ans;
    }
    QVector<std::pair<Check_out, VA> > search_vas_checked_out(int user_id, QString authors, QString title, QString keywords, bool overdue, bool or_and){
        QSqlQuery query;
        QVector<VA> vas = search_vas(authors, title, keywords, 0, or_and);
        QVector<std::pair<Check_out, VA> > ans;
        for (int i = 0; i < vas.size(); i++){
            QString req = "SELECT * FROM check_outs WHERE document_type = 3 AND year_end IS NULL AND document_id = " + QString::number(vas[i].id);
            if (user_id != 0) req += QString(or_and ? " AND" : " OR") + " user_id = " + QString::number(user_id);
            query.exec(req);
            while (query.next()) {
                int check_out_id = query.value(0).toInt();
                int current_user_id = query.value(1).toInt();
                int va_id = query.value(3).toInt();
                int year_start = query.value(4).toInt();
                int month_start = query.value(5).toInt();
                int day_start = query.value(6).toInt();
                int state_renew = query.value(10).toInt();
                VA va = get_va(va_id);
                PatronUser patron = get_patron(current_user_id);
                std::pair<QDate, int> end = calculate_check_out(3, year_start, month_start, day_start, patron.role, 0, va.price, state_renew);
                QDate return_date;
                return_date.setDate(end.first.year(), end.first.month(), end.first.day());
                if (overdue && QDate::currentDate().daysTo(return_date) >= 0) continue;
                ans.push_back(make_pair(Check_out(current_user_id, 3, va_id, check_out_id, year_start, month_start, day_start, end.first.year(), end.first.month(), end.first.day(), end.second), va));
            }
        }
        return ans;
    }

    int remove_last_wants_book(int document_id){
        QSqlQuery query;
        query.exec("SELECT wants FROM books WHERE id = " + document_id);
        query.next();
        QString wants = query.value(0).toString();
        Queue queue;
        queue.from_db(wants);

        int next_id = queue.next_user();
        if (next_id == -1)
            return -1;
        queue.delete_next();


        query.prepare("UPDATE books SET wants = :wants WHERE id = :document_id");
        query.bindValue(":wants", queue.to_db());
        query.bindValue(":document_id", document_id);
        query.exec();
        return next_id;
    }
    int remove_last_wants_article(int document_id){
        QSqlQuery query;
        query.exec("SELECT wants FROM articles WHERE id = " + document_id);
        query.next();
        QString wants = query.value(0).toString();
        Queue queue;
        queue.from_db(wants);

        int next_id = queue.next_user();
        if (next_id == -1)
            return -1;
        queue.delete_next();

        query.prepare("UPDATE articles SET wants = :wants WHERE id = :document_id");
        query.bindValue(":wants", queue.to_db());
        query.bindValue(":document_id", document_id);
        query.exec();
        return next_id;
    }
    int remove_last_wants_va(int document_id){
        QSqlQuery query;
        query.exec("SELECT wants FROM vas WHERE id = " + document_id);
        query.next();
        QString wants = query.value(0).toString();
        Queue queue;
        queue.from_db(wants);

        int next_id = queue.next_user();
        if (next_id == -1)
            return -1;
        queue.delete_next();

        query.prepare("UPDATE vas SET wants = :wants WHERE id = :document_id");
        query.bindValue(":wants", queue.to_db());
        query.bindValue(":document_id", document_id);
        query.exec();
        return next_id;
    }

    void set_settings(int days_add_renew){
        QSqlQuery query;
        query.prepare("UPDATE settings SET days_add_renew = :days_add_renew");
        query.bindValue(":days_add_renew", days_add_renew);
        query.exec();
    }
    int get_days_add_renew(){
        QSqlQuery query;
        query.exec("SELECT days_add_renew FROM settings");
        query.next();
        return query.value(0).toInt();
    }
    //fine, wants_user_id
    std::pair<int, int> return_book(int check_out_id){
        QSqlQuery query;
        query.exec("SELECT * FROM check_outs WHERE document_type = 1 AND check_out_id = " + QString::number(check_out_id));
        if (!query.next()) return make_pair(-1, -1);
        int user_id = query.value(1).toInt();
        int book_id = query.value(3).toInt();
        int year_start = query.value(4).toInt();
        int month_start = query.value(5).toInt();
        int day_start = query.value(6).toInt();
        int renew_state = query.value(10).toInt();

        PatronUser patron = get_patron(user_id);
        Book book = get_book(book_id);
        std::pair<QDate, int> end = calculate_check_out(1, year_start, month_start, day_start, patron.role, book.bestseller, book.price, renew_state);
        int fine = end.second;

        query.prepare("UPDATE check_outs SET year_end = :year_end, month_end = :month_end, day_end = :day_end WHERE check_out_id = :check_out_id");
        query.bindValue(":check_out_id", check_out_id);
        query.bindValue(":year_end", end.first.year());
        query.bindValue(":month_end", end.first.month());
        query.bindValue(":day_end", end.first.day());
        query.exec();

        query.prepare("UPDATE patrons SET check_outs = replace(check_outs, :check_out_id_str, '') WHERE id = :user_id");
        query.bindValue(":check_out_id_str", ";" + QString::number(check_out_id));
        query.bindValue(":user_id", user_id);
        query.exec();

        query.prepare("UPDATE books SET copies = copies + 1 WHERE id = :document_id");
        query.bindValue(":document_id", book.id);
        query.exec();

        query.prepare("DELETE FROM check_outs WHERE check_out_id = :check_out_id");
        query.bindValue(":check_out_id", check_out_id);
        query.exec();

        int wants_id = remove_last_wants_book(book.id);
        if (wants_id != -1)
            return make_pair(fine, wants_id);
        return make_pair(fine, -1);
    }
    std::pair<int, int> return_article(int check_out_id){
        QSqlQuery query;
        query.exec("SELECT * FROM check_outs WHERE document_type = 2 AND check_out_id = " + QString::number(check_out_id));
        if (!query.next()) return make_pair(-1, -1);
        int user_id = query.value(1).toInt();
        int article_id = query.value(3).toInt();
        int year_start = query.value(4).toInt();
        int month_start = query.value(5).toInt();
        int day_start = query.value(6).toInt();
        int renew_state = query.value(10).toInt();

        PatronUser patron = get_patron(user_id);
        Article article = get_article(article_id);
        std::pair<QDate, int> end = calculate_check_out(2, year_start, month_start, day_start, patron.role, 0, article.price, renew_state);
        int fine = end.second;

        query.prepare("UPDATE check_outs SET year_end = :year_end, month_end = :month_end, day_end = :day_end WHERE check_out_id = :check_out_id");
        query.bindValue(":check_out_id", check_out_id);
        query.bindValue(":year_end", end.first.year());
        query.bindValue(":month_end", end.first.month());
        query.bindValue(":day_end", end.first.day());
        query.exec();

        //remove check out from my list
        query.prepare("UPDATE patrons SET check_outs = replace(check_outs, :check_out_id_str, '') WHERE id = :user_id");
        query.bindValue(":check_out_id_str", ";" + QString::number(check_out_id));
        query.bindValue(":user_id", user_id);
        query.exec();

        query.prepare("UPDATE articles SET copies = copies + 1 WHERE id = :document_id");
        query.bindValue(":document_id", article.id);
        query.exec();

        query.prepare("DELETE FROM check_outs WHERE check_out_id = :check_out_id");
        query.bindValue(":check_out_id", check_out_id);
        query.exec();

        int wants_id = remove_last_wants_article(article.id);
        if (wants_id != -1)
            return make_pair(fine, wants_id);
        return make_pair(fine, -1);
    }
    std::pair<int, int> return_va(int check_out_id){
        QSqlQuery query;
        query.exec("SELECT * FROM check_outs WHERE document_type = 3 AND check_out_id = " + QString::number(check_out_id));
        if (!query.next()) return make_pair(-1, -1);
        int user_id = query.value(1).toInt();
        int va_id = query.value(3).toInt();
        int year_start = query.value(4).toInt();
        int month_start = query.value(5).toInt();
        int day_start = query.value(6).toInt();
        int renew_state = query.value(10).toInt();

        PatronUser patron = get_patron(user_id);
        VA va = get_va(va_id);
        std::pair<QDate, int> end = calculate_check_out(3, year_start, month_start, day_start, patron.role, 0, va.price, renew_state);
        int fine = end.second;

        query.prepare("UPDATE check_outs SET year_end = :year_end, month_end = :month_end, day_end = :day_end WHERE check_out_id = :check_out_id");
        query.bindValue(":check_out_id", check_out_id);
        query.bindValue(":year_end", end.first.year());
        query.bindValue(":month_end", end.first.month());
        query.bindValue(":day_end", end.first.day());
        query.exec();

        //remove check out from my list
        query.prepare("UPDATE patrons SET check_outs = replace(check_outs, :check_out_id_str, '') WHERE id = :user_id");
        query.bindValue(":check_out_id_str", ";" + QString::number(check_out_id));
        query.bindValue(":user_id", user_id);
        query.exec();

        query.prepare("UPDATE vas SET copies = copies + 1 WHERE id = :document_id");
        query.bindValue(":document_id", va.id);
        query.exec();

        query.prepare("DELETE FROM check_outs WHERE check_out_id = :check_out_id");
        query.bindValue(":check_out_id", check_out_id);
        query.exec();

        int wants_id = remove_last_wants_va(va.id);
        if (wants_id != -1)
            return make_pair(fine, wants_id);
        return make_pair(fine, -1);
    }

    QVector<PatronUser> search_patrons(int user_id, QString name, QString address, QString phone, int role, bool or_and){
        QSqlQuery query;
        QString ins = or_and ? " AND " : " OR ";
        name = name.toLower();
        address = address.toLower();
        phone = phone.toLower();
        QString req = "SELECT * FROM patrons WHERE ";
        if (user_id != 0) req += "id = " + QString::number(user_id) + ins;
        if (name != "") req += "instr(lower(name), '"+name+"') > 0" + ins;
        if (address != "") req += "instr(lower(address), '"+address+"') > 0" + ins;
        if (phone != "") req += "instr(lower(phone), '"+phone+"') > 0" + ins;
        if (role) req += "role = " + QString::number(role) + ins;
        req += "1 = " + QString(or_and ? "1" : "0");//nice hack to finish statement correctly
        if (req.length() == 33)//no parameters given
            req = "SELECT * FROM patrons";
        query.exec(req);
        QVector<PatronUser> ans;
        while (query.next()) {
            int user_id = query.value(0).toInt();
            QString name = query.value(1).toString();
            QString address = query.value(2).toString();
            QString phone = query.value(3).toString();
            int role = query.value(4).toInt();
            QString check_outs = query.value(5).toString();
            QString login = query.value(6).toString();
            QString password = query.value(7).toString();
            ans.push_back(PatronUser(user_id, name, address, phone, role, login, password, check_outs));
        }
        return ans;
    }

    void add_patron(QString name, QString address, QString phone, int role, QString login, QString password){
        password = Hasher::hash_password(login,password);
        QSqlQuery query;
        query.prepare("INSERT INTO patrons (name, address, phone, role, login, password) VALUES(:name, :address, :phone, :role, :login, :password)");
        query.bindValue(":name", name);
        query.bindValue(":address", address);
        query.bindValue(":phone", phone);
        query.bindValue(":role", role);
        query.bindValue(":login", login);
        query.bindValue(":password", password);
        query.exec();
    }

    bool modify_patron(int user_id, QString name, QString address, QString phone, int role, QString login, QString password){
        password = Hasher::hash_password(login,password);
        QSqlQuery query;
        query.prepare("UPDATE patrons SET name = :name, address = :address, phone = :phone, role = :role, login = :login, password = :password WHERE id = :user_id");
        query.bindValue(":name", name);
        query.bindValue(":address", address);
        query.bindValue(":phone", phone);
        query.bindValue(":role", role);
        query.bindValue(":login", login);
        query.bindValue(":password", password);
        query.bindValue(":user_id", user_id);
        query.exec();
        return 1;//mb change to void
    }

    PatronUser get_patron(int user_id){
        QSqlQuery query;
        query.exec("SELECT * FROM patrons WHERE id = " + QString::number(user_id));
        if (!query.next()) return PatronUser(-1, "", "", "", 0, "", "", "");
        QString name = query.value(1).toString();
        QString address = query.value(2).toString();
        QString phone = query.value(3).toString();
        int role = query.value(4).toInt();
        QString check_outs = query.value(5).toString();
        QString login = query.value(6).toString();
        QString password = query.value(7).toString();
        return PatronUser(user_id, name, address, phone, role, login, password, check_outs);
    }

    bool delete_patron(int user_id){
        QSqlQuery query;
        if (get_patron(user_id).check_outs.size() > 0)
            return 0; //cant delete if has check outs
        query.exec("DELETE FROM patrons WHERE id = " + QString::number(user_id));
        return 1;
    }

    bool add_book(QString title, QString authors, QString publisher, QString keywords, int year, int price, int room, int level, int copies, int bestseller, bool reference){
        QSqlQuery query;
        query.prepare("INSERT INTO books (title, authors, publisher, keywords, year, price, room, level, copies, bestseller, reference) VALUES(:title, :authors, :publisher , :keywords, :year, :price, :room, :level, :copies, :bestseller, :reference)");
        query.bindValue(":title", title);
        query.bindValue(":authors", authors);
        query.bindValue(":publisher", publisher);
        query.bindValue(":keywords", keywords);
        query.bindValue(":year", year);
        query.bindValue(":price", price);
        query.bindValue(":room", room);
        query.bindValue(":level", level);
        query.bindValue(":copies", copies);
        query.bindValue(":bestseller", (bestseller ? 1 : 0));
        query.bindValue(":reference", reference);
        query.exec();
        return 1;
    }
    bool add_article(QString title, QString authors, QString journal, QString publisher, QString keywords, QString editors, int year, int month, int price, int room, int level, int copies, bool reference){
        QSqlQuery query;
        query.prepare("INSERT INTO articles (title, authors, journal, publisher, keywords, editors, year, month, price, room, level, copies, reference) VALUES (:title, :authors, :journal, :publisher, :keywords, :editors, :year, :month, :price, :room, :level, :copies, :reference)");
        query.bindValue(":title", title);
        query.bindValue(":authors", authors);
        query.bindValue(":journal", journal);
        query.bindValue(":publisher", publisher);
        query.bindValue(":keywords", keywords);
        query.bindValue(":editors", editors);
        query.bindValue(":year", year);
        query.bindValue(":month", month);
        query.bindValue(":price", price);
        query.bindValue(":room", room);
        query.bindValue(":level", level);
        query.bindValue(":copies", copies);
        query.bindValue(":reference", reference);
        query.exec();
        return 1;
    }
    bool add_va(QString title, QString authors, QString keywords, int price, int room, int level, int copies, bool reference){
        QSqlQuery query;
        query.prepare("INSERT INTO vas (title, authors, keywords, price, room, level, copies, reference) VALUES(:title, :authors, :keywords, :price, :room, :level, :copies, :reference)");
        query.bindValue(":title", title);
        query.bindValue(":authors", authors);
        query.bindValue(":keywords", keywords);
        query.bindValue(":price", price);
        query.bindValue(":room", room);
        query.bindValue(":level", level);
        query.bindValue(":copies", copies);
        query.bindValue(":reference", reference);
        query.exec();
        return 1;
    }
    bool delete_book(int document_id){
        QSqlQuery query;
        query.exec("DELETE FROM books WHERE id = " + QString::number(document_id));
        return 1;
    }
    bool delete_article(int document_id){
        QSqlQuery query;
        query.exec("DELETE FROM articles WHERE id = " + QString::number(document_id));
        return 1;
    }
    bool delete_va(int document_id){
        QSqlQuery query;
        query.exec("DELETE FROM vas WHERE id = " + QString::number(document_id));
        return 1;
    }

    bool modify_book(int document_id, QString title, QString authors, QString publisher, QString keywords, int year, int price, int room, int level, int copies, bool bestseller, bool reference){
        QSqlQuery query;
        query.prepare("UPDATE books SET title = :title, authors = :authors, publisher = :publisher, keywords = :keywords, year = :year, price = :price, room = :room, level = :level, copies = :copies, bestseller = :bestseller, reference = :reference WHERE id = :document_id");
        query.bindValue(":title", title);
        query.bindValue(":authors", authors);
        query.bindValue(":publisher", publisher);
        query.bindValue(":keywords", keywords);
        query.bindValue(":year", year);
        query.bindValue(":price", price);
        query.bindValue(":room", room);
        query.bindValue(":level", level);
        query.bindValue(":copies", copies);
        query.bindValue(":bestseller", (bestseller ? 1 : 0));
        query.bindValue(":document_id", document_id);
        query.bindValue(":reference", reference);
        query.exec();
        return 1;
    }
    bool modify_article(int document_id, QString title, QString authors, QString journal, QString keywords, QString publisher, QString editors, int year, int month, int price, int room, int level, int copies, bool reference){
        QSqlQuery query;
        query.prepare("UPDATE articles SET title = :title, authors = :authors, journal = :journal, publisher = :publisher, keywords = :keywords, editors = :editors, year = :year, month = :month, price = :price, room = :room, level = :level, copies = :copies, reference = :reference WHERE id = :document_id");
        query.bindValue(":title", title);
        query.bindValue(":authors", authors);
        query.bindValue(":journal", journal);
        query.bindValue(":publisher", publisher);
        query.bindValue(":keywords", keywords);
        query.bindValue(":editors", editors);
        query.bindValue(":year", year);
        query.bindValue(":month", month);
        query.bindValue(":price", price);
        query.bindValue(":room", room);
        query.bindValue(":level", level);
        query.bindValue(":copies", copies);
        query.bindValue(":document_id", document_id);
        query.bindValue(":reference", reference);
        query.exec();
        return 1;
    }
    bool modify_va(int document_id, QString title, QString authors, QString keywords, int price, int room, int level, int copies, bool reference){
        QSqlQuery query;
        query.prepare("UPDATE vas SET title = :title, authors = :authors, keywords = :keywords, price = :price, room = :room, level = :level, copies = :copies, reference = :reference WHERE id = :document_id");
        query.bindValue(":title", title);
        query.bindValue(":authors", authors);
        query.bindValue(":keywords", keywords);
        query.bindValue(":price", price);
        query.bindValue(":room", room);
        query.bindValue(":level", level);
        query.bindValue(":copies", copies);
        query.bindValue(":document_id", document_id);
        query.bindValue(":reference", reference);
        query.exec();
        return 1;
    }

    void outstanding_book(int document_id){
        QSqlQuery query;
        query.exec("SELECT wants FROM books WHERE id = " + document_id);
        query.next();
        QString wants = query.value(0).toString();

        Queue queue;
        queue.from_db(wants);

        while(queue.next_user() != -1){
            int next_id = queue.next_user();
            queue.delete_next();
            notify_patron(next_id, "You was removed from waiting list for a book due to outstanding request");
        }
        query.exec("UPDATE books SET wants = ';' WHERE id = " + QString::number(document_id));
    }

    void outstanding_article(int document_id){
        QSqlQuery query;
        query.exec("SELECT wants FROM articles WHERE id = " + document_id);
        query.next();
        QString wants = query.value(0).toString();

        Queue queue;
        queue.from_db(wants);

        while(queue.next_user() != -1){
            int next_id = queue.next_user();
            queue.delete_next();
            notify_patron(next_id, "You was removed from waiting list for a article due to outstanding request");
        }
        query.exec("UPDATE articles SET wants = ';' WHERE id = " + QString::number(document_id));
    }

    void outstanding_va(int document_id){
        QSqlQuery query;
        query.exec("SELECT wants FROM vas WHERE id = " + document_id);
        query.next();
        QString wants = query.value(0).toString();

        Queue queue;
        queue.from_db(wants);

        while(queue.next_user() != -1){
            int next_id = queue.next_user();
            queue.delete_next();
            notify_patron(next_id, "You was removed from waiting list for a video/audio due to outstanding request");
        }
        query.exec("UPDATE vas SET wants = ';' WHERE id = " + QString::number(document_id));
    }

    void notify_patron(int patron_id, QString message){
        QSqlQuery query;
        query.prepare("SELECT phone from patrons WHERE id = :user_id");
        query.bindValue(":user_id", patron_id);
        query.exec();
        if (!query.next()) return;
        QString phone = query.value(0).toString();

        QString host = "www.twilio.com"; //sms api
        int port = 9502;
        /*
        //need to pay for sms send
        stringstream url;
        url << "/api?action=sendmessage&username=" << encode(username);
        url << "&password=" << encode(password);
        url << "&recipient=" << encode(recipient);
        url << "&messagetype=SMS:TEXT&messagedata=" << encode(message);
        url << "&originator=" << encode(originator);
        url << "&responseformat=xml";
        HINTERNET inet = InternetOpen("Ozeki", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
        HINTERNET conn = InternetConnect(inet, host.c_str() , port, NULL, NULL,
        INTERNET_SERVICE_HTTP, 0, 0);
        HINTERNET sess = HttpOpenRequest(conn, "GET", url.str().c_str(), "HTTP/1.1",
        NULL, NULL, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_RELOAD, 0);
        int error = GetLastError();
        if(error == NO_ERROR){
            HttpSendRequest(sess, NULL, 0, NULL,0);
            int size = 1024;
            char *buffer = new char[size + 1];
            DWORD read;
            int rsize = InternetReadFile(sess, (void *)buffer, size, &read);
            string s = buffer;
            s = s.substr(0, read);
            int pos = s.find("<statuscode>0</statuscode>");
            if(pos > 0) cout << "Message sent." << endl;
            else cout << "Error." << endl;
        }
        */
    }

    LibrarianUser(int id_, QString name_, QString address_, QString phone_, QString login_, QString password_, int privileges_){
        id = id_;
        name = name_;
        address = address_;
        phone = phone_;
        login = login_;
        password = password_;
        privileges = privileges_;
    }
    LibrarianUser(){};
};

class AdminUser : public User{
public:

    void set_profile(QString name, QString address, QString phone, QString login, QString password){
        password = Hasher::hash_password(login, password);
        QSqlQuery query;
        query.prepare("UPDATE admin SET name = :name, address = :address, phone = :phone, login = :login, password = :password WHERE user_id = 1");
        query.bindValue(":name", name);
        query.bindValue(":address", address);
        query.bindValue(":phone", phone);
        query.bindValue(":login", login);
        query.bindValue(":password", password);
        query.exec();
    }

    QVector<LibrarianUser> search_librarians(int user_id, QString name, QString address, QString phone, int privileges, bool or_and){
        QSqlQuery query;
        QString ins = or_and ? " AND " : " OR ";
        name = name.toLower();
        address = address.toLower();
        phone = phone.toLower();
        QString req = "SELECT * FROM librarians WHERE ";
        if (user_id != 0) req += "id = " + QString::number(user_id) + ins;
        if (name != "") req += "instr(lower(name), '"+name+"') > 0" + ins;
        if (address != "") req += "instr(lower(address), '"+address+"') > 0" + ins;
        if (phone != "") req += "instr(lower(phone), '"+phone+"') > 0" + ins;
        if (privileges != 0) req += "privileges = " + QString::number(privileges) + ins;
        req += "1 = " + QString(or_and ? "1" : "0");//nice hack to finish statement correctly
        if (req.length() == 36)//no parameters given
            req = "SELECT * FROM librarians";
        query.exec(req);
        QVector<LibrarianUser> ans;
        while (query.next()) {
            int user_id = query.value(0).toInt();
            QString name = query.value(1).toString();
            QString address = query.value(2).toString();
            QString phone = query.value(3).toString();
            QString login = query.value(4).toString();
            QString password = query.value(5).toString();
            int privileges = query.value(6).toInt();
            ans.push_back(LibrarianUser(user_id, name, address, phone, login, password, privileges));
        }
        return ans;
    }

    void add_librarian(QString name, QString address, QString phone, QString login, QString password, int privileges){
        password = Hasher::hash_password(login,password);
        QSqlQuery query;
        query.prepare("INSERT INTO librarians (name, address, phone, login, password, privileges) VALUES(:name, :address, :phone, :login, :password, :privileges)");
        query.bindValue(":name", name);
        query.bindValue(":address", address);
        query.bindValue(":phone", phone);
        query.bindValue(":login", login);
        query.bindValue(":password", password);
        query.bindValue(":privileges", privileges);
        query.exec();
    }

    bool modify_librarian(int user_id, QString name, QString address, QString phone, QString login, QString password, int privileges){
        password = Hasher::hash_password(login,password);
        QSqlQuery query;
        query.prepare("UPDATE librarians SET name = :name, address = :address, phone = :phone, login = :login, password = :password, privileges = :privileges WHERE id = :user_id");
        query.bindValue(":name", name);
        query.bindValue(":address", address);
        query.bindValue(":phone", phone);
        query.bindValue(":login", login);
        query.bindValue(":password", password);
        query.bindValue(":user_id", user_id);
        query.bindValue(":privileges", privileges);
        query.exec();
        return 1;
    }

    LibrarianUser get_librarian(int user_id){
        QSqlQuery query;
        query.exec("SELECT * FROM librarians WHERE id = " + QString::number(user_id));
        if (!query.next()) return LibrarianUser(-1, "", "", "", "", "", 0);
        QString name = query.value(1).toString();
        QString address = query.value(2).toString();
        QString phone = query.value(3).toString();
        QString login = query.value(4).toString();
        QString password = query.value(5).toString();
        int privileges = query.value(6).toInt();
        return LibrarianUser(user_id, name, address, phone, login, password, privileges);
    }

    bool delete_librarian(int user_id){
        QSqlQuery query;
        query.exec("DELETE FROM librarians WHERE id = " + QString::number(user_id));
        return 1;
    }

    AdminUser(int id_, QString name_, QString address_, QString phone_, QString login_, QString password_){
        id = id_;
        name = name_;
        address = address_;
        phone = phone_;
        login = login_;
        password = password_;
    }
    AdminUser(){};
};


class Login{
public:
    static PatronUser login_patron(QString login, QString password){
        password = Hasher::hash_password(login,password);
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
            int role = query.value(4).toInt();
            QString check_outs = query.value(5).toString();
            QString user_login = query.value(6).toString();
            QString user_password = query.value(7).toString();
            return PatronUser(user_id, name, address, phone, role, user_login, user_password, check_outs);
        }
        return PatronUser(-1,"","","",0,"","","");
    }
    static LibrarianUser login_librarian(QString login, QString password){
        password = Hasher::hash_password(login,password);
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
            int privileges = query.value(6).toInt();
            return LibrarianUser(user_id, name, address, phone, user_login, user_password, privileges);
        }
        return LibrarianUser(-1,"","","","","", 0);
    }
    static AdminUser login_admin(QString login, QString password){
        password = Hasher::hash_password(login, password);
        QSqlQuery query;
        query.prepare("SELECT * from admin WHERE login = :login and password = :password");
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
            return AdminUser(user_id, name, address, phone, user_login, user_password);
        }
        return AdminUser(-1,"","","","","");
    }

    static void clear_database(){
        QSqlQuery query;
        query.exec("DELETE FROM librarians");
        query.exec("DELETE FROM patrons");
        query.exec("DELETE FROM books");
        query.exec("DELETE FROM articles");
        query.exec("DELETE FROM vas");
        query.exec("DELETE FROM check_outs");
        AdminUser admin = Login::login_admin("sidr", "123");
        //admin.add_librarian("Сидорович", "Бункер на Кордоне", "88005553535", "sidr", "123");
        admin.add_librarian("libr", "inno", "234", "sidr", "123", 1);


    }

    static void make_database(){
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
                     "reference INTEGER DEFAULT 0,"
                     "wants VARCHAR(255) DEFAULT ';');");

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
                     "copies INTEGER,"
                     "reference INTEGER DEFAULT 0,"
                     "wants VARCHAR(255) DEFAULT ';');");

        query.exec("CREATE TABLE IF NOT EXISTS vas ("
                     "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                     "title VARCHAR(255), "
                     "authors VARCHAR(255), "
                     "keywords VARCHAR(255), "
                     "price INTEGER, "
                     "room INTEGER, "
                     "level INTEGER, "
                     "copies INTEGER,"
                     "reference INTEGER DEFAULT 0,"
                     "wants VARCHAR(255) DEFAULT ';');");

        query.exec("CREATE TABLE IF NOT EXISTS patrons ("
                     "id INTEGER PRIMARY KEY, "
                     "name VARCHAR(255), "
                     "address VARCHAR(255), "
                     "phone VARCHAR(255), "
                     "role INTEGER, "
                     "check_outs VARCHAR(255) DEFAULT ';', "
                     "login VARCHAR(255), "
                     "password VARCHAR(255));");

        query.exec("CREATE TABLE IF NOT EXISTS librarians ("
                     "id INTEGER PRIMARY KEY, "
                     "name VARCHAR(255), "
                     "address VARCHAR(255), "
                     "phone VARCHAR(255), "
                     "login VARCHAR(255), "
                     "password VARCHAR(255), "
                     "privileges INTEGER);");

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
                     "day_end INTEGER, "
                     "renew_state INTEGER DEFAULT 0);");

        query.exec("CREATE TABLE IF NOT EXISTS settings ("
                     "days_add_renew INTEGER DEFAULT 7)");

        query.exec("CREATE TABLE IF NOT EXISTS admin ("
                     "user_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                     "name VARCHAR(255), "
                     "address VARCHAR(255), "
                     "phone VARCHAR(255), "
                     "login VARCHAR(255), "
                     "password VARCHAR(255));");

        query.prepare("INSERT INTO librarians (name, address, phone, login, password) VALUES('admin', '', '', 'admin', 'admin')");
        query.exec();
    }
};

static void append_log(QString data){
    QDateTime cur = QDateTime::currentDateTime();
    QFile writeFile("log.txt");
    if(!writeFile.open(QFile::Append | QFile::Text)) return;
    QTextStream in(&writeFile);
    in << cur.toString() << ": " << data << "\r";
    writeFile.close();
}
