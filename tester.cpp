#include "tester.h"
#include <QtSql>
#include <skeleton.cpp>


void Tester::run_tests() {
    LibrarianUser libr;
    const QString db_name = "db_test.sqlite";

    QSqlDatabase sdb;
    sdb = QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName(db_name);
    sdb.open();
    Login::make_database();
    Login::clear_database();    

    qDebug() << "TEST1: " << (test1() ? "OK" : "FAIL");
    Login::clear_database();
    qDebug() << "TEST2: " << (test2() ? "OK" : "FAIL");
    Login::clear_database();
    qDebug() << "TEST3: " << (test3() ? "OK" : "FAIL");
    Login::clear_database();
    //qDebug() << "TEST4: " << (test4() ? "OK" : "FAIL");
    //Login::clear_database();
    qDebug() << "TEST5: " << (test5() ? "OK" : "FAIL");
    Login::clear_database();
    qDebug() << "TEST6: " << (test6() ? "OK" : "FAIL");
    Login::clear_database();
    qDebug() << "TEST7: " << (test7() ? "OK" : "FAIL");
    Login::clear_database();
    qDebug() << "TEST8: " << (test8() ? "OK" : "FAIL");
    Login::clear_database();
    sdb.close();
}


bool Tester::test1() {
    LibrarianUser Librarian = Login::login_librarian("sidr", "123");
    Librarian.add_book("Design Patterns: Elements of Reusable Object-Oriented Software", "Erich Gamma, Ralph Johnson, John Vlissides, Richard Helm", "Addison-Wesley Professional", "Object-Oriented", 2003, 1000, 0, 0, 2, 1, 0);
    Librarian.add_book("Introduction to Algorithms", "Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest and Clifford Stein", " MIT Press", "ITA", 2009, 1000, 0, 0, 3, 0, 0);
    Librarian.add_book("The Mythical Man-month", "Brooks,Jr., Frederick P", "Addison-Wesley Longman Publishing Co., Inc.", "Mythical", 1995, 1000, 0, 0, 1, 0, 1);

    Librarian.add_va("Null References: The Billion Dollar Mistake", "Tony Hoare", "null", 1000, 0, 0, 1, 0);
    Librarian.add_va("Information Entropy", "Claude Shannon", "Information", 1000, 0, 0, 1, 0);

    Librarian.add_patron("Sergey Afonso", "Via Margutta, 3", "30001", 1, "s.afonso", "1");
    Librarian.add_patron("Nadia Teixeira", "Via Sacra, 13", "30002", 0, "n.teixeira", "1");
    Librarian.add_patron("Elvira Espindola", "Via del Corso, 22", "30003", 0, "e.espindola", "1");

    QVector <PatronUser> patrons = Librarian.search_patrons(0,"","","",0,0);
    QVector <LibrarianUser> librarians = Librarian.search_librarians(0,"","","",0);
    QVector <Book> books = Librarian.search_books("","","","",0,0,0,0);
    QVector <VA> avs = Librarian.search_vas("","","",0,0);

    int users = 0;
    int documents = 0;

    for(int i = 0; i < books.length(); i++){
        documents += books[i].copies;
    }

    for(int i = 0; i < avs.length(); i++){
        documents += avs[i].copies;
    }

    for(int i = 0; i < patrons.length(); i++){
        users++;
    }

    for(int i = 0; i < librarians.length(); i++){
        users++;
    }

    if (documents != 8 || users != 4)
        return false;
    return true;
}

bool Tester::test2() {
    test1();
    LibrarianUser libr = Login::login_librarian("sidr", "123");
    //Removing books
    Book b1 = libr.search_books("", "Introduction to Algorithms", "", "", 0, false, false, false)[0];
    libr.modify_book(b1.id, b1.title, b1.publisher, b1.authors, b1.keywords, b1.year, b1.price,b1.room,b1.level,b1.copies - 2,b1.bestseller,b1.reference);
    Book b3 = libr.search_books("", "The Mythical Man-month", "", "", 0, false, false, false)[0];
    libr.modify_book(b3.id, b3.title, b3.publisher, b1.authors, b3.keywords, b3.year, b3.price,b3.room,b3.level,b3.copies - 1,b3.bestseller,b3.reference);
    PatronUser p2 = libr.search_patrons(0, "Nadia Teixeira", "", "", false, false)[0];
    libr.delete_patron(p2.id);

    //Count the rest amount of documents
    int docs = libr.search_books("","","","",0,false,false,false).size()      +
               libr.search_articles("","","","","","",0,0,false,false).size() +
               libr.search_vas("","","",false,false).size();
    int users = libr.search_patrons(0,"","","",false,false).size() +
                libr.search_librarians(0,"","","",false).size();

    return docs == 5 && users == 3;
}

bool Tester::test3() {
    test1();
    LibrarianUser librarian = Login::login_librarian("sidr", "123");
    QVector<PatronUser> p = librarian.search_patrons(0, "Sergey Afonso", "", "", 0, 0);
    if (p.size() != 1) return 0;
    QVector<PatronUser> p2 = librarian.search_patrons(0, "Elvira Espindola", "", "", 0, 0);
    if (p.size() != 1) return 0;
    return 1;
}

bool Tester::test5() {
    test2();
    //Try to login
    PatronUser p2 = Login::login_patron("n.teixeira", "1");
    //Check that login unsuccessful
    return p2.id == -1;
}

bool Tester::test6() {
    test2();
    PatronUser p1 = Login::login_patron("s.afonso", "1");
    PatronUser p3 = Login::login_patron("e.espindola", "1");

    Book b1 = p1.search_books("", "Introduction to Algorithms","","",0,0,0,0)[0];
    Book b2 = p1.search_books("", "Design Patterns: Elements of Reusable Object-Oriented Software", "", "", 0, 0, 0, 0)[0];

    p1.check_out_book(b1.id);
    p3.check_out_book(b1.id);
    p1.check_out_book(b2.id);

    LibrarianUser librarian = Login::login_librarian("sidr", "123");
    p1 = librarian.get_patron(p1.id);
    p3 = librarian.get_patron(p3.id);

    if (p1.check_outs.size() != 2 || p3.check_outs.size() != 0)
        return false;

    return true;
}

bool Tester::test7() {
    test1();
    PatronUser p1 = Login::login_patron("s.afonso", "1");
    Book b1 = p1.search_books("", "Introduction to Algorithms", "", "", 0, false, false, false)[0];
    Book b2 = p1.search_books("", "Design Patterns: Elements of Reusable Object-Oriented Software", "", "", 0, 0, 0, 0)[0];
    Book b3 = p1.search_books("", "The Mythical Man-month", "", "", 0, false, false, false)[0];
    VA av1 = p1.search_vas("Tony Hoare", "", "", 0, 0)[0];
    p1.check_out_book(b1.id);
    p1.check_out_book(b2.id);
    p1.check_out_book(b3.id);
    p1.check_out_av(av1.id);

    PatronUser p2 = Login::login_patron("n.teixeira", "1");
    VA av2 = p2.search_vas("", "Information Entropy", "", 0, 0)[0];
    p2.check_out_book(b1.id);
    p2.check_out_book(b2.id);
    p2.check_out_av(av2.id);

    LibrarianUser Librarian = Login::login_librarian("sidr", "123");
    PatronUser patron1 = Librarian.get_patron(p1.id);
    PatronUser patron2 = Librarian.get_patron(p2.id);

    if (patron1.check_outs.length() != 3 || patron2.check_outs.length() != 3)
        return false;
    return true;
}

bool Tester::test8() {
    test1();
    LibrarianUser libr = Login::login_librarian("sidr", "123");
    QDate p1_b1_checkday(2018, 9, 3);
    QDate p1_b2_checkday(2018, 2, 2);
    QDate p2_b1_checkday(2018, 2, 5);
    QDate p2_av1_checkday(2018, 2, 17);
    QDate current_day(2018, 3, 5);

    PatronUser p1 = libr.search_patrons(0, "Sergey Afonso", "", "", false, false)[0];
    PatronUser p2 = libr.search_patrons(0, "Nadia Teixeira", "", "", false, false)[0];

    int b1 = libr.search_books("","Introduction to Algorithms","","",0,false,false,false)[0].id;
    int b2 = libr.search_books("","Design Patterns: Elements of Reusable Object-Oriented Software","","",0,false,false,false)[0].id;
    int av1 = libr.search_vas("", "Null References: The Billion Dollar Mistake","",false,false)[0].id;

    p1.check_out_book(b1, &p1_b1_checkday);
    p1.check_out_book(b2, &p1_b2_checkday);
    p2.check_out_book(b1, &p2_b1_checkday);
    p2.check_out_av(av1, &p2_av1_checkday);

    bool c1,c2,c3;

    QVector<pair<Check_out, Book> > p1_books = p1.get_checked_out_books();
    QVector<pair<Check_out, Book> > p2_books = p2.get_checked_out_books();
    QVector<pair<Check_out, VA> > p2_vas = p2.get_checked_out_vas();
    for (int i = 0; i < p1_books.size(); i++){
        pair<Check_out, Book> p = p1_books[i];
        if (p.second.id == b2) {
            QDate date(p.first.year_end, p.first.month_end, p.first.day_end);
            int overdue = current_day.toJulianDay() - date.toJulianDay();
            c1 = overdue == 3;
        }
    }
    for (int i = 0; i < p2_books.size(); i++){
        pair<Check_out, Book> p = p2_books[i];
        if(p.second.id == b1) {
            QDate date(p.first.year_end, p.first.month_end, p.first.day_end);
            int overdue = current_day.toJulianDay() - date.toJulianDay();
            c2 = overdue == 7;
        }
    }
    for (int i = 0; i < p2_vas.size(); i++){
        pair<Check_out, VA> p = p2_vas[i];
        if(p.second.id == av1) {
            QDate date(p.first.year_end, p.first.month_end, p.first.day_end);
            int overdue = current_day.toJulianDay() - date.toJulianDay();
            c3 = overdue == 2;
        }
    }

    return c1 && c2 && c3;
}
