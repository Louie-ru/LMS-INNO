#include <QString>
#include <QVector>
#include <time.h>
#include <pair>
#include <string>
using namespace std;

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

#define localsolt "rnksrlhwdrrohbdr"

string hashPassword(const string &password, int id) {
	return sha512(password + localsolt + to_string(id));
}

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

	QVector<Book>* search_books(QString *authors, QString *title, QString *keywords, QString *publisher, int *year, bool bestseller, bool available, bool or_and) {
		return db::search_books(authors, title, keywords, publisher, year, bestseller, available, or_and);
	}
	QVector<Article>* search_articles(QString *authors, QString *title, QString *keywords, QString *journal_title, QString *publisher, QString *editors, int *year, int *month, bool available, bool or_and) {
		return db::search_articles(authors, title, keywords, publisher, year, bestseller, available, or_and);
	}
	QVector<VA>* search_av(QString *authors, QString *title, QString *keywords, bool available, bool or_and) {
		return db::search_av(authors, title, keywords, available, or_and);
	}

	User(QString &name, QString &address, QString &phone, int id, QString &login, string &password) {
		this.name = name;
		this.address = address;
		this.phone = phone;
		this.login = login;
		this.password = password;
		this.id = id;
	}
};

//can search/check_out documents, renew/return check_outed documents
class PatronUser : public User{
private:
	const int sec_in_day = 60 * 60 * 24;
	const int fine_per_day = 100;

	tm getDate() {
		time_t t = time(NULL);
		tm *d = localtime(t);
		tm r = *d;
		delete d;
		return r;
	}
	tm add_weeks(const tm& d, int weeks) {
		d->tm_yday += weeks * 7;
		time_t t = mktime(&d);
		tm *d2 = localtime(&t);
		tm r = *d2;
		delete d2;
		return r
	}
	int calculate_fine(Check_out *ch, int price) { 
		time_t t1 = time(NULL);
		time_t t0;
		tm *d = localtime(&t0);
		d->tm_year = ch->year_end - 1900;
		d->tm_mon = ch->month_end;
		d->tm_mday = ch->day_end;
		time_t t2 = mktime(d);
		delete d;
		int fine = ((t2 - t1) / sec_in_day) * fine_per_day;
		if (fine > 0)
			if (fine > price)
				return price;
			else
				return fine;
		else
			return 0;
	}

public:
    bool faculty;//is faculty member

    //return number of weeks for check_out or -1 for error
	int check_out_book(int id) {
		Book *book = db::get_book(id);
		if (book != null) {
			if (book->copies > 0) {
				book->copies -= 1;
				db::modify_book(book);
				int weeks = 0;
				if (faculty)
					weeks = 4;
				else if (book->bestseller)
					weeks = 2;
				else
					weeks = 3;
				
				tm tm1 = getDate();
				tm tm2 = add_weeks(tm1, weeks);
				db::add_checkout(book, BOOK, tm1, tm2);
				delete book;
				return weeks;
			}
			else {
				return -1
			}
		}
		else {
			return -1;
		}
	}

	int check_out_article(int id) {
		Article *article = db::get_article(id);
		if (article != null) {
			if (article->copies > 0) {
				book->copies -= 1;
				db::modify_article(article);
				int weeks = 2;
				tm tm1 = getDate();
				tm tm2 = add_weeks(tm1, weeks);
				db::add_checkout(article, ARTICLE, tm1, add_weeks();
				delete article;
				return weeks;
			}
			else {
				return -1;
			}
		}
		else {
			return -1;
		}
	}

	int check_out_av(int id) {
		VA *av = db::get_av(id);
		if (av != null) {
			if (av->copies > 0) {
				av->copies -= 1;
				db::modify_article(av);
				int weeks = 2;
				tm tm1 date = getDate();
				tm tm2 = add_weeks(tm1, weeks);
				db::add_checkout(av, AV, tm1, tm2);
				delete av;
				return weeks;
			}
			else {
				return -1;
			}
		}
		else {
			return -1;
		}
	}

    //return QVector of my documents
	QVector<pair<Check_out, Book>>* get_checked_out_books() {
		return db::search_books_checked_out(&id, nullptr, nullptr, nullptr, nullptr, nullptr, false, false);
	}

	QVector<pair<Check_out, Article>>* get_checked_out_articles() {
		return db::search_articles_checked_out(&id, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, false);
	}
	QVector<pair<Check_out, VA>> get_checked_out_avs() {
		return db::search_av_checked_out(&id, nullptr, nullptr, nullptr, false);
	}

    //return fine size or 0
	int return_book(int check_id) {
		Check_out *ch = db::get_check_out(check_id);
		Book *book = db::get_book(ch->document_id);
		int fine = calculate_fine(ch, book->price);
		db::delete_checkout(check_id);
		book->copies += 1;
		db::modify_book(book);
		delete ch;
		delete book;
		return fine;
	}
	int return_article(int check_id) {
		Check_out *ch = db::get_check_out(check_id);
		Article *article = db::get_article(ch->document_id);
		int fine = calculate_fine(ch, article->price);
		db::delete_checkout(check_id);
		article->copies += 1;
		db::modify_article(article);
		delete ch;
		delete article;
		return fine;
	}
	int return_av(int check_id) {
		Check_out *ch = db::get_check_out(check_id);
		VA *av = db::get_article(ch->document_id);
		int fine = calculate_fine(ch, av->price);
		db::delete_checkout(check_id);
		av->copies += 1;
		db::modify_av(av);
		delete ch;
		delete av;
		return fine;
	}

    PatronUser(){}
	PatronUser(QString &name, QString &address, QString &phone, int id, bool faculty, QString &login, string &password) : User(name, address, phone, id, login, password) {
		this.faculty = faculty;
	}
};

//can search/add/delete/modify users, search/add/delete/modify documents, search overdue documents
class LibrarianUser : public User{
public:
    //search in Check_outs table
    QVector<std::pair<Check_out, Book>>* search_books_checked_out(int *user_id, QString *authors, QString *title, QString *keywords, QString *publisher, int *year, bool bestseller, bool or_and);
    QVector<std::pair<Check_out, Article>>* search_articles_checked_out(int *user_id, QString *authors, QString *title, QString *keywords, QString *journal_title, QString *publisher, QString *editors, int *year, int *month, bool or_and);
    QVector<std::pair<Check_out, VA>>* search_av_checked_out(int *user_id, QString *authors, QString *title, QString *keywords, bool or_and);

    //return empty vector if there are no such users
	QVector<PatronUser>* search_patrons(QString *name, QString *address, QString *phone, bool *faculty, bool has_overdues, bool or_and) {
		return db::search_patrons(name, address, phone, faculty, has_overdues, or_and);
	}
	//return TRUE if success, FALSE if error
	bool add_patron(QString name, QString address, QString phone, int id, bool faculty, QString login, string password) {
		password = hashPassword(password, id);
		PatronUser *user = new PatronUser(name, address, phone, id, faculty, login, password);
		bool r = db::add_patron(user);
		delete user;
		return r;
	}
	bool delete_patron(int user_id) {
	}
    bool modify_patron(int user_id, QString name, QString address, QString phone, bool faculty);

	//return empty vector if there are no such librarians
    QVector<LibrarianUser> search_librarians(QString name, QString address, QString phone, bool or_and);
	//return TRUE if success, FALSE if error
	bool add_librarian(QString name, QString address, QString phone, int id, QString login, string password) {
		password = hashPassword(password, id);
		LibrarianUser *user = new LibrarianUser(name, address, phone, id, login, password);
		bool r = db::add_librarian(user);
		delete user;
		return r;
	}
    bool delete_librarian(int user_id);
    bool modify_librarian(int user_id, QString name, QString address, QString phone);

    bool add_book_new(QString authors, QString title, QString keywords, QString publisher, QString editors, int year, bool bestseller, int copies, int price, int room, int level){
       if(db::search_books(authors, title, publisher, editors, year).size == 0){
            Book* book = new Book(authors,title,keywords,publisher,0,year,copies,price,room,level,bestseller);
            db::add_book(book);
            delete book;
            return true;
        }
        return false;

    }

    bool add_article_new(QString authors, QString title, QString keywords, QString journal_title, QString publisher, QString editors, int year, int month, int copies, int price, int room, int level){
        if(db::search_articles(authors, title, publisher, editors, year, month).size == 0){
            Article* article = new Article(authors,title,journal_title,keywords,publisher,editors,0,year,month,copies,price,room,level);
            db::add_article(article);
            delete article;
            return true;
        }
        return false;
    }

    bool add_av_new(QString authors, QString title, QString keywords, int copies, int price, int room, inr level){
        if(db::search_av(authors,title,keywords).size == 0){
            VA* av = new VA(authors,title,keywords,0,copies,price,room,level);
            db::add_av(av);
            delete av;
            return true;
        }
        return false;
    }

    bool delete_book(int id, int copies){
        if(db::get_book(id) == null)
            return false;
        if (copies == 0){
            db::delete_book(id);
            return true;
        }
        Book* book = db::get_book(id);
        modify_book(book->id,book->authors,book->title,book->keywords,book->publisher,book->year,book->bestseller,book->copies-copies);
        return true;
    }
    bool delete_article(int id, int copies){
        if(db::get_article(id) == null)
            return false;
        if(copies == 0){
            db::delete_article(id);
            return true;
        }
        Article* article = db::get_article(id);
        modify_article(article->id,article->authors,article->title,article->keywords,article->journal_title,article->publisher,article->editors,article->year,article->month,article->copies-copies);
        return true;
    }
    bool delete_av(int id, int copies){
        if(db::get_av(id) == null)
            return false;
        if(copies == 0){
            db::delete_av(id);
            return true;
        }
        VA* av = db::get_av(id);
        modify_av(av->id,av->authors,av->title,av->keywords,true,av->copies-copies);
        return true;
    }

    bool modify_book(int id, QString authors, QString title, QString keywords, QString publisher, int year, bool bestseller, int copies,int room,int level){
        Book* book = new Book(authors,title,keywords,publisher,id,year,copies,price,room,level,bestseller);
        db::modify_book(book);
        delete book;
    }
    bool modify_article(int id, QString authors, QString title, QString keywords, QString journal_title, QString publisher, QString editors, int year, int month, int copies, int price, int room, int level){
        Article* article = new Article(authors,title,journal_title,keywords,publisher,editors,id,year,month,copies,price,room,level);
        db::modify_article(article);
        delete article;
    }
    bool modify_av(int id, QString authors, QString title, QString keywords, bool available, int copies, int price, int room, int level){
        VA* av = new VA(authors,title,keywords,id,copies,price,room,level);
        db::modify_av(av);
        delete av;
    }
    LibrarianUser(){}
	LibrarianUser(QString &name, QString &address, QString &phone, int id, QString &login, string &password) : User(name, address, phone, id, login, password) {}
};

// return null in case of error
pair<User, int>* login(QString login, string password) {
	pair<User, int>* user = db::get_user(login);
	password = hashPassword(password, user->first.id);
	if (user->first.password == password)
		return pair;
	else
		return null;
}
