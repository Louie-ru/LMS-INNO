#include "tester2.h"
#include <QtSql>
#include <skeleton.cpp>


void Tester2::run_tests() {
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
   /* qDebug() << "TEST2: " << (test2() ? "OK" : "FAIL");
    Login::clear_database();
    qDebug() << "TEST3: " << (test3() ? "OK" : "FAIL");
    Login::clear_database();
    qDebug() << "TEST4: " << (test4() ? "OK" : "FAIL");
    Login::clear_database();
    qDebug() << "TEST5: " << (test5() ? "OK" : "FAIL");
    Login::clear_database();
    qDebug() << "TEST6: " << (test6() ? "OK" : "FAIL");
    Login::clear_database();
    qDebug() << "TEST7: " << (test7() ? "OK" : "FAIL");
    Login::clear_database();
    qDebug() << "TEST8: " << (test8() ? "OK" : "FAIL");
    Login::clear_database(); */
    sdb.close();
}

bool Tester2::test1(){
    LibrarianUser Librarian = Login::login_librarian("sidr", "123");
    Librarian.add_book("Introduction to Algorithms", "Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest and Clifford Stein", " MIT Press", "ITA", 2009, 5000, 0, 0, 3, 0, 0);
    Librarian.add_book("Design Patterns: Elements of Reusable Object-Oriented Software", "Erich Gamma, Ralph Johnson, John Vlissides, Richard Helm", "Addison-Wesley Professional", "Object-Oriented", 2003, 1700, 0, 0, 3, 1, 0);
    Librarian.add_va("Null References: The Billion Dollar Mistake", "Tony Hoare", "null", 700, 0, 0, 2, 0);
    Librarian.add_patron("Sergey Afonso", "Via Margutta, 3", "30001", 4, "s.afonso", "1");
    Librarian.add_patron("Nadia Teixeira", "Via Sacra, 13", "30002", 4, "n.teixeira", "1");
    Librarian.add_patron("Elvira Espindola", "Via del Corso, 22", "30003", 4, "e.espindola", "1");
    Librarian.add_patron("Andrey Velo", "Avenida Mazatlan 250", "30004", 1, "a.velo", "1");
    Librarian.add_patron("Veronika Rama","Stret Atocha, 27", "30005", 5, "v.rama", "1");

    PatronUser p1 = Login::login_patron("s.afonso", "1");
    int d1 = p1.search_books("Introduction to Algorithms","","","",2009,0,1,0)[0].id;
    int d2 = p1.search_books("Design Patterns: Elements of Reusable Object-Oriented Software","","","",2003,1,1,0)[0].id;

    QDate p1_d1d2_check(2018, 3, 5);
    //QDate* current_day(2018, 4, 2);

    p1.check_out_book(d1, &p1_d1d2_check);
    p1.check_out_book(d2, &p1_d1d2_check);

    LibrarianUser librarian = Login::login_librarian("sidr", "123");

    librarian.return_book(librarian.search_books_checked_out(0,"Design Patterns: Elements of Reusable Object-Oriented Software","","","",2003,1,0,0)[0].first.check_out_id);
    if(p1.get_checked_out_books()[0].first.day_end != 2 || p1.get_checked_out_books()[0].first.month_end != 4)
        return false;
    if(p1.get_checked_out_books()[0].first.fine != 0)
        return false;
    return true;
}


