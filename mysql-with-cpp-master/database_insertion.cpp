#include "database.h"

void MySQL ::  newCustomer() {
    string fname, lname, cellular, landline;
    cout << "To create a new customer please insert the following details:" << endl;
    cout << "First name:";
    getline(cin.ignore(), fname);
    cout << "Last name:";
    getline(cin.ignore(), lname);
    cout << "Cellular:";
    getline(cin.ignore(), cellular);
    cout << "Landline Phone:";
    getline(cin.ignore(), landline);

    string toInsert = "INSERT INTO customer (first_name, last_name, cellular, landline) VALUES('";
    toInsert += fname + "','" + lname + "','" + cellular + "','" + landline + "')";

    mysql_query(connect, toInsert.c_str());
}

void MySQL :: newBook() {
    string bname, bstate, weight, pages;

    cout << "Please enter the Book details:" << endl;
    cout << "Book name:";
    getline(cin.ignore(), bname);
    cout << "Book state ('as new'/'new'/'good'/'bad'):";
    getline(cin.ignore(), bstate);
    cout << "Weight:";
    getline(cin.ignore(), weight);
    cout << "Pages:";
    getline(cin.ignore(), pages);

    string insertBook = "INSERT INTO books (book_name, book_state, weight, pages) VALUES('";
    insertBook += bname + "','" + bstate + "','" + weight + "','" + pages + "')";

    mysql_query( connect, insertBook.c_str());

    string a_fname, a_lname;
    cout << "Please enter the Author details:" << endl;
    cout << "First name:";
    getline(cin.ignore(), a_fname);
    cout << "Last name:";
    getline(cin.ignore(), a_lname);

    
    
    /*
    if (numrows == 0) {
        string insertAuthor = "INSERT INTO author (first_name, last_name, book_name) VALUES('";
        insertAuthor += a_fname + "','" + a_lname + "','" + bname + "')";

        ////// INSERT AUTHOR WITH QUERY 'insertAuthor'
    }
    */
    string p_name, p_year;
    cout << "Please enter the Publisher details:" << endl;
    cout << "Brand name:";
    getline(cin, p_name);
    cout << "Publish year:";
    getline(cin, p_year);

    string publisherCheck = "select * from publisher where brand_name='" + p_name + "' and publish_year='" + p_year + "' and book_name='" + bname + "'";

    ////// Query for PUBLISHER existence

    /*
    if (numrows == 0) {
        string insertPublisher = "INSERT INTO publisher (brand_name, publish_year, book_name) VALUES('";
        insertPublisher += p_name + "','" + p_year + "','" + bname + "')";

        ////// INSERT PUBLISHER WITH QUERY 'insertAuthor'
    }*/

    cout << "Is this book translated? (type the 1/2)\n1. Yes\n2. No" << endl;
    int choice = 0;
    cin >> choice;
    if (choice == 1) {
        string t_fname, t_lname;
        cout << "Please enter the translator details:" << endl;
        cout << "First name:";
        getline(cin, t_fname);
        cout << "Last year:";
        getline(cin, t_lname);

        string translatorCheck = "select * from translator where first_name='" + t_fname + "' and last_name='" + t_lname + "' and book_name='" + bname + "'";

        ////// Query for TRANSLATOR existence
        /*
        if (numrows == 0) {
            string insertTranslator = "INSERT INTO translator (t_fname, t_lname, book_name) VALUES('";
            insertTranslator += t_fname + "','" + t_lname + "','" + bname + "')";

            ////// INSERT PUBLISHER WITH QUERY 'insertAuthor'
        }*/
    }


    ////// CALL THE QUERY FOR THE ID

    ////// FETCH ID TO RETURN IT

}