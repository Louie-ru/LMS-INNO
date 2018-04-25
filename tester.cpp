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
    qDebug() << "TEST4: " << (test4() ? "OK" : "FAIL");
    Login::clear_database();
    qDebug() << "TEST5: " << (test5() ? "OK" : "FAIL");
    Login::clear_database();
    qDebug() << "TEST6: " << (test6() ? "OK" : "FAIL");
    Login::clear_database();
    qDebug() << "TEST7: " << (test7() ? "OK" : "FAIL");
    Login::clear_database();
    qDebug() << "TEST10: " << (test10() ? "OK" : "FAIL");
    Login::clear_database();
    sdb.close();
}

bool Tester::test1(){
    AdminUser admin1 = Login::login_admin("sidr", "123");
    return true;
}

bool Tester::test2(){
    AdminUser admin1 = Login::login_admin("sidr", "123");
    admin1.add_librarian("l1","Innopolis", "3333", "l1", "1", 1);
    admin1.add_librarian("l2","Innopolis", "3333", "l2", "1", 3);
    admin1.add_librarian("l3","Innopolis", "3333", "l3", "1", 3);
    return true;
}

bool Tester::test3(){
    test2();

    LibrarianUser l1 = Login::login_librarian("l1", "1");
    l1.add_book("Introduction to Algorithms", "Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest and Clifford Stein", " MIT Press", "Algorithms, Data Structures, Complexity, Computational Theory", 2009, 5000, 0, 0, 3, 0, 0);
    l1.add_book("Algorithms + Data Structures = Programs", "Niklaus Wirth", "Prentice Hall PTR", "Algorithms, Data Structures, Search Algorithms, Pascal", 1978, 5000, 1, 1, 3, 0, 0);
    l1.add_book("The Art of Computer Programming", "Donald E. Knuth", "Addison Wesley Longman Publishing Co., Inc.", "Algorithms, Combinatorial Algorithms, Recursion", 1977, 5000, 1, 1, 3, 0, 0);
    return true;
}

bool Tester::test4(){
    test2();

    LibrarianUser l2 = Login::login_librarian("l2", "1");
    l2.add_book("Introduction to Algorithms", "Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest and Clifford Stein", " MIT Press", "Algorithms, Data Structures, Complexity, Computational Theory", 2009, 5000, 0, 0, 3, 0, 0);
    l2.add_book("Algorithms + Data Structures = Programs", "Niklaus Wirth", "Prentice Hall PTR", "Algorithms, Data Structures, Search Algorithms, Pascal", 1978, 5000, 1, 1, 3, 0, 0);
    l2.add_book("The Art of Computer Programming", "Donald E. Knuth", "Addison Wesley Longman Publishing Co., Inc.", "Algorithms, Combinatorial Algorithms, Recursion", 1977, 5000, 1, 1, 3, 0, 0);

    l2.add_patron("Andrey Velo", "Avenida Mazatlan 250", "30004", 1, "a.velo", "1");
    l2.add_patron("Sergey Afonso", "Via Margutta, 3", "30001", 4, "s.afonso", "1");
    l2.add_patron("Nadia Teixeira", "Via Sacra, 13", "30002", 4, "n.teixeira", "1");
    l2.add_patron("Elvira Espindola", "Via del Corso, 22", "30003", 4, "e.espindola", "1");
    l2.add_patron("Veronika Rama","Stret Atocha, 27", "30005", 5, "v.rama", "1");


    return true;
}

bool Tester::test5(){
    test4();

    LibrarianUser l3 = Login::login_librarian("l3", "1");

    int d1 = l3.search_books("Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest and Clifford Stein","","","",0,0,0,0)[0].id;

    l3.modify_book(d1,"","","","",0,5000,1,1,2,0,0);

    if (l3.get_book(d1).copies == 2)
        return true;

    return false;


}


bool Tester::test6(){
    test4();

    LibrarianUser l1 = Login::login_librarian("l1","1");

    PatronUser p1 = Login::login_patron("s.afonso", "1");

    int d3 = p1.search_books("Donald E. Knuth","","","",0,0,0,0)[0].id;

    p1.check_out_book(d3);

    PatronUser p2 = Login::login_patron("n.teixeira", "1");

    p2.check_out_book(d3);
    l1.modify_book(d3,"","","","",0,0,0,0,1,0,0);

    PatronUser s = Login::login_patron("a.velo", "1");

    s.check_out_book(d3);
    l1.modify_book(d3,"","","","",0,0,0,0,0,0,0);

    PatronUser v = Login::login_patron("v.rama", "1");

    v.check_out_book(d3);

    PatronUser p3 = Login::login_patron("e.espindola", "1");

    p3.check_out_book(d3);
    //l1.outstanding_book(d3);

    return true;

}

bool Tester::test7(){
    test4();

    PatronUser p1 = Login::login_patron("s.afonso", "1");
    LibrarianUser l3 = Login::login_librarian("l3","1");

    int d3 = p1.search_books("Donald E. Knuth","","","",0,0,0,0)[0].id;

    p1.check_out_book(d3);

    PatronUser p2 = Login::login_patron("n.teixeira", "1");

    p2.check_out_book(d3);
    l3.modify_book(d3,"","","","",0,0,0,0,1,0,0);

    PatronUser s = Login::login_patron("a.velo", "1");

    s.check_out_book(d3);
    l3.modify_book(d3,"","","","",0,0,0,0,0,0,0);

    PatronUser v = Login::login_patron("v.rama", "1");

    v.check_out_book(d3);

    PatronUser p3 = Login::login_patron("e.espindola", "1");

    p3.check_out_book(d3);
    //l3.outstanding_book(d3);


    return true;

}

bool Tester::test10(){
    return true;
}









