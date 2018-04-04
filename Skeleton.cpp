#pragma once
#include <QString>
#include <QVector>
#include <QtSql>
#include <unistd.h>
#include <hasher.h>


using namespace std;

#define BOOK       1
#define ARTICLE    2
#define AV         3

#define PATRON_STUDENT             1
#define PATRON_INSTRUCTOR          2
#define PATRON_TA                  3
#define PATRON_PROFESSOR           4
#define PATRON_VISITINGPROFESSOR   5

const QString document_names[4] = {
        QString(""), QString("books"), QString("articles"), QString("vas")
};


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

class Queue;
class Reserve;

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
    std::pair<QDate, int> calculate_check_out(int document_type, int year_start, int month_start, int day_start, int patron_type, bool bestseller, int price, int renew_state){
        QDate date_start;
        date_start.setDate(year_start, month_start, day_start);
        QDate date_end;
        int days_to_add = 0;
        switch(document_type) {
        case BOOK:
            if(patron_type == PATRON_VISITINGPROFESSOR)
                days_to_add = 7;
            else if(patron_type == PATRON_PROFESSOR || patron_type == PATRON_INSTRUCTOR || patron_type == PATRON_TA)
                days_to_add = 28;
            else if (bestseller)
                days_to_add = 14;
            else
                days_to_add = 21;
            break;
        case ARTICLE: case AV:
            if(patron_type == PATRON_VISITINGPROFESSOR)
                days_to_add = 7;
            else
                days_to_add = 14;
            break;
        }
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
    int patron_type;
    QVector<int> check_outs;

    void check_out_book(int document_id, QDate *gdate = NULL){
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

        QDate date;
        if(gdate == NULL)
            date = QDate::currentDate();
        else
            date = *gdate;
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
    void check_out_article(int document_id, QDate *gdate = NULL) {
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


        QDate date;
        if(gdate == NULL)
            date = QDate::currentDate();
        else
            date = *gdate;
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
    void check_out_av(int document_id, QDate *gdate = NULL){
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


        QDate date;
        if(gdate == NULL)
            date = QDate::currentDate();
        else
            date = *gdate;
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

    void take_reserved(int document_id, int document_type) {
        Reserve r(document_id, document_type, this->id);
        r.deleteFromDB();

        QDate date = QDate::currentDate();
        int year_start = date.year();
        int month_start = date.month();
        int day_start = date.day();

        QSqlQuery query;
        switch(document_type) {
        case BOOK:
            query.prepare("INSERT INTO check_outs (user_id,document_type,document_id,year_start,month_start,day_start) VALUES(:user_id,:document_type,:document_id,:year_start,:month_start,:day_start)");
            query.bindValue(":user_id", id);
            query.bindValue(":document_type", BOOK);
            query.bindValue(":document_id", document_id);
            query.bindValue(":year_start", year_start);
            query.bindValue(":month_start", month_start);
            query.bindValue(":day_start", day_start);
            query.exec();
            break;
        case ARTICLE:
            query.prepare("INSERT INTO check_outs (user_id,document_type,document_id,year_start,month_start,day_start) VALUES(:user_id,:document_type,:document_id,:year_start,:month_start,:day_start)");
            query.bindValue(":user_id", id);
            query.bindValue(":document_type", ARTICLE);
            query.bindValue(":document_id", document_id);
            query.bindValue(":year_start", year_start);
            query.bindValue(":month_start", month_start);
            query.bindValue(":day_start", day_start);
            query.exec();
            break;
        case AV:
            query.prepare("INSERT INTO check_outs (user_id,document_type,document_id,year_start,month_start,day_start) VALUES(:user_id,:document_type,:document_id,:year_start,:month_start,:day_start)");
            query.bindValue(":user_id", id);
            query.bindValue(":document_type", AV);
            query.bindValue(":document_id", document_id);
            query.bindValue(":year_start", year_start);
            query.bindValue(":month_start", month_start);
            query.bindValue(":day_start", day_start);
            query.exec();
            break;
        }

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

    //return -1 if user already in base
    //return -2 if for this document set outstanding request
    //otherwise return position in queue
    int want_document(int document_id, int document_type) {
        QSqlQuery query;
        query.prepare("UPDATE check_outs SET renew_state = 1 WHERE document_type = :document_type AND renew_state = 0 AND year_end IS NULL AND document_id = :document_id");
        query.bindValue(":document_id", document_id);
        query.bindValue(":document_type", document_type);
        query.exec();

        if (Queue::existInDB(document_id, document_type)) {
            Queue q = Queue::getFromDB(document_id, document_type);
            if(q.outstanding_request) return -2;
            else if(q.hasPatron(this)) return -1;
            else {
                int pos = q.addPatron(this);
                q.saveInDB();
                return pos;
            }
        } else {
            Queue q(document_id, document_type, this);
            q.saveInDB();
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

        if (renew_state != 0) return 1; //someone wants

        Book book = get_book(book_id);
        std::pair<QDate, int> end = calculate_check_out(1, year_start, month_start, day_start, patron_type, book.bestseller, book.price, renew_state);

        if (today.daysTo(end.first) != 0 && today.daysTo(end.first) != 1)
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
        std::pair<QDate, int> end = calculate_check_out(2, year_start, month_start, day_start, patron_type, 0, article.price, renew_state);

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
        std::pair<QDate, int> end = calculate_check_out(3, year_start, month_start, day_start, patron_type, 0, va.price, renew_state);

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
            std::pair<QDate, int> end = calculate_check_out(1, year_start, month_start, day_start, patron_type, book.bestseller, book.price, state_renew);
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
            std::pair<QDate, int> end = calculate_check_out(2, year_start, month_start, day_start, patron_type, 0, article.price, state_renew);
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
            std::pair<QDate, int> end = calculate_check_out(3, year_start, month_start, day_start, patron_type, 0, va.price, state_renew);
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

    PatronUser(int id_, QString name_, QString address_ , QString phone_, int patron_type_, QString login_, QString password_, QString check_outs_){
        id = id_;
        name = name_;
        address = address_;
        phone = phone_;
        patron_type = patron_type_;
        login = login_;
        password = password_;
        add_check_outs(check_outs_);
    }
    PatronUser(){};
};

//search/add/delete/modify users, search/add/delete/modify documents, search overdue documents
class LibrarianUser : public User{
public:
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
                std::pair<QDate, int> end = calculate_check_out(1, year_start, month_start, day_start, patron.patron_type, book.bestseller, book.price, state_renew);
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
                std::pair<QDate, int> end = calculate_check_out(2, year_start, month_start, day_start, patron.patron_type, 0, article.price, state_renew);
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
                std::pair<QDate, int> end = calculate_check_out(3, year_start, month_start, day_start, patron.patron_type, 0, va.price, state_renew);
                QDate return_date;
                return_date.setDate(end.first.year(), end.first.month(), end.first.day());
                if (overdue && QDate::currentDate().daysTo(return_date) >= 0) continue;
                ans.push_back(make_pair(Check_out(current_user_id, 3, va_id, check_out_id, year_start, month_start, day_start, end.first.year(), end.first.month(), end.first.day(), end.second), va));
            }
        }
        return ans;
    }

    int remove_last_wants_book(int document_id){
        Book book = get_book(document_id);
        if (book.wants.size() == 0)
            return -1;
        int last = book.wants[book.wants.size()-1];
        book.wants.pop_back();
        QString new_wants = ";";
        for (int i = 0; i < book.wants.size(); i++)
            new_wants += QString::number(book.wants[i]) + ";";
        QSqlQuery query;
        query.prepare("UPDATE books SET wants = :wants WHERE id = :document_id");
        query.bindValue(":wants", new_wants);
        query.bindValue(":document_id", document_id);
        query.exec();
        return last;
    }
    int remove_last_wants_article(int document_id){
        Article article = get_article(document_id);
        if (article.wants.size() == 0)
            return -1;
        int last = article.wants[article.wants.size()-1];
        article.wants.pop_back();
        QString new_wants = ";";
        for (int i = 0; i < article.wants.size(); i++)
            new_wants += QString::number(article.wants[i]) + ";";
        QSqlQuery query;
        query.prepare("UPDATE articles SET wants = :wants WHERE id = :document_id");
        query.bindValue(":wants", new_wants);
        query.bindValue(":document_id", document_id);
        query.exec();
        return last;
    }
    int remove_last_wants_va(int document_id){
        VA va = get_va(document_id);
        if (va.wants.size() == 0)
            return -1;
        int last = va.wants[va.wants.size()-1];
        va.wants.pop_back();
        QString new_wants = ";";
        for (int i = 0; i < va.wants.size(); i++)
            new_wants += QString::number(va.wants[i]) + ";";
        QSqlQuery query;
        query.prepare("UPDATE vas SET wants = :wants WHERE id = :document_id");
        query.bindValue(":wants", new_wants);
        query.bindValue(":document_id", document_id);
        query.exec();
        return last;
    }

    void make_outstanding(int document_id, int document_type) {
        if(Queue::existInDB(document_id, document_type)) {
            Queue q = Queue::getFromDB(document_id, document_type);
            q.deleteFromDB();
        } else {
            QSqlQuery query;
            query.prepare("UPDATE check_outs SET renew_state = 1 WHERE document_id = :document_id AND document_type = :document_type AND renew_state = 0");
            query.bindValue(":document_id", document_id);
            query.bindValue(":document_type", document_type);
            query.exec();
        }
    }

    void remove_outstanding(int document_id, int document_type) {
        QSqlQuery query;
        query.prepare("UPDATE check_outs SET renew = 0 WHERE document_id = :document_id AND document_type = :document_type AND renew_state = 1");
        query.bindValue(":document_id", document_id);
        query.bindValue(":document_type", document_type);
        query.exec();
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
        std::pair<QDate, int> end = calculate_check_out(1, year_start, month_start, day_start, patron.patron_type, book.bestseller, book.price, renew_state);
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

        remove_outstanding(book_id, BOOK);

        int wants_id;
        if (Queue::existInDB(book_id, BOOK)) {
            Queue q = Queue::getFromDB(book_id, BOOK);
            wants_id = q.nextPatron();
            Reserve r(book_id, BOOK, wants_id);
            r.saveInDB();
        } else {
            wants_id = -1;
            query.prepare("UPDATE books SET copies = copies + 1 WHERE id = :document_id");
            query.bindValue(":document_id", book.id);
            query.exec();
        }

        query.prepare("DELETE FROM check_outs WHERE check_out_id = :check_out_id");
        query.bindValue(":check_out_id", check_out_id);
        query.exec();

        return make_pair(fine, wants_id);
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
        std::pair<QDate, int> end = calculate_check_out(2, year_start, month_start, day_start, patron.patron_type, 0, article.price, renew_state);
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

        remove_outstanding(article_id, ARTICLE);

        int wants_id;
        if (Queue::existInDB(book_id, ARTICLE)) {
            Queue q = Queue::getFromDB(book_id, ARTICLE);
            wants_id = q.nextPatron();
            Reserve r(book_id, ARTICLE, wants_id);
            r.saveInDB();
        } else {
            wants_id = -1;
            query.prepare("UPDATE articles SET copies = copies + 1 WHERE id = :document_id");
            query.bindValue(":document_id", article.id);
            query.exec();
        }

        query.prepare("DELETE FROM check_outs WHERE check_out_id = :check_out_id");
        query.bindValue(":check_out_id", check_out_id);
        query.exec();

        return make_pair(fine, wants_id);
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
        std::pair<QDate, int> end = calculate_check_out(3, year_start, month_start, day_start, patron.patron_type, 0, va.price, renew_state);
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

        remove_outstanding(va_id, AV);

        int wants_id;
        if (Queue::existInDB(va_id, AV)) {
            Queue q = Queue::getFromDB(va_id, AV);
            wants_id = q.nextPatron();
            Reserve r(va_id, AV, wants_id);
            r.saveInDB();
        } else {
            wants_id = -1;
            query.prepare("UPDATE vas SET copies = copies + 1 WHERE id = :document_id");
            query.bindValue(":document_id", va.id);
            query.exec();
        }

        query.prepare("DELETE FROM check_outs WHERE check_out_id = :check_out_id");
        query.bindValue(":check_out_id", check_out_id);
        query.exec();

        return make_pair(fine, wants_id);
    }

    QVector<PatronUser> search_patrons(int user_id, QString name, QString address, QString phone, int patron_type, bool or_and){
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
        if (patron_type) req += "faculty = " + patron_type + ins;
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
            bool faculty = query.value(4).toInt();
            QString check_outs = query.value(5).toString();
            QString login = query.value(6).toString();
            QString password = query.value(7).toString();
            ans.push_back(PatronUser(user_id, name, address, phone, faculty, login, password, check_outs));
        }
        return ans;
    }

    QVector<LibrarianUser> search_librarians(int user_id, QString name, QString address, QString phone, bool or_and){
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
            ans.push_back(LibrarianUser(user_id, name, address, phone, login, password));
        }
        return ans;
    }

    void add_patron(QString name, QString address, QString phone, int patron_type, QString login, QString password){
        password = Hasher::hash_password(login,password);
        QSqlQuery query;
        query.prepare("INSERT INTO patrons (name, address, phone, faculty, login, password) VALUES(:name, :address, :phone, :faculty, :login, :password)");
        query.bindValue(":name", name);
        query.bindValue(":address", address);
        query.bindValue(":phone", phone);
        query.bindValue(":faculty", patron_type);
        query.bindValue(":login", login);
        query.bindValue(":password", password);
        query.exec();
    }
    void add_librarian(QString name, QString address, QString phone, QString login, QString password){
        password = Hasher::hash_password(login,password);
        QSqlQuery query;
        query.prepare("INSERT INTO librarians (name, address, phone, login, password) VALUES(:name, :address, :phone, :login, :password)");
        query.bindValue(":name", name);
        query.bindValue(":address", address);
        query.bindValue(":phone", phone);
        query.bindValue(":login", login);
        query.bindValue(":password", password);
        query.exec();
    }

    bool modify_patron(int user_id, QString name, QString address, QString phone, int patron_type, QString login, QString password){
        password = Hasher::hash_password(login,password);
        QSqlQuery query;
        query.prepare("UPDATE patrons SET name = :name, address = :address, phone = :phone, faculty = :faculty, login = :login, password = :password WHERE id = :user_id");
        query.bindValue(":name", name);
        query.bindValue(":address", address);
        query.bindValue(":phone", phone);
        query.bindValue(":faculty", patron_type);
        query.bindValue(":login", login);
        query.bindValue(":password", password);
        query.bindValue(":user_id", user_id);
        query.exec();
        return 1;//mb change to void
    }

    bool modify_librarian(int user_id, QString name, QString address, QString phone, QString login, QString password){
        password = Hasher::hash_password(login,password);
        QSqlQuery query;
        query.prepare("UPDATE librarians SET name = :name, address = :address, phone = :phone, login = :login, password = :password WHERE id = :user_id");
        query.bindValue(":name", name);
        query.bindValue(":address", address);
        query.bindValue(":phone", phone);
        query.bindValue(":login", login);
        query.bindValue(":password", password);
        query.bindValue(":user_id", user_id);
        query.exec();
        return 1;
    }

    PatronUser get_patron(int user_id){
        QSqlQuery query;
        query.exec("SELECT * FROM patrons WHERE id = " + QString::number(user_id));
        if (!query.next()) return PatronUser(-1, "", "", "", 0, "", "", "");
        QString name = query.value(1).toString();
        QString address = query.value(2).toString();
        QString phone = query.value(3).toString();
        int patron_type = query.value(4).toInt();
        QString check_outs = query.value(5).toString();
        QString login = query.value(6).toString();
        QString password = query.value(7).toString();
        return PatronUser(user_id, name, address, phone, patron_type, login, password, check_outs);
    }

    LibrarianUser get_librarian(int user_id){
        QSqlQuery query;
        query.exec("SELECT * FROM librarians WHERE id = " + QString::number(user_id));
        if (!query.next()) return LibrarianUser(-1, "", "", "", "", "");
        QString name = query.value(1).toString();
        QString address = query.value(2).toString();
        QString phone = query.value(3).toString();
        QString login = query.value(4).toString();
        QString password = query.value(5).toString();
        return LibrarianUser(user_id, name, address, phone, login, password);
    }



    bool delete_patron(int user_id){
        QSqlQuery query;
        if (get_patron(user_id).check_outs.size() > 0)
            return 0; //cant delete if has check outs
        query.exec("DELETE FROM patrons WHERE id = " + QString::number(user_id));
        return 1;
    }

    bool delete_librarian(int user_id){
        QSqlQuery query;
        query.exec("DELETE FROM librarians WHERE id = " + QString::number(user_id));
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

    void notify(int user_id, QString message){
        PatronUser p = get_patron(user_id);
        QString phone = p.phone;
        //SMS_Sender.send(phone, message);
        qDebug() << "message: \"" + message + "\" sent to user " + QString(user_id);
    }

    LibrarianUser(int id_, QString name_, QString address_, QString phone_, QString login_, QString password_){
        id = id_;
        name = name_;
        address = address_;
        phone = phone_;
        login = login_;
        password = password_;
    }
    LibrarianUser(){};
};

class Queue {
public:
    int document_id;
    int document_type;
    bool outstanding_request;
    QVector<int> request_ids;
    QVector<int> patron_posses;

    static QVector<int> stringToVector(QString s) {
        QVector<int> v;
        QStringList list = s.split(" ");
        for (int i = 0; i < list.size(); ++i) {
            if(list[i].isEmpty()) continue;
            v.append(list[i].toInt());
        }
        return v;
    }

    static QString vectorToString(QVector<int> v) {
        QString s = "";
        for (int i = 0; i < v.size(); ++i) {
            s += v[i] + " ";
        }
        return s;
    }

public:
    Queue(int document_id, int document_type, bool outstanding_request, QString request_ids, QString patron_posses) {
        this->document_id = document_id;
        this->document_type = document_type;
        this->outstanding_request = outstanding_request;
        this->request_ids = stringToVector(request_ids);
        this->patron_posses = stringToVector(patron_posses);
        this->patron_posses.push_front(0);
    }

    Queue(int document_id, int document_type, PatronUser patron) {
        this->document_id = document_id;
        this->document_type = document_type;
        this->outstanding_request = false;
        this->request_ids = QVector<int>();
        this->patron_posses = QVector<int>();
        for (int i = 0; i < 5; ++i)
            this->patron_posses.append(0);
        addPatron(patron);
    }

    static Queue getFromDB(int document_id, int document_type) {
        QSqlQuery query;
        query.prepare("SELECT * FROM Queue WHERE document_id = :document_id AND document_type = :document_type");
        query.bindValue(":document_id", document_id);
        query.bindValue(":document_type", document_type);
        query.exec();
        query.next();
        return Queue(query.value(0).toInt(), query.value(0).toInt(), query.value(1).toBool(), query.value(3).toString(), query.value(4).toString());
    }

    static bool existInDB(int document_id, int document_type) {
        QSqlQuery query;
        query.prepare("SELECT * FROM Queue WHERE document_id = :document_id AND document_type = :document_type");
        query.bindValue(":document_id", document_id);
        query.bindValue(":document_type", document_type);
        query.exec();
        return query.next();
    }

    void saveInDB() {
        QSqlQuery query;

        query.prepare("DELETE FROM Queue WHERE document_id = :document_id AND document_type = :document_type");
        query.bindValue(":document_id", this->document_id);
        query.bindValue(":document_type", this->document_type);
        query.exec();

        QString ids = vectorToString(this->request_ids);
        QString patron_posses = vectorToString(this->patron_posses);

        query.prepare("INSERT INTO Queue VALUES (:document_id, :document_type, :outstanding_request, :request_ids, :requests, :patron_posses");
        query.bindValue(":document_id", this->document_id);
        query.bindValue(":document_type", this->document_type);
        query.bindValue(":outstanding_request", this->outstanding_request);
        query.bindValue(":request_ids", ids);
        query.bindValue(":requests", this->request_ids.size());
        query.bindValue(":patron_posses", patron_posses);
        query.exec();
    }

    void deleteFromDB() {
        QSqlQuery query;
        query.prepare("DELETE FROM Queue WHERE document_id = :document_id AND document_type = :document_type");
        query.bindValue(":document_id", this->document_id);
        query.bindValue(":document_type", this->document_type);
        query.exec();
    }

    //return position in queue or -1 if patron already in queue
    int addPatron(PatronUser patron) {
        int start = this->patron_posses[patron.patron_type];
        int end = this->patron_posses[patron.patron_type + 1];

        for (int i = start; i < end; ++i)
            if(this->request_ids[i] == patron.id)
                return -1;

        request_ids.insert(end, patron.id);
        for (int i = patron.patron_type; i < patron_posses.size(); ++i)
            ++patron_posses[i];
        return patron_posses[patron.patron_type];
    }

    bool hasPatron(PatronUser patron) {
        int start = this->patron_posses[patron.patron_type];
        int end = this->patron_posses[patron.patron_type + 1];

        for (int i = start; i < end; ++i)
            if (request_ids[i] == patron.id) return true;
        return false;
    }

    int nextPatron() {
        int result = request_ids[0];
        request_ids.removeFirst();
        if(request_ids.empty())
            deleteFromDB();
        return result;
    }
};

class Reserve {
    int document_id;
    int document_type;
    int user_id;
    QDate endDate;

    Reserve(int document_id, int document_type, int user_id) {
        this->document_id = document_id;
        this->user_id = user_id;
        this->document_type = document_type;
        endDate = QDate::currentDate();
        endDate.addDays(1);
    }

    Reserve(int document_id, int document_type, int user_id, QString endDate) {
        this->document_id = document_id;
        this->user_id = user_id;
        this->document_type = document_type;
        QStringList values = endDate.split(" ");
        this->endDate = QDate(values[0].toInt(), values[1].toInt(), values[2].toInt());
    }

    static bool existInDB(int document_id, int document_type, int user_id) {
        QSqlQuery query;
        query.prepare("SELECT * FROM Reserves WHERE document_id = :document_id AND document_type = :document_type");
        query.bindValue(":document_id", document_id);
        query.bindValue(":document_type", document_type);
        query.exec();
        QDate now = QDate::currentDate();
        while(query.next()) {
            Reserve r (query.value(0).toInt(), query.value(1).toInt(), query.value(2).toInt(), query.value(3).toString());
            if(now.day() > r.endDate.day())
                r.deleteFromDB();
                if (r.user_id == user_id) return false;
            if(r.user_id == user_id) return true;
        }
        return false;
    }

    static Reserve getFromDB(int document_id, int document_type, int user_id) {
        QSqlQuery query;
        query.prepare("SELECT * FROM Reserves WHERE document_id = :document_id AND document_type = :document_type AND user_id = :user_id");
        query.bindValue(":document_id", document_id);
        query.bindValue(":document_type", document_type);
        query.bindValue(":user_id", user_id);
        query.exec();
        query.next();
        return Reserve(query.value(0).toInt(), query.value(1).toInt(), query.value(2).toInt(), query.value(3).toString());
    }

    bool saveInDB() {
        QSqlQuery query;
        query.prepare("DELETE FROM Reserves WHERE document_id = :document_id AND document_type = :document_type AND user_id = :user_id");
        query.bindValue(":document_id", document_id);
        query.bindValue(":document_type", document_type);
        query.bindValue(":user_id", user_id);
        query.exec();

        QString date = QString(endDate.year()) + " " + QString(endDate.month()) + " " + QString(endDate.day());
        query.prepare("INSERT INTO Reserve VALUES (:document_id, :user_id, :document_type, :endDate");
        query.bindValue(":document_id", document_id);
        query.bindValue(":user_ud", user_id);
        query.bindValue(":document_type", document_type);
        query.bindValue(":endDate", date);
        query.exec();
        return true;
    }

    void deleteFromDB() {
        QSqlQuery query;

        query.prepare("DELETE FROM Reserves WHERE document_id = :document_id AND document_type = :document_type AND user_id = :user_id");
        query.bindValue(":document_id", document_id);
        query.bindValue(":document_type", document_type);
        query.bindValue(":user_id", user_id);
        query.exec();

        query.prepare("UPDATE :documents SET copies = copies + 1 WHERE document_id = :document_id");
        query.bindValue(":document_id", document_id);
        query.bindValue(":documents", document_names[document_type]);
        query.exec();
    }
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
            int faculty = query.value(4).toInt();
            QString check_outs = query.value(5).toString();
            QString user_login = query.value(6).toString();
            QString user_password = query.value(7).toString();
            return PatronUser(user_id, name, address, phone, faculty, user_login, user_password, check_outs);
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
            return LibrarianUser(user_id, name, address, phone, user_login, user_password);
        }
        return LibrarianUser(-1,"","","","","");
    }

    static void clear_database(){
        QSqlQuery query;
        query.exec("DELETE FROM librarians");
        query.exec("DELETE FROM patrons");
        query.exec("DELETE FROM books");
        query.exec("DELETE FROM articles");
        query.exec("DELETE FROM vas");
        query.exec("DELETE FROM check_outs");
        LibrarianUser libr = Login::login_librarian("sidr", "123");
        libr.add_librarian("Сидорович", "Бункер на Кордоне", "88005553535", "sidr", "123");
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
                     "day_end INTEGER, "
                     "renew_state INTEGER DEFAULT 0,"
                     "return_state INTEGER DEFAULT 0)");

        query.exec("CREATE TABLE IF NOT EXISTS settings ("
                     "days_add_renew INTEGER DEFAULT 7)");
        //TODO: create basic librarian
    }
};
