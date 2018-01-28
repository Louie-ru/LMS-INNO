#include <QString>
#include <QVector>
#include <time.h>
#include <pair>
#include <string>


class Document{
public:
    QString title, keywords;
    int copies, id, price, room, level;
};

//defined type constants
#define BOOK       1
#define ARTICLE    2
#define AV         3

#define PATRON     11
#define LIBRARIAN  12

class Book : public Document{
public:
    QString publisher, authors;
    int year;
    bool bestseller;
    Book(QString authors_, QString title_, QString keywords_, QString publisher_, int id_, int year_, int copies_, int price_, int room_, int level_, bool bestseller_){
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
    }
    Book(){}
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
    Article(){}
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
    VA(){}
};

class Check_out {
public:
    int document_type;//1-book 2-article 3-av
    int document_id;//id in Books/Articles/AVs table
    int check_out_id;//id in Check_outs table
    int year_start, month_start, day_start;//taken date
    int year_end, month_end, day_end;//need to return date
    int fine, user_id;
};


class User{
public:
    QString name, address, phone, login;
	string password;
    int id;//id in Patrons/Librarians table

    QVector<Book>* search_books(QString *authors, QString *title, QString *keywords, QString *publisher, int *year, bool bestseller, bool available, bool or_and);
    QVector<Article>* search_articles(QString *authors, QString *title, QString *keywords, QString *journal_title, QString *publisher, QString *editors, int *year, int *month, bool available, bool or_and);
    QVector<VA>* search_av(QString *authors, QString *title, QString *keywords, bool available, bool or_and);

	User(QString &name, QString &address, QString &phone, QString &login, string &password, int id);
};

//can search/check_out documents, renew/return check_outed documents
class PatronUser : public User{
private:
	tm getDate();
	void add_weeks(const tm&, int weeks);
	int calculate_fine(Check_out *ch, int price);
public:
    bool faculty;//is faculty member

    //return number of weeks for check_out or -1 for error
    int check_out_book(int id);
    int check_out_article(int id);
    int check_out_av(int id);

    //return QVector of my documents
    QVector<std::pair<Check_out, Book>>* get_checked_out_books();
    QVector<std::pair<Check_out, Article>>* get_checked_out_articles();
    QVector<std::pair<Check_out, VA>>* get_checked_out_avs();

    //return fine size or 0
    int return_book(int id);
    int return_article(int id);
    int return_av(int id);

    PatronUser(){}
	PatronUser(QString name, QString address, QString phone, int id, bool faculty, QString login, string password);
};

//can search/add/delete/modify users, search/add/delete/modify documents, search overdue documents
class LibrarianUser : public User{
private:
	string hashPassword(const string &password, int id);
public:
    //search in Check_outs table
    QVector<pair<Check_out, Book>>* search_books_checked_out(int *user_id, QString *authors, QString *title, QString *keywords, QString *publisher, int *year, bool bestseller, bool or_and);
    QVector<pair<Check_out, Article>>* search_articles_checked_out(int *user_id, QString *authors, QString *title, QString *keywords, QString *journal_title, QString *publisher, QString *editors, int *year, int *month, bool or_and);
    QVector<pair<Check_out, VA>>* search_av_checked_out(int *user_id, QString *authors, QString *title, QString *keywords, bool or_and);

    //return TRUE if success, FALSE if error
    QVector<PatronUser>* search_patrons(QString *name, QString *address, QString *phone, bool *faculty, bool has_overdues, bool or_and);
    bool add_patron(QString name, QString address, QString phone, int id, bool faculty, QString login, string password);
    bool delete_patron(int user_id);
    bool modify_patron(int user_id, QString name, QString address, QString phone, bool faculty, QString login, string password);

    QVector<LibrarianUser> search_librarians(QString name, QString address, QString phone, bool or_and);
    bool add_librarian(QString name, QString address, QString phone);
    bool delete_librarian(int user_id);
    bool modify_librarian(int user_id, QString name, QString address, QString phone);

    bool add_book_new(QString authors, QString title, QString keywords, QString publisher, QString editors, int year, bool bestseller, int copies);
    bool add_article_new(QString authors, QString title, QString keywords, QString journal_title, QString publisher, QString editors, int year, int month, int copies);
    bool add_av_new(QString authors, QString title, QString keywords, int available, int copies);

    bool delete_book(int id, int copies);
    bool delete_article(int id, int copies);
    bool delete_av(int id, int copies);

    bool modify_book(int id, QString authors, QString title, QString keywords, QString publisher, int year, bool bestseller, int copies);
    bool modify_article(int id, QString authors, QString title, QString keywords, QString journal_title, QString publisher, QString editors, int year, int month, int copies);
    bool modify_av(int id, QString authors, QString title, QString keywords, bool available, int copies);

    LibrarianUser(){}
};

namespace db {
    
    // false if there are exist equal document
    bool add_book(Book*);
    bool add_article(Article*);
    bool add_av(VA*);
    bool add_checkout(Document*, int type, tm&begin, tm&end);
    
    // false if there nothing to modify
    bool modify_book(Book*);
    bool modify_article(Article*);
    bool modify_av(VA*);
    
    // false if there nothing to delete
    bool delete_book(int id);
    bool delete_article(int id);
    bool delete_av(int id);
    bool delete_checkout(int id);
    
    // return null if there are no appropriate document
    Book* get_book(int id);
    Article* get_article(int id);
    VA* get_AV(int id);
    
    //return empty vector if there are no appropriate documents
    QVector<Book>* search_books(QString *authors, QString *title, QString *keywords, QString *publisher, int *year, bool bestseller, bool available, bool or_and);
    QVector<Article>* search_articles(QString *authors, QString *title, QString *keywords, QString *journal_title, QString *publisher, QString *editors, int *year, int *month, bool available, bool or_and);
    QVector<VA>* search_av(QString *authors, QString *title, QString *keywords, bool available, bool or_and);
    
    //return empty vector if there are no appropriate documents
    QVector<pair<Check_out, Book>>* search_books_checked_out(int *user_id, QString *authors, QString *title, QString *keywords, QString *publisher, int *year, bool bestseller, bool or_and);
    QVector<pair<Check_out, Article>>* search_articles_checked_out(int *user_id, QString *authors, QString *title, QString *keywords, QString *journal_title, QString *publisher, QString *editors, int *year, int *month, bool or_and);
    QVector<pair<Check_out, VA>>* search_av_checked_out(int *user_id, QString *authors, QString *title, QString *keywords, bool or_and);
    
    // false if there are exist user with such id
    bool add_patron(PatronUser*);
    bool add_librarian(LibrarianUser*);
    
    // false if there are no user with such id or types is not coincides
    bool modify_patron(PatronUser*);
    bool modify_librarian(LibrarianUser*);
    
    // false if there are no user with such id or types is not coincides
    bool delete_patron(int id);
    bool delete_librarian(int id);
    
    // return null if there are no user with such id or types is not concides
    PatronUser* get_patron(int id);
    LibrarianUser* get_librarian(int id);

	QVector<PatronUser>* search_patrons(QString *name, QString *address, QString *phone, bool *faculty, bool has_overdues, bool or_and);
    
    // return null if password don't coincides
    pair<User,int>* get_user(string login);
}

//return 0-error 1-patron 2-librarian
pair<User,int>* login(QString login, QString password);
