/*
 * ===================================================================
 *
 *       Filename:  database-main.cpp
 *
 *    Description:  Main Method
 *
 *        Created:  Thursday 28 February 2013 10:53:59  IST
 *       Compiler:  gcc
 *
 *         Author:  Mandeep Kaur, meghasimak@gmail.com
 *        License:  GNU General Public License
 *      Copyright:  Copyright (c) 2013, Great Developers
 *
 * ===================================================================
 */

 /**-------------------------------------------------------------------
  *  Include required header files
  *------------------------------------------------------------------*/

#include "database.h"

  /**-------------------------------------------------------------------
	* Helping main functions
	*------------------------------------------------------------------*/
void userInput(int& input)
{	
	cout << "\nEnter you're choice: ";
	fflush(stdin);
	cin >> input;
}

void mainMenu(int& userChoice)
{
	system("cls");
	cout << "Main menu - Please type a number from the options below:" << endl;
	cout << "1. Books information" << endl;
	cout << "2. Customer information" << endl;
	cout << "3. Deals information" << endl;
	cout << "4. Deliveries information" << endl;
	cout << "5. Manager information" << endl;
	cout << "6. Exit the program" << endl;

	userInput(userChoice);
}

void booksMenu(int& userChoice, MySQL& Mysql)
{
	system("cls");
	cout << "Please select the desired option: " << endl;
	cout << "0. return to main-menu" << endl;
	cout << "1. search for book in stock" << endl;
	cout << "2. Who is  the oldest book in stock" << endl;
	cout << "3. How much copies of a book were sold" << endl;
	cout << "4. Who is the most read author in store? [Date insertion]" << endl;
	cout << "5. Most translated book in stock" << endl;
	cout << "6. Books in warehouse per month [Date insertion]" << endl;
	cout << "7. How much books our store purchased between two dates" << endl;

	userInput(userChoice);

	switch (userChoice)
	{
	case 0: return;
	case 1: Mysql.BookInStock();
		break;
	case 2: Mysql.oldestInInventory();
		break;
	case 3: Mysql.copiesSold();
		break;
	case 4: Mysql.mostReadAuthor();
		break;
	case 5: Mysql.mostTranslatedInStock();
		break;
	case 6: Mysql.monthlyCutStorage();
		break;
	case 7: Mysql.storePurchaseByDate();
		break;
	default: throw "No valid choice";
	}
	system("pause");
	userChoice = 0;
}

void customerMenu(int& userChoice, MySQL& Mysql)
{
	system("cls");
	cout << "Please select the desired option:" << endl;
	cout << "0. Return to main-menu" << endl;
	cout << "1. Oldest customer" << endl;
	cout << "2. Top 3 customers" << endl;
	cout << "3. Purchase history of customer [Customer details]" << endl;
	cout << "4. Orders history of customer [Customer details]" << endl;
	cout << "5. Splitted deliveries of customer [Customer details]" << endl;
	cout << "6. Unactive customers over the past 2 years" << endl;
	cout << "7. Customer who got informed that thier order arrived more than two weeks" << endl;
	cout << "8. All active orders" << endl;

	userInput(userChoice);

	switch (userChoice)
	{
	case 0: return;
	case 1: Mysql.oldestCustomer();
		break;
	case 2:	Mysql.topThreeCustomers();
		break;
	case 3:	Mysql.customerPurchaseHistory();
		break;
	case 4:	Mysql.customerOrderHistory();
		break;
	case 5:	Mysql.dealSplitDelivery();
		break;
	case 6: Mysql.notActiveCustomer();
		break;
	case 7: Mysql.bookOrderPickUpCustomer();
		break;
	case 8: Mysql.orderList();
		break;
	default: throw "No valid choice";
	}
	system("pause");
	userChoice = 0;

}

void dealsMenu(int& userChoice, MySQL& Mysql)
{
	system("cls");
	cout << "Please select the desired option:" << endl;
	cout << "0. return to main-menu" << endl;
	cout << "1. Deals that were made in the past 12 months and that thier profit is higher than the average" << endl;
	cout << "2. What is the store's income in a month" << endl;
	cout << "3. Total deals paid with 'BIT' in a month" << endl;

	userInput(userChoice);


	switch (userChoice) {
	case 0: return;
	case 1:	Mysql.pastYearDealsMoreThanAvg();
		break;
	case 2: Mysql.storeRetailInMonth();
		break;
	case 3: Mysql.bitPayInMonth();
		break;
	default: throw "No Valid Income";
	}
	system("pause");
	userChoice = 0;
}


void deliveryMenu(int& userChoice, MySQL& Mysql)
{
	system("cls");
	cout << "Please select the desired option:" << endl;
	cout << "0. Return to main-menu" << endl;
	cout << "1. Cost of delivery [delivery Id]" << endl;
	cout << "2. Status of delivery [delivery Id]" << endl;
	cout << "3. Total cost of deliveries made with 'Xpress' in a month [Date insertion]" << endl;
	cout << "4. Deliveries in the past year cutted by delivery companies" << endl;
	cout << "5. Deliveries that included more than one edition of some book" << endl;

	userInput(userChoice);

	switch (userChoice) {
	case 0: return;
	case 1:	Mysql.costOfDelivery();
		break;
	case 2:	Mysql.deliveryStatus();
		break;
	case 3: Mysql.deliveryMadeByXpress();
		break;
	case 4: Mysql.pastYearDeliveryByCompany();
		break;
	case 5: Mysql.severalBookCopyDelivery();
		break;
	default: throw "No Valid Income";
	}
	system("pause");
	userChoice = 0;
}

void managerMenu(int& userChoice, MySQL& Mysql)
{
	system("cls");
	cout << "Required password.." << endl;

	cin.clear();
	fflush(stdin);
	string pass;
	cin >> pass;

	if (pass == "1234")
	{
		system("cls");
		cout << "Please select the desired option:" << endl;
		cout << "0. return to main-menu" << endl;
		cout << "1. What is the salary of some employee in a month" << endl;
		cout << "2. Who is the highest selling employee in a month" << endl;

		userInput(userChoice);

		switch (userChoice)
		{
		case 0: return;
		case 1: Mysql.employeeSalary();
			break;
		case 2:	Mysql.highestSellingEmployee();
			break;
		default: throw "No valid choice";
		}
	}
	else
		throw "Incorrect manager password!";
	system("pause");
	userChoice = 0;

}


int main(void)
{
	int userChoice = 0;
	MySQL Mysql;

	while (userChoice != 6)
	{
		try
		{
			if (userChoice == 0)
				mainMenu(userChoice);
			else if (userChoice == 1)
				booksMenu(userChoice, Mysql);
			else if (userChoice == 2)
				customerMenu(userChoice, Mysql);
			else if (userChoice == 3)
				dealsMenu(userChoice, Mysql);
			else if (userChoice == 4)
				deliveryMenu(userChoice, Mysql);
			else if (userChoice == 5)
				managerMenu(userChoice, Mysql);
			else
				userChoice = 0;
		}
		catch (const char* error)
		{
			cout << error << endl;
			userChoice = 0;
			system("pause");
		}
	}

	cout << "Thanks for using our DBMS!" << endl;

	system("pause");

	return 0;
}
