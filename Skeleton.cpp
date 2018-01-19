/*
All strings(authors, keywords, editors...) are stored in one string (not vector<string>) separated by comma for making requests simpler like
SELECT * FROM Books WHERE authors CONTAINS "Thomas";


*/

class Document{
	string authors, title, keywords;
	int copies;//number of copies in library
	int id, price, room, level;
}

#define BOOK 1
#define ARTICLE 2
#define AV 3

class Book : Document{
	string publisher;
	int year;
	bool bestseller;
}

class Article : Document{
	string journal_title, publisher, editors;
	int year, month;
}

class AV : Document{
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
}

class Patron : User{
	bool faculty;
	vector<int> check_outs;//ids of user's check_outs

	/*
	"" for string not set
	-1 for int not set
	bool or_and:  TRUE if client wants all criterias be true; FALSE if client wants any criteria be true
	*/
	vector<Book> search_books(string author, string title, string keywords, string publisher, string editors, int year, int bestseller, int available, bool or_and);
	vector<Article> search_articles(string author, string title, string keywords, string journal_title, string publisher, string editors, int year, int month, int available, bool or_and);
	vector<AV> search_av(string author, string title, string keywords, int available, bool or_and);

	//return TRUE if success, FALSE if error
	bool check_out_book(int id);
	bool check_out_article(int id);
	bool check_out_av(int id);

	//return fine size or 0
	int return_book(int id);
	int return_article(int id);
	int return_av(int id);
}

class Librarian : User{
	//search in Check_outs table
	vector<Book> search_overdue_books(string author, string title, string keywords, string publisher, string editors, int year, int bestseller, int available, bool or_and);
	vector<Article> search_overdue_articles(string author, string title, string keywords, string journal_title, string publisher, string editors, int year, int month, int available, bool or_and);
	vector<AV> search_overdue_av(string author, string title, string keywords, int available, bool or_and);

	//return TRUE if success, FALSE if error
	bool add_patron(string name, string address, string phone, bool faculty);
	bool add_librarian(string name, string address, string phone, bool faculty);

	vector<Patron> search_patrons(string name, string address, string phone, bool faculty, bool or_and);
	bool delete_patron(int user_id);
	bool modify_patron(int user_id, string name, string address, string phone, bool faculty);

	Check_out get_check_out(int id);
	Book get_book(int id);
	Article get_article(int id);
	AV get_av(int id);

	bool add_book_new(string author, string title, string keywords, string publisher, string editors, int year, bool bestseller, int copies);
	bool add_book_copy(int id, int copies);
	bool add_article_new(string author, string title, string keywords, string journal_title, string publisher, string editors, int year, int month, int copies);
	bool add_article_copy(int id, int copies);
	bool add_av_new(string author, string title, string keywords, int available, int copies);
	bool add_av_copy(int id, int copies);

	bool delete_book(int id, int copies);
	bool delete_article(int id, int copies);
	bool delete_av(int id, int copies);

	bool modify_book(int id, string author, string title, string keywords, string publisher, string editors, int year, bool bestseller, int copies);
	bool modify_article(int id, string author, string title, string keywords, string journal_title, string publisher, string editors, int year, int month, int copies);
	bool modify_av(int id, string author, string title, string keywords, bool available, int copies);
}

int login(string username, string password);

DATABASE TABLES
----------------------------------------------------------------------------------------------------------------
Table: Books
Columns: string authors, string title, string keywords, string publisher, int copies, int id, int price, int room, int level, int year, bool bestseller;

Table: Articles
Columns: string authors, string title, string keywords, string journal_title, string publisher, string editors, int copies, int id, int price, int year, int month, int room, int level;

Table: AVs
Columns: string authors, string title, string keywords, int copies, int id, int price, int room, int level;

Table: Patrons
Columns: string name, string address, string phone, int id, bool faculty, string check_outs;

Table: Librarians
Columns: string name, string address, string phone, int id;

Table: Check_outs
Columns: int document_type, int document_id, check_out_id, int year_start, int month_start, int day_start, int year_end, int month_end, int day_end;
----------------------------------------------------------------------------------------------------------------


/*

*/
