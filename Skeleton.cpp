#include <QString>
#include <QVector>

namespace sk {
    class Document{
    public:
        QString title, keywords;
        int copies, id, price, room, level;
    };

    //defined type constants
    #define BOOK 1
    #define ARTICLE 2
    #define AV 3

    class Book : Document{
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
    };

    class Article : Document{
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
    };

    class VA : Document{
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
    };

    class Check_out {
        int document_type;//1-book 2-article 3-av
        int document_id;//id in Books/Articles/AVs table
        int check_out_id;//id in Check_outs table
        int year_start, month_start, day_start;//taken date
        int year_end, month_end, day_end;//need to return date
    };


    class User{
        QString name, address, phone;
        int id;//id in Patrons/Librarians table

        QVector<Book> search_books(QString authors, QString title, QString keywords, QString publisher, int year, int bestseller, int available, bool or_and);
        QVector<Article> search_articles(QString authors, QString title, QString keywords, QString journal_title, QString publisher, QString editors, int year, int month, int available, bool or_and);
        QVector<VA> search_av(QString authors, QString title, QString keywords, int available, bool or_and);
    };

    //can search/check_out documents, renew/return check_outed documents
    class Patron : User{
        bool faculty;//is faculty member

        //return number of weeks for check_out or -1 for error
        int check_out_book(int id);
        int check_out_article(int id);
        int check_out_av(int id);

        //return QVector of my documents
        QVector<Book> get_checked_out_books();
        QVector<Article> get_checked_out_articles();
        QVector<VA> get_checked_out_avs();

        //return fine size or 0
        int return_book(int id);
        int return_article(int id);
        int return_av(int id);
    };

    //can search/add/delete/modify users, search/add/delete/modify documents, search overdue documents
    class Librarian : User{
        //search in Check_outs table
        QVector<Book> search_books_checked_out(int user_id, QString authors, QString title, QString keywords, QString publisher, int year, int bestseller, int available, bool or_and);
        QVector<Article> search_articles_checked_out(int user_id, QString authors, QString title, QString keywords, QString journal_title, QString publisher, QString editors, int year, int month, int available, bool or_and);
        QVector<VA> search_av_checked_out(int user_id, QString authors, QString title, QString keywords, int available, bool or_and);

        //return TRUE if success, FALSE if error
        QVector<Patron> search_patrons(QString name, QString address, QString phone, bool faculty, bool has_overdues, bool or_and);
        bool add_patron(QString name, QString address, QString phone, bool faculty);
        bool delete_patron(int user_id);
        bool modify_patron(int user_id, QString name, QString address, QString phone, bool faculty);

        QVector<Librarian> search_librarians(QString name, QString address, QString phone, bool or_and);
        bool add_librarian(QString name, QString address, QString phone);
        bool delete_librarian(int user_id);
        bool modify_librarian(int user_id, QString name, QString address, QString phone);

        bool add_book_new(QString authors, QString title, QString keywords, QString publisher, QString editors, int year, bool bestseller, int copies);
        bool add_book_copy(int id, int copies);
        bool add_article_new(QString authors, QString title, QString keywords, QString journal_title, QString publisher, QString editors, int year, int month, int copies);
        bool add_article_copy(int id, int copies);
        bool add_av_new(QString authors, QString title, QString keywords, int available, int copies);
        bool add_av_copy(int id, int copies);

        bool delete_book(int id, int copies);
        bool delete_article(int id, int copies);
        bool delete_av(int id, int copies);

        bool modify_book(int id, QString authors, QString title, QString keywords, QString publisher, int year, bool bestseller, int copies);
        bool modify_article(int id, QString authors, QString title, QString keywords, QString journal_title, QString publisher, QString editors, int year, int month, int copies);
        bool modify_av(int id, QString authors, QString title, QString keywords, bool available, int copies);
    };

    //return 0-error 1-patron 2-librarian
    int login(QString username, QString password);

}
