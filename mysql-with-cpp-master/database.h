/*
 * ===================================================================
 *
 *       Filename:  database.h
 *
 *    Description:  Declaring MySQL class for using database.
 *
 *        Created:  Friday 22 February 2013 12:48:40  IST
 *       Compiler:  gcc
 *
 *         Author:  Mandeep Kaur, meghasimak@gmail.com
 *        License:  GNU General Public License
 *      Copyright:  Copyright (c) 2013, Great Developers
 *
 * ===================================================================
 */

 /**-------------------------------------------------------------------
  *  Include database-detail.h and other files
  *------------------------------------------------------------------*/

#include "header.h"
#include "database-detail.h"

  /**
   * ===================================================================
   *        Class:  MySQL
   *  Description:  MySQL class for database accessability
   * ===================================================================
   */

class MySQL
{
protected:
	/** MySQL connectivity Variables */
	MYSQL* connect;
	MYSQL_RES* res_set;
	MYSQL_ROW row;


	unsigned int i;

public:
	/** MySQL Constructor */
	MySQL();

	/* Helpful functions*/
	void printData(int cols, int spacing);
	void getRes(string& query);



	/*------------------------------------------Queries-----------------------------------------*/
	/*1. Function to show  if book is in stock*/
	void BookInStock();

	/*2. Function to show who is the oldest customer*/
	void oldestCustomer();

	/*3. Function to show oldest book in inventory*/
	void oldestInInventory();

	/*4. Function to show all active customer book orders*/
	void orderList();

	/*5. Function to show number of copies are sold of book Y*/
	void copiesSold();

	/*6. Function to show most read author in given dates*/
	void mostReadAuthor();

	/*7. Function to show the top three customers*/
	void topThreeCustomers();

	/*8. Function to show the most translated book*/
	void mostTranslatedInStock();

	/*9. Function to show what is the purchase history of customer*/
	void customerPurchaseHistory();

	/*10. Function to show what is the delivery cost */
	void customerOrderHistory();

	/*11. Function to show price of delivery*/
	void costOfDelivery();

	/*12. Function to show price of delivery*/
	void dealSplitDelivery();

	/*13. Function to show delivery status*/
	void deliveryStatus();

	/*14. Function to show delivery cost made by Xpress*/
	void deliveryMadeByXpress();

	/*15. Function to show total money paid with bit*/
	void bitPayInMonth();

	/*16. Function to show the deals in the past and that are more than avg*/
	void pastYearDealsMoreThanAvg();

	/*17. Function to show number of deliveries split by companies*/
	void pastYearDeliveryByCompany();

	/*18. Function to show deliveries inclduing more than 2 book copies*/
	void severalBookCopyDelivery();

	/*19. Function to show customer who have'nt made purchase in past two years*/
	void notActiveCustomer();

	/*20. Function to show book orders that haven't been picked up by customer*/
	void bookOrderPickUpCustomer();

	/*21. Function to show books in storage by cut */
	void monthlyCutStorage();

	/*22. Function to show book bought by store by date*/
	void storePurchaseByDate();

	/*23. Function to show book store retail in month*/
	void storeRetailInMonth();

	/*24. Function to show income in a year*/
	void yearCutIncome();

	/*25. Function to show employee monthly salary */
	void employeeSalary();

	/*26. Function to show who is the highest selling employee */
	void highestSellingEmployee();


	/** MySQL Destructor */
	~MySQL();
};
