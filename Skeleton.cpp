/*
All strings(authors, keywords, editors...) are stored in one string (not vector<string>) separated by comma for making requests simpler like
SELECT * FROM Books WHERE authors CONTAINS "Thomas";

In search requests:
"" for string not set
-1 for int not set
bool or_and:  TRUE if client wants all criterias be true; FALSE if client wants any criteria be true
*/

class Document{
	string title, keywords;
	int copies, id, price, room, level;
}

//defined type constants
#define BOOK 1
#define ARTICLE 2
#define AV 3

class Book : Document{
	string publisher, authors;
	int year;
	bool bestseller;
}

class Article : Document{
	string journal_title, publisher, editors, authors;
	int year, month;
}

class AV : Document{
	string authors;
}

class Check_out {
	int document_type;//1-book 2-article 3-av
	int document_id;//id in Books/Articles/AVs table
	int check_out_id;//id in Check_outs table
	int year_start, month_start, day_start;//taken date
	int year_end, month_end, day_end;//need to return date
}


class User{
	string name, address, phone;
	int id;//id in Patrons/Librarians table

	vector<Book> search_books(string authors, string title, string keywords, string publisher, string editors, int year, int bestseller, int available, bool or_and);
	vector<Article> search_articles(string authors, string title, string keywords, string journal_title, string publisher, string editors, int year, int month, int available, bool or_and);
	vector<AV> search_av(string authors, string title, string keywords, int available, bool or_and);
}

//can search/check_out documents, renew/return check_outed documents
class Patron : User{
	bool faculty;//is faculty member

	//return number of weeks for check_out or -1 for error
	int check_out_book(int id);
	int check_out_article(int id);
	int check_out_av(int id);

	//return vector of my documents
	vector<Book> get_checked_out_books();
	vector<Article> get_checked_out_articles();
	vector<AV> get_checked_out_avs();

	//return fine size or 0
	int return_book(int id);
	int return_article(int id);
	int return_av(int id);
}

//can search/add/delete/modify users, search/add/delete/modify documents, search overdue documents
class Librarian : User{
	//search in Check_outs table
	vector<Book> search_books_checked_out(int user_id, string authors, string title, string keywords, string publisher, string editors, int year, int bestseller, int available, bool or_and);
	vector<Article> search_articles_checked_out(int user_id, string authors, string title, string keywords, string journal_title, string publisher, string editors, int year, int month, int available, bool or_and);
	vector<AV> search_av_checked_out(int user_id, string authors, string title, string keywords, int available, bool or_and);

	//return TRUE if success, FALSE if error
	vector<Patron> search_patrons(string name, string address, string phone, bool faculty, bool has_overdues, bool or_and);
	bool add_patron(string name, string address, string phone, bool faculty);
	bool delete_patron(int user_id);
	bool modify_patron(int user_id, string name, string address, string phone, bool faculty);

	vector<Librarian> search_librarians(string name, string address, string phone, bool or_and);
	bool add_librarian(string name, string address, string phone);
	bool delete_librarian(int user_id);
	bool modify_librarian(int user_id, string name, string address, string phone);

	bool add_book_new(string authors, string title, string keywords, string publisher, string editors, int year, bool bestseller, int copies);
	bool add_book_copy(int id, int copies);
	bool add_article_new(string authors, string title, string keywords, string journal_title, string publisher, string editors, int year, int month, int copies);
	bool add_article_copy(int id, int copies);
	bool add_av_new(string authors, string title, string keywords, int available, int copies);
	bool add_av_copy(int id, int copies);

	bool delete_book(int id, int copies);
	bool delete_article(int id, int copies);
	bool delete_av(int id, int copies);

	bool modify_book(int id, string authors, string title, string keywords, string publisher, string editors, int year, bool bestseller, int copies);
	bool modify_article(int id, string authors, string title, string keywords, string journal_title, string publisher, string editors, int year, int month, int copies);
	bool modify_av(int id, string authors, string title, string keywords, bool available, int copies);
}

//return 0-error 1-patron 2-librarian
int login(string username, string password);
