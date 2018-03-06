#include "tester.h"
#include <QtSql>
#include <skeleton.cpp>

void Tester::run_tests() {
    const QString db_name = "db_test.sqlite";

    QSqlDatabase sdb;
    sdb = QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName(db_name);
    sdb.open();
    Login::make_database();
    LibrarianUser libr;
    libr.add_librarian("Сидорович", "Бункер на Кордоне", "88005553535", "sidr", "123");

    qDebug() << "TEST1: " << (test1() ? "OK" : "FAIL");
    qDebug() << "TEST2: " << (test2() ? "OK" : "FAIL");
    qDebug() << "TEST3: " << (test3() ? "OK" : "FAIL");
    qDebug() << "TEST4: " << (test4() ? "OK" : "FAIL");
    qDebug() << "TEST5: " << (test5() ? "OK" : "FAIL");
    qDebug() << "TEST6: " << (test6() ? "OK" : "FAIL");
    qDebug() << "TEST7: " << (test7() ? "OK" : "FAIL");
    qDebug() << "TEST8: " << (test8() ? "OK" : "FAIL");
    qDebug() << "TEST9: " << (test9() ? "OK" : "FAIL");

    sdb.close();
    //QFile(db_name).remove();
}


bool Tester::test1() {
    LibrarianUser Librarian = mainLogin.login_librarian("sidr", "123");
    Librarian.add_book("Design Patterns: Elements of Reusable Object-Oriented Software", "Erich Gamma, Ralph Johnson, John Vlissides, Richard Helm", "Addison-Wesley Professional", "Object-Oriented", 2003, 1000, 0, 0, 2, 1, 0);
    Librarian.add_book("Introduction to Algorithms", "Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest and Clifford Stein", " MIT Press", "ITA", 2009, 1000, 0, 0, 3, 0, 0);
    Librarian.add_book("The Mythical Man-month", "Brooks,Jr., Frederick P", "Addison-Wesley Longman Publishing Co., Inc.", "Mythical", 1995, 1000, 0, 0, 1, 0, 1);
        
    Librarian.add_va("Null References: The Billion Dollar Mistake", "Tony Hoare", "null", 1000, 0, 0, 1, 0);
    Librarian.add_va("Information Entropy", "Claude Shannon", "Information", 1000, 0, 0, 1, 0);
    
    Librarian.add_patron("Sergey Afonso", "Via Margutta, 3", "30001", 1, "s.afonso", "1");
    Librarian.add_patron("Nadia Teixeira", "Via Sacra, 13", "30002", 0, "n.teixeira", "1");
    Librarian.add_patron("Elvira Espindola", "Via del Corso, 22", "30003", 0, "e.espindola", "1");
    
    QVector <PatronUser> patrons = Librarian.search_patrons(0,"","","",1,1);
    QVector <LibrarianUser> librarians = Librarian.search_librarians(0,"","","",1);
    QVector <Book> books = Librarian.search_books("","","","",0,0,0,1);
    QVector <VA> avs = Librarian.search_vas("","","",0,1);
    
    int users = 0;
    int documents = 0;
    
    for(int i = 0; i < books.length(); i++){
        documents++;
    }
    
    for(int i = 0; i < avs.length(); i++){
        documents++;
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
    return true;
}

bool Tester::test3() {
    return true;
}

bool Tester::test4() {
    test2();

    LibrarianUser Librarian = mainLogin.login_librarian("sidr", "123");

    PatronUser patron = Librarian.get_patron(1011);
    if(patron.name != ""){
        return false;
    }

    PatronUser patron1 = Librarian.get_patron(1100);
    if(patron1.name == ""){
        return false;
    }

    else{
         if (patron1.name != "Elvira Espindola"){
             return false;
         }
         if (patron1.address != "Via del Corso, 22"){
             return false;
         }
         if (patron1.phone != "30003"){
             return false;
         }
         if (patron1.faculty != 0){
             return false;
         }
         return true;
    }
}

bool Tester::test5() {
    return true;
}

bool Tester::test6() {
    return true;
}

bool Tester::test7() {
    test1();

    PatronUser p1 = mainLogin.login_patron("s.afonso", "1");
    Book b1 = p1.search_books("Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest and Clifford Stein", "Introduction to Algorithms","","",2009,0,1,1)[0];
    Book b2 = p1.search_books("Erich Gamma, Ralph Johnson, John Vlissides, Richard Helm", "Design Patterns: Elements of Reusable Object-Oriented Software", "", "", 2003, 1, 1, 1)[0];
    Book b3 = p1.search_books("Brooks,Jr., Frederick P", "The Mythical Man-month", "", "", 1995, 0, 1, 1)[0];
    VA av1 = p1.search_vas("Tony Hoare", "Null References: The Billion Dollar Mistake", "", 1, 1)[0];
    p1.check_out_book(b1.id);
    p1.check_out_book(b2.id);
    p1.check_out_book(b3.id);
    p1.check_out_av(av1.id);

    PatronUser p2 = mainLogin.login_patron("n.teixeira", "1");
    VA av2 = p2.search_vas("Claude Shannon", " Information Entropy", "", 1, 1)[0];
    p2.check_out_book(b1.id);
    p2.check_out_book(b2.id);
    p2.check_out_av(av2.id);

    LibrarianUser Librarian = mainLogin.login_librarian("sidr", "123");
    PatronUser patron1 = Librarian.get_patron(p1.id);
    PatronUser patron2 = Librarian.get_patron(p2.id);
    
    if (patron1.check_outs.length() != 2){
        return false;
    }
    
    if (patron2.check_outs.length() != 2){
        return false;
    }
    
    return true;
}

bool Tester::test8() {
    return true;
}

bool Tester::test9() {
    return true;
}
