/*
 * ===================================================================
 *
 *       Filename:  database.cc
 *
 *    Description:  Definition of MySQL Class for accessing database
 *
 *        Created:  Friday 22 February 2013 02:07:49  IST
 *       Compiler:  gcc
 *
 *         Author:  Mandeep Kaur, meghasimak@gmail.com
 *        License:  GNU General Public License
 *      Copyright:  Copyright (c) 2013, Great Developers
 *
 * ===================================================================
 */

 /**-------------------------------------------------------------------
  *  Include mysql.h file local header file(declaration of class)
  *------------------------------------------------------------------*/

#include "database.h"

  /**
   *--------------------------------------------------------------------
   *       Class:  MySQL
   *      Method:  MySQL :: MySQL()
   * Description:  Constructor to initialize database conectivity
   *--------------------------------------------------------------------
   */

MySQL::MySQL()
{
	connect = mysql_init(NULL);
	if (!connect)
	{
		cout << "MySQL Initialization Failed";
	}

	connect = mysql_real_connect(connect, SERVER, USER, PASSWORD, DATABASE, 0, NULL, 0);

	if (connect)
	{
		cout << "Connection Succeeded\n";
	}

	else
	{
		cout << "Connection Failed\n";
	}

	i = 0;
}

/**
 *--------------------------------------------------------------------
 *       Class:  MySQL
 *      Method:  BookInStock()
 *      Description:  1. is book X exist in the inventory?
 *--------------------------------------------------------------------
 */

void MySQL::BookInStock()
{
	string book_name, author_first_name, author_last_name;
	string query;
	int choice;

	cout << "1. search by book name" << endl;
	cout << "2.If you don't remember your book's name you can search for it by it's author name" << endl;
	cin >> choice;
	
	switch (choice)
	{
	case 1: 
		cout << "Please eneter the book's name you are searching for: " << endl;
		std::getline(cin.ignore(), book_name);
		query += "select b.*, a.first_name, a.last_name from books b inner join inventory using(book_id) inner join authors a using(book_name) where b.book_name = '" + book_name + "'";
		break;
	case 2:
		cout << "Please eneter the author first name: " << endl;
		std::getline(cin.ignore(), author_first_name);
		cout << "Please eneter the author last name: " << endl;
		std::getline(cin, author_last_name);
		query += "select b.*, a.first_name, a.last_name from books b inner join inventory using(book_id) inner join authors a using(book_name) ";
		query += "where a.first_name = '" + author_first_name + "' and a.last_name= '" +author_last_name + "'";
		break;
	default: 
		throw "No valid data for your search";
	}

	getRes(query);

	unsigned int numrows = mysql_num_rows(res_set);
	int field = mysql_num_fields(res_set);

	if (numrows == 0)
		throw "This book is not in stock";
	while ((row = mysql_fetch_row(res_set)) != NULL)
	{
		printData(field, 25);
	}
}

/*
 *--------------------------------------------------------------------
 *       Class:  MySQL
 *      Method:  oldestCustomer()
 *      Description:  1. who is the oldest customer of the store?
 *--------------------------------------------------------------------
 */
void MySQL::oldestCustomer()
{
	cout << "See the oldest customer: " << endl;

	string query = "select bk.order_date, c.customer_id, c.first_name, c.last_name from customer c inner join book_order bk using(customer_id)order by order_date limit 1";
	string query2 = "select d.deal_date, c.customer_id, c.first_name, c.last_name  from customer c inner join books_sale using (customer_id) inner join deals d using (deal_id) order by deal_date limit 1";

	getRes(query);

	unsigned int numrows = mysql_num_rows(res_set);

	row = mysql_fetch_row(res_set);

	mysql_query(connect, query2.c_str());

	res_set = mysql_store_result(connect);

	unsigned int numrows2 = mysql_num_rows(res_set);

	MYSQL_ROW row2 = mysql_fetch_row(res_set);
	int field = mysql_num_fields(res_set);


	string first_date = row[0];
	string second_date = row2[0];
	if (first_date > second_date)
		row = row2;

	printData(field, 15);
}


/*
 *--------------------------------------------------------------------
 *       Class:  MySQL
 *      Method:  oldestInInventory()
 *      Description:  3. what is the oldest book in inventory?
 *--------------------------------------------------------------------
 */

void MySQL::oldestInInventory()
{
	string query = "select b.book_id, b.book_name, date_stored  from books b inner join inventory using(book_id)order by date_stored limit 1";

	getRes(query);

	unsigned int numrows = mysql_num_rows(res_set);
	row = mysql_fetch_row(res_set);
	int field = mysql_num_fields(res_set);

	cout << "The oldest book in the inventory is:  " << endl;

	printData(field, 15);
	cout << endl;


}

/*
 *--------------------------------------------------------------------
 *       Class:  MySQL
 *      Method:  orderList()
 *      Description: 4. current active orders?
 *--------------------------------------------------------------------
 */
void MySQL::orderList()
{
	string query = "select first_name, last_name, book_name, order_date ,informed_date from book_order ";
	query += "inner join books using(book_id) ";
	query += "inner join customer using(customer_id) where isnull(informed_date) order by order_date";

	getRes(query);

	unsigned int numrows = mysql_num_rows(res_set);
	int field = mysql_num_fields(res_set);
	cout << "current active orders are:  " << endl;

	while (((row = mysql_fetch_row(res_set)) != NULL))
	{
		printData(field-1, 30);
	}
}

/**
 *--------------------------------------------------------------------
 *       Class:  MySQL
 *      Method:  copiesSold()
 *      Description:  5. How many copies of book name (some Y) sold by book store
 *--------------------------------------------------------------------
 */

void MySQL::copiesSold()
{
	string book_name;
	cout << "Please eneter the book's name you are searching for: " << endl;
	std::getline(cin.ignore(), book_name);

	string query = "select count(b.book_name) as sold_copies from books b inner join books_sale bs using (book_id)";
	query += "group by b.book_name having book_name = '" + book_name + "'";


	getRes(query);

	unsigned int numrows = mysql_num_rows(res_set);

	if (numrows == 0)
		throw "No copies with that book name sold";

	int field = mysql_num_fields(res_set);

	while (((row = mysql_fetch_row(res_set)) != NULL))
	{
		printData(field, 15);
	}
}

/**
 *--------------------------------------------------------------------
 *       Class:  MySQL
 *      Method:  mostReadAuthor()
 *      Description:  6. Most read author between X and Y dates
 *--------------------------------------------------------------------
 */

void MySQL::mostReadAuthor()
{
	string date1, date2;
	cout << "From date ('YYYY-MM-DD'): ";
	std::getline(cin.ignore(), date1);
	cout << "To date ('YYYY-MM-DD'): ";
	fflush(stdin);
	std::getline(cin, date2);

	string query = "select count(a.first_name) most_read , a.first_name, a.last_name from books b ";
	query += "inner join authors a using (book_name) inner join books_sale using(book_id) inner join deals ";
	query += "using(deal_id) where deal_date between '" + date1 + "' and '" + date2 + "' group by a.first_name order by most_read desc limit 1";

	getRes(query);

	unsigned int numrows = mysql_num_rows(res_set);

	if (numrows == 0)
		throw  "No results for given dates";

	int field = mysql_num_fields(res_set);

	row = mysql_fetch_row(res_set);
	printData(field, 25);

}

/**
 *--------------------------------------------------------------------
 *       Class:  MySQL
 *      Method:  topThreeCustomers()
 * Description:  7. Who are the top three customer in book buying?
 *--------------------------------------------------------------------
 */

void MySQL::topThreeCustomers()
{
	string query = "select c.first_name, c.last_name, count(customer_id) top_customer from customer c ";
	query += "inner join books_sale bs using (customer_id) group by customer_id order by top_customer desc  limit 3";


	getRes(query);

	unsigned int numrows = mysql_num_rows(res_set);
	int field = mysql_num_fields(res_set);
	cout << "The store's top three customers over the years are: " << endl;

	while (((row = mysql_fetch_row(res_set)) != NULL))
	{
		printData(field, 15);
	}
}

/**
 *--------------------------------------------------------------------
 *       Class:  MySQL
 *      Method:  mostTranslatedInStock()
 * Description:  8. What is the most tranaslated book in storage?
 *--------------------------------------------------------------------
 */

void MySQL::mostTranslatedInStock()
{
	string query = "select t.book_name ,count(book_name) Translations from translator t inner join books b ";
	query += "using(book_name) inner join inventory using (book_id) group by t.first_name, t.last_name order by Translations desc limit 1";

	getRes(query);

	unsigned int numrows = mysql_num_rows(res_set);
	int field = mysql_num_fields(res_set);

	while (((row = mysql_fetch_row(res_set)) != NULL))
	{
		printData(field, 15);
	}
}

/**
 *--------------------------------------------------------------------
 *      Class:  MySQL
 *      Method:  customerPurchaseHistory()
 *      Description:  9.what is the purchase history of customer X
 *--------------------------------------------------------------------
 */

void MySQL::customerPurchaseHistory()
{
	string fname, lname;
	cout << "First name: ";
	std::getline(cin.ignore(), fname);
	cout << "Last name: ";
	std::getline(cin, lname);

	string query = "select c.customer_id, b.book_name, d.deal_date, bs.book_price from books_sale bs ";
	query += "inner join customer c using(customer_id) inner join books b using(book_id) inner join deals d ";
	query += "using(deal_id) where c.first_name = '" + fname + "' and c.last_name = '" + lname + "' order by deal_date desc";

	getRes(query);

	unsigned int numrows = mysql_num_rows(res_set);
	if (numrows == 0) {
		cout << "No purchase history for " << fname << " " << lname << endl;
		return;
	}

	int field = mysql_num_fields(res_set);
	cout << "The purchase history of " << fname << " " << lname << ": " << endl;

	while (((row = mysql_fetch_row(res_set)) != NULL))
	{
		printData(field, 30);
	}
}


/*
 *--------------------------------------------------------------------
 *      Class:  MySQL
 *      Method: customerOrderHistory()
 *      Description:  10.what is the order history of customer?
 *--------------------------------------------------------------------
 */

void MySQL::customerOrderHistory()
{
	string fname, lname;
	cout << "First name: ";
	std::getline(cin.ignore(), fname);
	cout << "Last name: ";
	std::getline(cin, lname);

	string query = " select b.book_id ,b.book_name,bo.order_date, i.date_stored ,bs.book_price  from customer c ";
	query += "inner join book_order bo using(customer_id) left join inventory i using(book_id) inner join books b using(book_id)";
	query += "left join books_sale bs using(book_id)";
	query += "where c.first_name = '" + fname + "' and c.last_name = '" + lname + "' order by order_date";

	getRes(query);
	unsigned int numrows = mysql_num_rows(res_set);

	if (numrows == 0)
		throw "No orders were made by this customer";


	int field = mysql_num_fields(res_set);
	cout << "The order history of " << fname << " " << lname << ": " << endl;

	while (((row = mysql_fetch_row(res_set)) != NULL))
	{
		if (row[3] == NULL)
			row[3] = "NOT IN STORE";

		if (row[4] == NULL)
			row[4] = "NOT SOLD YET";

		printData(field, 30);
	}
}





/*
 *--------------------------------------------------------------------
 *      Class:  MySQL
 *      Method: costOfDelivery()
 *      Description:  11.what is the cost of delivery?
 *--------------------------------------------------------------------
 */

void MySQL::costOfDelivery() {
	string del_id;

	cout << "Please enter delivery number: " << endl;
	std::getline(cin.ignore(), del_id);

	string query = "select company, sum(b.weight) as total_weight, sp.pay_rate, sum(b.weight)*pay_rate as price from books b ";
	query += "inner join books_sale bs using(book_id) inner join delivery d using(delivery_id) inner join shipping_rates sp using(shipping_method) ";
	query += "where delivery_id = '" + del_id + "' group by delivery_id";

	getRes(query);

	unsigned int numrows = mysql_num_rows(res_set);

	if (numrows == 0)
		throw "No delivery for that id";

	int field = mysql_num_fields(res_set);

	while (((row = mysql_fetch_row(res_set)) != NULL))
	{
		printData(field, 20);
	}

}


/*
 *--------------------------------------------------------------------
 *      Class:  MySQL
 *      Method: dealSplitDelivery()
 *      Description: 12. splitted deliveries in a deal of customer X
 *--------------------------------------------------------------------
 */

void MySQL::dealSplitDelivery() {
	string fname, lname;
	cout << "First name: ";
	std::getline(cin.ignore(), fname);
	cout << "Last name: ";
	std::getline(cin, lname);

	string query = "select d.*, da.city, da.street from (select c.last_name,c.first_name,bs.deal_id, count(deal_id) multy_shipping from delivery ";
	query += "inner join books_sale bs using(delivery_id) inner join customer c using(customer_id) group by deal_id ";
	query += "having multy_shipping > 1 and c.last_name = '" + lname + "' and first_name = '" + fname + "' ) AS mid_tb inner join books_sale bs ";
	query += "using(deal_id) inner join delivery d using(delivery_id) inner join delivery_address da using(delivery_id) group by delivery_id";

	getRes(query);

	unsigned int numrows = mysql_num_rows(res_set);

	if (numrows == 0)
		throw "No splitted deliveries for that customer";

	int field = mysql_num_fields(res_set);

	while (((row = mysql_fetch_row(res_set)) != NULL))
		printData(field, 22);

}

/*
 *--------------------------------------------------------------------
 *      Class:  MySQL
 *      Method: dealSplitDelivery()
 *      Description:  13.splitted deliveries in a deal of customer X
 *--------------------------------------------------------------------
 */

void MySQL::deliveryStatus() {

	string del_id;
	cout << "Please enter delivery number: " << endl;
	std::getline(cin.ignore(), del_id);

	string query = "select shipping_status, shipping_method as Status from delivery where delivery_id =" + del_id;

	getRes(query);

	unsigned int numrows = mysql_num_rows(res_set);

	if (numrows == 0) {
		cout << "No Status for " << del_id << " id" << endl;
		return;
	}

	int field = mysql_num_fields(res_set);

	while (((row = mysql_fetch_row(res_set)) != NULL))
	{
		printData(field, 18);
	}

}


/*
 *--------------------------------------------------------------------
 *      Class:  MySQL
 *      Method: deliveryMadeByXpress()
 *      Description: 14.what is the price of delivery made by Xpress in a specific month?
 *--------------------------------------------------------------------
 */

void MySQL::deliveryMadeByXpress() {

	string month, year;
	cout << "Enter Year('YYYY'): ";
	std::getline(cin.ignore(), year);
	cout << "Enter Month('MM'): ";
	std::getline(cin, month);


	string query = "select sum(sr.pay_rate * b.weight) as total_pay from books b ";
	query += "inner join books_sale bs using(book_id) inner join delivery d using(delivery_id) ";
	query += "inner join shipping_rates sr using(shipping_method) ";
	query += "where company = 'Xpress' and month(delivery_date) = '" + month + "' and year(delivery_date) = '" + year + "'";

	getRes(query);

	if (res_set == NULL)
		throw "Not a valid input";

	unsigned int numrows = mysql_num_rows(res_set);

	int field = mysql_num_fields(res_set);

	while (((row = mysql_fetch_row(res_set)) != NULL))
	{
		if (row[0] == NULL) {
			throw "No delivries made by 'Xpress' in this month";
		}
		printData(field, 10);
	}

}


/*
 *--------------------------------------------------------------------
 *      Class:  MySQL
 *      Method: deliveryMadeByXpress()
 *      Description: 15.what is the price of delivery made by Xpress in a specific month?
 *--------------------------------------------------------------------
 */

void MySQL::bitPayInMonth() {
	string month, year;
	cout << "Enter Year('YYYY'): ";
	std::getline(cin.ignore(), year);
	cout << "Enter Month('MM'): ";
	std::getline(cin, month);

	string query = "select count(*) deals_number,sum(total_pay) bit_pay from deals ";
	query += " where payment_method = 'bit' and month(deal_date) = " + month + " and year(deal_date) = " + year;

	getRes(query);

	if (res_set == NULL)
		throw "Not a valid input";

	unsigned int numrows = mysql_num_rows(res_set);

	if (numrows == 0)
		throw "No deals were paid with 'BIT' in this month";

	int field = mysql_num_fields(res_set);

	while (((row = mysql_fetch_row(res_set)) != NULL))
	{
		if (row[0] == NULL)
			row[0] = "0";
		printData(field, 18);
	}

}


/*
 *--------------------------------------------------------------------
 *      Class:  MySQL
 *      Method: pastYearDeals()
 *      Description: 16.what are the deals that earned more than the avarage in the past year?
 *--------------------------------------------------------------------
 */


void MySQL::pastYearDealsMoreThanAvg() {

	string query = "select d.deal_id, d.deal_date, sum(bs.book_price) Profit from deals d ";
	query += "inner join books_sale bs using(deal_id) inner join books b using(book_id) group by deal_id ";
	query += "having d.deal_date between DATE_SUB(current_date(), INTERVAL 12 MONTH) and current_date()";

	getRes(query);
	unsigned int numrows = mysql_num_rows(res_set);

	int avg = 0;

	while (((row = mysql_fetch_row(res_set)) != NULL))
	{
		avg += atoi(row[2]);
	}

	avg = (avg / numrows);

	getRes(query);

	int field = mysql_num_fields(res_set);
	cout << "\n The average profit in the past year is: " << avg << endl;
	while (((row = mysql_fetch_row(res_set)) != NULL))
	{
		if (atoi(row[2]) > avg)
			printData(field, 15);
	}
}


/*
 *--------------------------------------------------------------------
 *      Class:  MySQL
 *      Method: pastYearDeliveryByCompany()
 *      Description: 17.how many deliveries occured in the past year splitted by companies?
 *--------------------------------------------------------------------
 */

void MySQL::pastYearDeliveryByCompany() {

	string query = "select count(company) distribution, company from delivery inner join shipping_rates using(shipping_method) ";
	query += "where delivery_date between DATE_SUB(current_date(), INTERVAL 12 MONTH) and current_date() group by(company) ";

	getRes(query);
	unsigned int numrows = mysql_num_rows(res_set);
	int field = mysql_num_fields(res_set);

	if (numrows == 0)
		throw "No shippings were made in the past year";


	while (((row = mysql_fetch_row(res_set)) != NULL))
	{
		printData(field, 20);
	}

}


/*
 *--------------------------------------------------------------------
 *      Class:  MySQL
 *      Method: severalBookCopyDelivery()
 *      Description: 18.how many deliveries have had more than one copy of the same book in them?
 *--------------------------------------------------------------------
 */

void MySQL::severalBookCopyDelivery() {

	string query = "select b.book_id, b.book_name, d.delivery_id, brand_name as publisher, publish_year, t.first_name, t.last_name from books_sale bs ";
	query += "inner join delivery d using(delivery_id) inner join books b using(book_id) inner join publisher using(book_name) ";
	query += "left join translator t using(book_name) order by delivery_id, book_name ";

	getRes(query);
	unsigned int numrows = mysql_num_rows(res_set);
	int field = mysql_num_fields(res_set);

	row = mysql_fetch_row(res_set);
	int delivery_id = atoi(row[2]);
	string book_name = row[1];
	string publisher = row[3];
	string publish_year = row[4];
	string first_name = "";
	string last_name = "";

	if (row[5] != NULL) {
		first_name = row[5];
		last_name = row[6];
	}
	vector<int> dels;
	string firstname_comp = "";
	string lastname_comp = "";

	while (((row = mysql_fetch_row(res_set)) != NULL))
	{
		int id_comp = atoi(row[2]);
		string bookname_comp = row[1];
		string publisher_comp = row[3];
		string publishyear_comp = row[4];
		if (row[5] != NULL)
		{
			firstname_comp = row[5];
			lastname_comp = row[6];
		}
		else
		{
			firstname_comp = "";
			lastname_comp = "";
		}

		if (delivery_id == id_comp && bookname_comp == book_name)
		{
			if (publisher != publisher_comp || publish_year != publishyear_comp || first_name != firstname_comp || last_name != lastname_comp)
				if ( dels.size() == 0 || *(dels.end()-1) != delivery_id)
					dels.push_back(delivery_id);
		}

		book_name = bookname_comp;
		delivery_id = id_comp;
		publisher = publisher_comp;
		publish_year = publishyear_comp;
		first_name = firstname_comp;
		last_name = lastname_comp;
	}

	string quer;
	if (dels.size() > 0)
	{
		quer = " select * from delivery inner join delivery_address using(delivery_id) where delivery_id = ";

		for (int j = dels.size() - 1; j >= 1; j--)
		{
			quer += std::to_string(dels[j]) ;
			quer += " or delivery_id = ";
		}

		quer += std::to_string(dels[0]);
	}

	else
		throw "No deliveries containing more than one copy of any book";


	getRes(quer);

	while (((row = mysql_fetch_row(res_set)) != NULL))
	{

		printData(field, 22);
	}

}

/*
 *--------------------------------------------------------------------
 *      Class:  MySQL
 *      Method: notActiveCustomer()
 *      Description: 19.who are the customers who haven't made deals in the past 2 years?
 *--------------------------------------------------------------------
 */

void MySQL::notActiveCustomer()
{
	string query = "select c.* from customer c inner join books_sale using(customer_id) inner join deals d using(deal_id) ";
	query += "where c.customer_id  not in(select c.customer_id from books_sale inner join customer c using(customer_id) ";
	query += "inner join deals d using(deal_id) where d.deal_date between  ";
	query += "DATE_SUB(current_date(), INTERVAL 2 YEAR) and current_date()) group by c.customer_id ";

	getRes(query);

	unsigned int numrows = mysql_num_rows(res_set);

	if (numrows == 0)
		throw "Error finding data";

	int field = mysql_num_fields(res_set);

	while (((row = mysql_fetch_row(res_set)) != NULL))
		printData(field, 15);

}


/*
 *--------------------------------------------------------------------
 *      Class:  MySQL
 *      Method: bookOrderPickUpCustomer()
 *      Description: 20.who are the customers that got  informed one arriving orders more than two weeks and havn't bout the book?
 *--------------------------------------------------------------------
 */


void MySQL::bookOrderPickUpCustomer() {

	string query = "select c.*, b.book_name, bo.book_id, bo.informed_date from book_order bo inner join books b using(book_id) ";
	query += "inner join inventory using(book_id) inner join customer c using(customer_id) ";
	query += "where informed_date < DATE_SUB(current_date(), INTERVAL 14 DAY) order by bo.informed_date ";

	getRes(query);

	unsigned int numrows = mysql_num_rows(res_set);

	if (numrows == 0) {
		cout << "No Orders match your search " << endl;
		return;
	}

	int field = mysql_num_fields(res_set);

	while (((row = mysql_fetch_row(res_set)) != NULL))
	{
		printData(field, 20);
	}

}



/*
 *--------------------------------------------------------------------
 *      Class:  MySQL
 *      Method: monthlyCutStorage()
 *      Description: 21. How many books are in the warehouse in specific month??
 *--------------------------------------------------------------------
 */

void MySQL::monthlyCutStorage()
{
	string month, year;
	cout << "Enter Year('YYYY'): ";
	std::getline(cin.ignore(), year);
	cout << "Enter Month('MM'): ";
	std::getline(cin, month);

	string query = "select count(*) from warehouse ";
	query += "where (((year(date_in) = '" + year + "' and month(date_in) <= '" + month + "' ) or year(date_in) < '" + year + "') ";
	query += "and (((year(date_out) = '" + year + "' and month(date_out) >= '" + month + " ') or year(date_out) > '" + year + "') or isnull(date_out)))";

	getRes(query);

	row = mysql_fetch_row(res_set);

	cout << "Number of books at the warehouse in " << year << "-" << month << " is: " << row[0] << endl;;

}



/*
 *--------------------------------------------------------------------
 *      Class:  MySQL
 *      Method: storePurchaseByDate()
 *      Description: 22.how many books did the store bought between two months and how much did it cost?
 *--------------------------------------------------------------------
 */


void MySQL::storePurchaseByDate()
{
	string date1, date2;
	cout << "From date ('YYYY-MM-DD'): ";
	std::getline(cin.ignore(), date1);
	cout << "To date ('YYYY-MM-DD'): ";
	std::getline(cin, date2);

	string query = "select count(*) book_purchase, sum(price) total_price from store_purchase ";
	query += "where purchase_date>= '" + date1 + "' and purchase_date<= '" + date2 + "'";

	getRes(query);

	if (res_set == NULL)
		throw "Not a valid input";

	row = mysql_fetch_row(res_set);

	if (row[1] == NULL)
		row[1] = "0";

	int field = mysql_num_fields(res_set);

	printData(field, 20);

}



/*
 *--------------------------------------------------------------------
 *      Class:  MySQL
 *      Method: storeRetailInMonth()
 *      Description: 23.what is the store retail in specific month?
 *--------------------------------------------------------------------
 */

void MySQL::storeRetailInMonth()
{
	string month, year;
	cout << "Enter Year('YYYY'): ";
	std::getline(cin.ignore(), year);
	cout << "Enter Month('MM'): ";
	std::getline(cin, month);

	string query = "select sum(bs.book_price - sp.price) store_income, month(deal_date) month, year(deal_date) year from books_sale bs ";
	query += "inner join deals using(deal_id) inner join store_purchase sp using(book_id) ";
	query += "where month(deal_date) = '" + month + "'and year(deal_date) = '" + year + "'";

	getRes(query);

	int numrows = mysql_num_rows(res_set);
	
	if (numrows == 0)
		throw "No match for inserted date";

	int field = mysql_num_fields(res_set);
	row = mysql_fetch_row(res_set);
	printData(field, 15);
}

/*
*--------------------------------------------------------------------
* Class:  MySQL
* Method : yearCutIncome()
* Description : 24.yearly income based by months
*--------------------------------------------------------------------
*/

void MySQL::yearCutIncome()
{
	string month, year;
	cout << "Enter Year('YYYY'): ";
	std::getline(cin.ignore(), year);

	string query = "select count(*) number_of_deals, sum(total_pay)/(12) year_avg from deals ";
	query += "where year(deal_date) = '" + year + "'";

	getRes(query);


	int field = mysql_num_fields(res_set);
	row = mysql_fetch_row(res_set);
	printData(field, 20);


}



/*
*--------------------------------------------------------------------
* Class:  MySQL
* Method: employeeSalay()
* Description : 25.yearly income based by months
*--------------------------------------------------------------------
*/

void MySQL::employeeSalary()
{
	string month, year, emp_detail;
	cout << "Enter Year('YYYY'): ";
	std::getline(cin.ignore(), year);
	cout << "Enter Month('MM'): ";
	std::getline(cin, month);
	cout << "Enter Employee's first name or employee id: ";
	std::getline(cin, emp_detail);



	string query = "select emp_id id, pay_date, total_hours, first_name, last_name, total_hours*30 salary from salaries inner join employee using(emp_id) ";
	query += "where month(pay_date) = '" + month + "' and year(pay_date) = '" + year;
	query += "' and (emp_id = '" + emp_detail + "' or first_name = '" + emp_detail + "')";


	getRes(query);

	int field = mysql_num_fields(res_set);
	int numrows = mysql_num_rows(res_set);

	if (numrows == 0)
		throw "No employee matchs your search";

	while ((row = mysql_fetch_row(res_set)) != NULL)
	{
		printData(field, 15);
	}
}


/*
*--------------------------------------------------------------------
* Class:  MySQL
* Method: highestSellingEmployee()
* Description :26.yearly income based by months
*--------------------------------------------------------------------
*/

void MySQL::highestSellingEmployee()
{

	string month, year, emp_detail;
	cout << "Enter Year('YYYY'): ";
	std::getline(cin.ignore(), year);
	cout << "Enter Month('DD'): ";
	std::getline(cin, month);

	string query = "select emp_id, first_name, last_name,count(emp_id) sales_number from employee ";
	query += "inner join sales using(emp_id) inner join deals using(deal_id) ";
	query += "where month(deal_date) = '" + month + "'and year(deal_date) = '" + year + "' group by emp_id limit 1";

	getRes(query);

	int numrows = mysql_num_rows(res_set);

	if (numrows == 0)
		throw "No employee matchs your search";

	int field = mysql_num_fields(res_set);


	while ((row = mysql_fetch_row(res_set)) != NULL)
	{
		printData(field, 15);
	}
}



/**
 *--------------------------------------------------------------------
 *       Class:  MySQL
 *      Method:  MySQL :: ~MySQL()
 * Description:  Destructor of MySQL class for closing mysql
 *               connection
 *--------------------------------------------------------------------
 */

MySQL :: ~MySQL()
{
	mysql_close(connect);
}

/*
 *--------------------------------------------------------------------
 *      Class:  MySQL
 *      Method: printData()
 *      Description:  printing the data for a single row
 *      cols => num of fields in row
 *--------------------------------------------------------------------
 */

void MySQL::printData(int cols, int spacing = 5)
{
	const char separator = ' ';
	MYSQL_FIELD* _fieldset;
	int count = 0;
	cout << endl;
	while ((_fieldset = mysql_fetch_field(res_set)) != NULL && count++ < cols)
	{
		string build = "[";
		build += _fieldset->name;
		build += "]";
		cout << left << setw(spacing) << setfill(separator) << build;
	}

	cout << endl;

	for (int j = 0; j < cols; j++)
	{
		if (row[j] != NULL)
			cout << left << setw(spacing) << setfill(separator) << row[j];
		else
			cout << left << setw(spacing) << setfill(separator) << "- ";
	}

	cout << endl;

}

/*
 *--------------------------------------------------------------------
 *      Class:  MySQL
 *      Method:  getRes()
 *      Description: setting res for all queries
 *--------------------------------------------------------------------
 */

void MySQL::getRes(string& query) {

	mysql_query(connect, query.c_str());

	res_set = mysql_store_result(connect);
}