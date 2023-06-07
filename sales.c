#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<Windows.h>
#pragma warning(disable:4996)

typedef struct {
	int day;
	int month;
	int year;
}Date;

typedef struct {
	char salesOrderID[10];
	char staffID[10];
	char memberID[10];
	char itemCode[10];
	double price;
	int quantity;
	double total;
	double taxAmount;
	double grandTotal;
	double commission;
	char paymentMethod;
	Date salesDate;
}Sales;

typedef struct {
	char itemCode[5];
	char description[41];
	double itemPrice;
	int stockQty;
	int minStock;
	int reorderQty;
	Date lastRestock;
	char staffID[5];
}Stock;

typedef struct {
	char staffID[6];
	char name[30];
	char gender;
	char password[8];
	char staffic[13];
	double salary;
	int workminute;
	Date indate;
}Staff;

typedef struct {
	char memberID[10];
	char name[60];
	char gender;
	char IC[16];
	char contact[15];
	char upLineID[10];
	char referral[10];
	int numOfPurchase;
}Member;

//main function
void salesMenu();
void addSales(Sales* sales, int* totalSales);
void searchSales(Sales* sales, int totalSales);
void modifySales(Sales* sales, int totalSales);
void displaySales(Sales* sales, int totalSales);
void deleteSales(Sales* sales, int* totalSales);
void salesReport(Sales* sales, int totalSales);

//display
void headingDisplay();
void tableLineSales(int totalSales);
void headingSales(int type);

//append, read and write file
void appendSales(Sales* appendSales, int totalSales);
int readSales(Sales* sales);
void writeSales(Sales* writeSales, int totalSales);

//validation
int salesIdUnique(Sales* sales, int totalSales, char id[5]);
int idValidation(char id[6], char idChar[3], char idChecked[]);
int intValidation(char dataField[], int num, int min, int max);
int dateValidation(int day, int month, int year);
int yearValidation(int year);
char decision();

//check stock quantity, stock price, staff id and member id validation
int isExistingItem(char itemCode[6], Stock* stock, int totalStock);
int isMinimumLevel(char itemCode[6], Stock* stock, int totalStock);
int isExistingMember(char memberID[6], Member* member, int member_no);
int isExistingStaff(char staffID[6], Staff* staff, int staffNo);
int checkAvailableStock(int quantity, char itemCode[6], Stock* stock, int totalStock);
void updateStock(int quantity, char itemCode[6], Stock* stock, int totalStock);

//read staff
int readStaff(Staff* staff);
//write stock
void rewriteStock(Stock* rewriteStock, int totalStock);
//read member
int readMember(Member* member);
//read stock
int readStock(Stock* stock);


void salesMenu() {
	Sales sales[100];
	int select, num_items;
	char check[45];

	do {
		num_items = readSales(sales);
		printf("=================================\n");
		printf("|             SALES             |\n");
		printf("|-------------------------------|\n");
		printf("| 1) Add sales record           |\n");
		printf("|-------------------------------|\n");
		printf("| 2) Search for sales record    |\n");
		printf("|-------------------------------|\n");
		printf("| 3) Modify sales record        |\n");
		printf("|-------------------------------|\n");
		printf("| 4) Display sales records      |\n");
		printf("|-------------------------------|\n");
		printf("| 5) Delete sales record        |\n");
		printf("|-------------------------------|\n");
		printf("| 6) Sales Report		|\n");
		printf("|-------------------------------|\n");
		printf("| 7) Exit the sales function    |\n");
		printf("=================================\n");
		printf("Select one of the options (1 - 7): ");

		//Invalid data type check
		if (scanf("%d", &select) != 1) {
			scanf("%[^\n]", &check);
			select = 8;
			printf("\nError: '%s' is not an integer! Please enter a value of 1 - 7 only.\n\n", check);
		}
		//Out of range
		else if (select < 1 || select > 7) {
			printf("\nError: '%d' is not an option! Please enter a value of 1 - 7 only.\n\n", select);
		}

		switch (select) {
		case 1:
			system("cls");
			addSales(sales, &num_items);
			break;
		case 2:
			system("cls");
			searchSales(sales, num_items);
			break;
		case 3:
			modifySales(sales, num_items);
			break;
		case 4:
			system("cls");
			displaySales(sales, num_items);
			system("pause");
			system("cls");
			break;
		case 5:
			deleteSales(sales, &num_items);
			break;
		case 6:
			system("cls");
			salesReport(sales, num_items);
			break;
		case 7:
			break;
		}

		printf("\n");
	} while (select != 7);

	printf("Thank you for using!\n\n");
	system("pause");
}
void addSales(Sales* sales, int* totalSales) {
	Stock stock[99];
	Member member[99];
	Staff staff[99];
	int checkID, totalStock, member_no, staffNo;
	char addInfo, confirm;
	double tax = 0.06;
	double com = 0.05;

	do {
		system("cls");
		headingSales(1);
		printf("\n-------- Enter sales %d details --------\n", *totalSales + 1);

		//read stock file
		totalStock = readStock(stock);
		//read member file
		member_no = readMember(member);
		//read staff file
		staffNo = readStaff(staff);

		//sales id
		do {
			printf("\nEnter Sales Order ID [S001]: ");
			rewind(stdin);
			scanf("%s", sales[*totalSales].salesOrderID);
			//validation
			checkID = idValidation(sales[*totalSales].salesOrderID, "S", "Sales Order ID");
			if (checkID == 1) {
				checkID = salesIdUnique(sales, *totalSales, sales[*totalSales].salesOrderID);
			}
		} while (checkID == 0);

		//staff id
		do {
			printf("\nEnter Staff ID [SF001]: ");
			rewind(stdin);
			scanf("%s", sales[*totalSales].staffID);
		} while (idValidation(sales[*totalSales].staffID, "SF", "Staff ID") == 0 || isExistingStaff(sales[*totalSales].staffID, staff, staffNo) == 0);

		//Member ID
		do {
			printf("\nEnter Member ID [M101]: ");
			rewind(stdin);
			scanf("%s", sales[*totalSales].memberID);
		} while (idValidation(sales[*totalSales].memberID, "M", "Member ID") == 0 || isExistingMember(sales[*totalSales].memberID, member, member_no) == 0);

		do {
			//Product ID
			do {
				printf("\nEnter Item Code [P001]: ");
				rewind(stdin);
				scanf("%s", sales[*totalSales].itemCode);
			} while (idValidation(sales[*totalSales].itemCode, "P", "Item Code") == 0 || isExistingItem(sales[*totalSales].itemCode, stock, totalStock) == 0
				|| isMinimumLevel(sales[*totalSales].itemCode, stock, totalStock) == 0);

			//price
			int updatedPrice = 0;
			//Auto enter sales price based on stock module item price
			for (int i = 0; i < totalStock; i++) {
				if (strcmp(sales[*totalSales].itemCode, stock[i].itemCode) == 0) {
					sales[*totalSales].price = stock[i].itemPrice;
					updatedPrice++;
				}
			}
			//If price could not be found
			if (updatedPrice == 0) {
				printf("\nAn error has occurred, the price for the item could not be found. Exiting the add sales function...\n\n");
				system("pause");
				exit(-1);
			}

			//Sales quantity
			do {
				printf("\nEnter Sales Quantity: ");
				rewind(stdin);
				scanf("%d", &sales[*totalSales].quantity);
			} while (intValidation("sales quantity", sales[*totalSales].quantity, 1, 20) == 0);
		} while (checkAvailableStock(sales[*totalSales].quantity, sales[*totalSales].itemCode, stock, totalStock) == 0);
		updateStock(sales[*totalSales].quantity, sales[*totalSales].itemCode, stock, totalStock);

		//Total
		sales[*totalSales].total = sales[*totalSales].price * sales[*totalSales].quantity;

		//Taxed amount
		sales[*totalSales].taxAmount = sales[*totalSales].total * tax;

		//Grand total
		sales[*totalSales].grandTotal = sales[*totalSales].total + sales[*totalSales].taxAmount;

		//Commission
		sales[*totalSales].commission = sales[*totalSales].grandTotal * com;

		//Payment method
		printf("\nEnter Payment Method [Cash = C /Online = O]: ");
		rewind(stdin);
		scanf("%c", &sales[*totalSales].paymentMethod);
		sales[*totalSales].paymentMethod = toupper(sales[*totalSales].paymentMethod);
		//validation
		while (sales[*totalSales].paymentMethod != 'C' && sales[*totalSales].paymentMethod != 'O') {
			printf("\nInvalid! Please key in accordingly");
			printf("\nEnter Payment Method [Cash = C /Online = O]: ");
			rewind(stdin);
			scanf("%c", &sales[*totalSales].paymentMethod);
			sales[*totalSales].paymentMethod = toupper(sales[*totalSales].paymentMethod);
		}

		//Sales date
		do {
			printf("\nEnter Sales Date (DD/MM/YYYY): ");
			rewind(stdin);
			scanf("%d/%d/%d", &sales[*totalSales].salesDate.day, &sales[*totalSales].salesDate.month, &sales[*totalSales].salesDate.year);
		} while (dateValidation(sales[*totalSales].salesDate.day, sales[*totalSales].salesDate.month, sales[*totalSales].salesDate.year) == 0);

		//adding info
		do {
			printf("\nConfirm to add details? [Y/N]: ");
			rewind(stdin);
			scanf("%c", &addInfo);
			addInfo = toupper(addInfo);
			if (addInfo != 'N' && addInfo != 'Y')
				printf("\nInvalid! Please key in accordingly\n");
		} while (addInfo != 'N' && addInfo != 'Y');

		//add
		if (addInfo == 'Y') {
			appendSales(sales, *totalSales);
			(*totalSales)++;
			printf("\nItem added successfully\n");
		}
		//cancel adding
		else {
			sales[*totalSales] = sales[*totalSales + 1];
			printf("\nCancelled adding item\n");
		}

		//confirmation
		printf("Continue add sales info? [Y/N]: ");
		confirm = decision();
	} while (confirm != 'N');

	system("cls");
	printf("\nYou have added %d sales info\n", *totalSales);
}
void displaySales(Sales* sales, int num) {
	system("cls");

	headingSales(2);
	headingDisplay();

	for (int no = 0; no < num; no++) {
		printf("|%-3d|%-9s|%-9s|%-9s|%-9s|RM%7.2f   |%-7d |RM%7.2f  |RM%7.2f   |RM%7.2f  |RM%7.2f |%-10c    |%02d/%02d/%4d |\n", no + 1, sales[no].salesOrderID,
			sales[no].staffID, sales[no].memberID, sales[no].itemCode, sales[no].price, sales[no].quantity, sales[no].total, sales[no].taxAmount,
			sales[no].grandTotal, sales[no].commission, sales[no].paymentMethod, sales[no].salesDate.day, sales[no].salesDate.month, sales[no].salesDate.year);
	}
	tableLineSales(141);
}
void searchSales(Sales* sales, int totalSales) {
	Sales searchSalesResult[99];
	char ID[11], confirm;
	int count = 0, checkID;

	do {
		system("cls");
		headingSales(3);
		printf("\n-------- Enter sales details --------\n");

		do {
			printf("\nEnter sales id [S001]: ");
			rewind(stdin);
			scanf("%[^\n]", ID);
			checkID = idValidation(ID, "S", "Sales ID");
		} while (checkID == 0);

		for (int i = 0; i < totalSales; i++) {
			if (strcmp(ID, sales[i].salesOrderID) == 0) {
				searchSalesResult[count] = sales[i];
				count++;
			}
		}

		displaySales(searchSalesResult, count);

		if (count == 0)
			printf("\nNo matching records found!\n");
		else
			printf("\nFound %d matching records!\n", count);
		count = 0;

		printf("\n\n");
		printf("Continue search sales info? [Y/N]: ");
		confirm = decision();
		system("cls");
	} while (confirm != 'N');
}
void modifySales(Sales* sales, int totalSales) {
	Sales modifyS;
	Stock stock[99];
	char ID[11], confirm, method;
	int modifyNum = 0, choice, checkID, totalStock;
	double tax = 0.06;
	double com = 0.05;

	do {
		totalStock = readStock(stock);
		system("cls");
		headingSales(4);
		do {
			printf("\nEnter sales id [S001]: ");
			rewind(stdin);
			scanf("%[^\n]", ID);
			checkID = idValidation(ID, "S", "Sales ID");
		} while (checkID == 0);

		system("cls");

		//search for modify
		do {
			headingSales(7);
			headingDisplay();

			for (int i = 0; i < totalSales; i++) {
				if (strcmp(sales[i].salesOrderID, ID) == 0) {
					tableLineSales(141);
					printf("|%-3d|%-9s|%-9s|%-9s|%-9s|RM%7.2f   |%-7d |RM%7.2f  |RM%7.2f   |RM%7.2f  |RM%7.2f |%-10c    |%02d/%02d/%4d |\n", i + 1, sales[i].salesOrderID,
						sales[i].staffID, sales[i].memberID, sales[i].itemCode, sales[i].price, sales[i].quantity, sales[i].total, sales[i].taxAmount,
						sales[i].grandTotal, sales[i].commission, sales[i].paymentMethod, sales[i].salesDate.day, sales[i].salesDate.month, sales[i].salesDate.year);
					tableLineSales(141);
					modifyNum = i;
				}
			}

			//Shows no matching records
			if (modifyNum == -1) {
				printf("The following Sales item does not exist.\n");
				return;
			}
		} while (modifyNum == -1);

		printf("\n\n");
		headingSales(4);
		//modification
		printf("============================================\n");
		printf("|           LIST OF MODIFICATION           |\n");
		printf("|------------------------------------------|\n");
		printf("| 1) Every Data Fields                     |\n");
		printf("|------------------------------------------|\n");
		printf("| 2) Staff ID                              |\n");
		printf("|------------------------------------------|\n");
		printf("| 3) Member ID                             |\n");
		printf("|------------------------------------------|\n");
		printf("| 4) Item Code                             |\n");
		printf("|------------------------------------------|\n");
		printf("| 5) Quantity of Item Sold                 |\n");
		printf("|------------------------------------------|\n");
		printf("| 6) Payment Method                        |\n");
		printf("|------------------------------------------|\n");
		printf("| 7) Date of Item Sold                     |\n");
		printf("|------------------------------------------|\n");
		printf("| 8) Exit Modification                     |\n");
		printf("============================================\n");
		printf("Select one of the options [1-8]: ");
		rewind(stdin);
		scanf("%d", &choice);
		//validation
		while (choice < 1 || choice > 8) {
			printf("\nInvalid! Please key in accordingly\n");
			printf("Select one of the options [1-8]: ");
			rewind(stdin);
			scanf("%d", &choice);
		}

		printf("\n");
		switch (choice) {
		case 1:
			//new staff id
			do {
				printf("\nEnter new staff id [SF001]: ");
				rewind(stdin);
				scanf("%[^\n]", modifyS.staffID);
				checkID = idValidation(modifyS.staffID, "SF", "Staff ID");
			} while (checkID == 0);

			//new member ID
			do {
				printf("\nEnter new Member ID [M101]: ");
				rewind(stdin);
				scanf("%s", modifyS.memberID);
			} while (idValidation(modifyS.memberID, "M", "Member ID") == 0);

			do {
				//new Product ID
				do {
					printf("\nEnter new Item Code [P001]: ");
					rewind(stdin);
					scanf("%s", modifyS.itemCode);
				} while (idValidation(modifyS.itemCode, "P", "Item Code") == 0 || isExistingItem(modifyS.itemCode, stock, totalStock) == 0
					|| isMinimumLevel(modifyS.itemCode, stock, totalStock) == 0);

				//update price
				int updatedPrice = 0;
				//Auto enter sales price based on stock module item price
				for (int i = 0; i < totalStock; i++) {
					if (strcmp(modifyS.itemCode, stock[i].itemCode) == 0) {
						modifyS.price = stock[i].itemPrice;
						updatedPrice++;
					}
				}
				//If price could not be found
				if (updatedPrice == 0) {
					printf("\nAn error has occurred, the price for the item could not be found. Exiting the add sales function...\n\n");
					system("pause");
					return;
				}

				//new Sales quantity
				do {
					printf("\nEnter new Sales Quantity: ");
					rewind(stdin);
					scanf("%d", &modifyS.quantity);
				} while (intValidation("sales quantity", modifyS.quantity, 1, 20) == 0);
			} while (checkAvailableStock(modifyS.quantity, modifyS.itemCode, stock, totalStock) == 0);
			updateStock(modifyS.quantity, modifyS.itemCode, stock, totalStock);

			//total
			modifyS.total = modifyS.quantity * modifyS.price;

			//taxed amount
			modifyS.taxAmount = modifyS.total * tax;

			//grand total
			modifyS.grandTotal = modifyS.total + modifyS.taxAmount;

			//commission
			modifyS.commission = modifyS.grandTotal * com;

			//new payment method
			printf("\nEnter new Payment Method [Cash = C /Online = O]: ");
			rewind(stdin);
			scanf("%c", &modifyS.paymentMethod);
			method = toupper(modifyS.paymentMethod);
			while (method != 'C' && method != 'O') {
				printf("\nInvalid! Please key in accordingly");
				printf("\nEnter Payment Method [Cash = C /Online = O]: ");
				rewind(stdin);
				scanf("%c", &modifyS.paymentMethod);
				method = toupper(modifyS.paymentMethod);
			}

			//new sales date
			do {
				printf("\nEnter the new Sales Date (DD/MM/YYYY): ");
				rewind(stdin);
				scanf("%d/%d/%d", &modifyS.salesDate.day, &modifyS.salesDate.month, &modifyS.salesDate.year);
			} while (dateValidation(modifyS.salesDate.day, modifyS.salesDate.month, modifyS.salesDate.year) == 0);

			//display modification
			headingDisplay();
			printf("|%-3d|%-9s|%-9s|%-9s|%-9s|RM%7.2f   |%-7d |RM%7.2f  |RM%7.2f   |RM%7.2f  |RM%7.2f |%-10c    |%02d/%02d/%4d |\n", modifyNum + 1, sales[modifyNum].salesOrderID,
				modifyS.staffID, modifyS.memberID, modifyS.itemCode, modifyS.price, modifyS.quantity, modifyS.total, modifyS.taxAmount,
				modifyS.grandTotal, modifyS.commission, modifyS.paymentMethod, modifyS.salesDate.day, modifyS.salesDate.month, modifyS.salesDate.year);
			tableLineSales(141);
			break;


		case 2:
			do {
				printf("\nEnter new staff id [SF001]: ");
				rewind(stdin);
				scanf("%[^\n]", modifyS.staffID);
				checkID = idValidation(modifyS.staffID, "SF", "Staff ID");
			} while (checkID == 0);

			//display modification
			headingDisplay();
			printf("|%-3d|%-9s|%-9s|%-9s|%-9s|RM%7.2f   |%-7d |RM%7.2f  |RM%7.2f   |RM%7.2f  |RM%7.2f |%-10c    |%02d/%02d/%4d |\n", modifyNum + 1, sales[modifyNum].salesOrderID,
				modifyS.staffID, sales[modifyNum].memberID, sales[modifyNum].itemCode, sales[modifyNum].price, sales[modifyNum].quantity, sales[modifyNum].total, sales[modifyNum].taxAmount,
				sales[modifyNum].grandTotal, sales[modifyNum].commission, sales[modifyNum].paymentMethod, sales[modifyNum].salesDate.day, sales[modifyNum].salesDate.month, sales[modifyNum].salesDate.year);
			tableLineSales(141);
			break;

		case 3:
			do {
				printf("\nEnter new Member ID [M101]: ");
				rewind(stdin);
				scanf("%s", modifyS.memberID);
			} while (idValidation(modifyS.memberID, "M", "Member ID") == 0);

			//display modification
			headingDisplay();
			printf("|%-3d|%-9s|%-9s|%-9s|%-9s|RM%7.2f   |%-7d |RM%7.2f  |RM%7.2f   |RM%7.2f  |RM%7.2f |%-10c    |%02d/%02d/%4d |\n", modifyNum + 1, sales[modifyNum].salesOrderID,
				sales[modifyNum].staffID, modifyS.memberID, sales[modifyNum].itemCode, sales[modifyNum].price, sales[modifyNum].quantity, sales[modifyNum].total, sales[modifyNum].taxAmount,
				sales[modifyNum].grandTotal, sales[modifyNum].commission, sales[modifyNum].paymentMethod, sales[modifyNum].salesDate.day, sales[modifyNum].salesDate.month, sales[modifyNum].salesDate.year);
			tableLineSales(141);
			break;

		case 4:
			do {
				printf("\nEnter new Item Code [P001]: ");
				rewind(stdin);
				scanf("%s", modifyS.itemCode);
			} while (idValidation(modifyS.itemCode, "P", "Item Code") == 0 || isExistingItem(modifyS.itemCode, stock, totalStock) == 0
				|| isMinimumLevel(modifyS.itemCode, stock, totalStock) == 0);

			//update price
			int updatedPrice = 0;
			//Auto enter sales price based on stock module item price
			for (int i = 0; i < totalStock; i++) {
				if (strcmp(modifyS.itemCode, stock[i].itemCode) == 0) {
					modifyS.price = stock[i].itemPrice;
					updatedPrice++;
				}
			}
			//If price could not be found
			if (updatedPrice == 0) {
				printf("\nAn error has occurred, the price for the item could not be found. Exiting the add sales function...\n\n");
				system("pause");
				return;
			}

			//total
			modifyS.total = sales[modifyNum].quantity * modifyS.price;

			//taxed amount
			modifyS.taxAmount = modifyS.total * tax;

			//grand total
			modifyS.grandTotal = modifyS.total + modifyS.taxAmount;

			//commission
			modifyS.commission = modifyS.grandTotal * com;

			//display modification
			headingDisplay();
			printf("|%-3d|%-9s|%-9s|%-9s|%-9s|RM%7.2f   |%-7d |RM%7.2f  |RM%7.2f   |RM%7.2f  |RM%7.2f |%-10c    |%02d/%02d/%4d |\n", modifyNum + 1, sales[modifyNum].salesOrderID,
				sales[modifyNum].staffID, sales[modifyNum].memberID, modifyS.itemCode, modifyS.price, sales[modifyNum].quantity, modifyS.total, modifyS.taxAmount,
				modifyS.grandTotal, modifyS.commission, sales[modifyNum].paymentMethod, sales[modifyNum].salesDate.day, sales[modifyNum].salesDate.month, sales[modifyNum].salesDate.year);
			tableLineSales(141);
			break;

		case 5:
			do {
				do {
					printf("\nEnter new Sales Quantity: ");
					rewind(stdin);
					scanf("%d", &modifyS.quantity);
				} while (intValidation("sales quantity", modifyS.quantity, 1, 20) == 0);
			} while (checkAvailableStock(modifyS.quantity, sales[modifyNum].itemCode, stock, totalStock) == 0);
			updateStock(modifyS.quantity, sales[modifyNum].itemCode, stock, totalStock);

			//total
			modifyS.total = modifyS.quantity * sales[modifyNum].price;

			//taxed amount
			modifyS.taxAmount = modifyS.total * tax;

			//grand total
			modifyS.grandTotal = modifyS.total + modifyS.taxAmount;

			//commission
			modifyS.commission = modifyS.grandTotal * com;

			//display modification
			headingDisplay();
			printf("|%-3d|%-9s|%-9s|%-9s|%-9s|RM%7.2f   |%-7d |RM%7.2f  |RM%7.2f   |RM%7.2f  |RM%7.2f |%-10c    |%02d/%02d/%4d |\n", modifyNum + 1, sales[modifyNum].salesOrderID,
				sales[modifyNum].staffID, sales[modifyNum].memberID, sales[modifyNum].itemCode, sales[modifyNum].price, modifyS.quantity, modifyS.total, modifyS.taxAmount,
				modifyS.grandTotal, modifyS.commission, sales[modifyNum].paymentMethod, sales[modifyNum].salesDate.day, sales[modifyNum].salesDate.month, sales[modifyNum].salesDate.year);
			tableLineSales(141);
			break;

		case 6:
			printf("\nEnter new Payment Method [Cash = C /Online = O]: ");
			rewind(stdin);
			scanf("%c", &modifyS.paymentMethod);
			method = toupper(modifyS.paymentMethod);
			while (method != 'C' && method != 'O') {
				printf("\nInvalid! Please key in accordingly");
				printf("\nEnter Payment Method [Cash = C /Online = O]: ");
				rewind(stdin);
				scanf("%c", &modifyS.paymentMethod);
				method = toupper(modifyS.paymentMethod);
			}

			//display modification
			headingDisplay();
			printf("|%-3d|%-9s|%-9s|%-9s|%-9s|RM%7.2f   |%-7d |RM%7.2f  |RM%7.2f   |RM%7.2f  |RM%7.2f |%-10c    |%02d/%02d/%4d |\n", modifyNum + 1, sales[modifyNum].salesOrderID,
				sales[modifyNum].staffID, sales[modifyNum].memberID, sales[modifyNum].itemCode, sales[modifyNum].price, sales[modifyNum].quantity, sales[modifyNum].total, sales[modifyNum].taxAmount,
				sales[modifyNum].grandTotal, sales[modifyNum].commission, modifyS.paymentMethod, sales[modifyNum].salesDate.day, sales[modifyNum].salesDate.month, sales[modifyNum].salesDate.year);
			tableLineSales(141);
			break;

		case 7:
			do {
				printf("\nEnter the new Sales Date (DD/MM/YYYY): ");
				rewind(stdin);
				scanf("%d/%d/%d", &modifyS.salesDate.day, &modifyS.salesDate.month, &modifyS.salesDate.year);
			} while (dateValidation(modifyS.salesDate.day, modifyS.salesDate.month, modifyS.salesDate.year) == 0);

			//display modification
			headingDisplay();
			printf("|%-3d|%-9s|%-9s|%-9s|%-9s|RM%7.2f   |%-7d |RM%7.2f  |RM%7.2f   |RM%7.2f  |RM%7.2f |%-10c    |%02d/%02d/%4d |\n", modifyNum + 1, sales[modifyNum].salesOrderID,
				sales[modifyNum].staffID, sales[modifyNum].memberID, sales[modifyNum].itemCode, sales[modifyNum].price, sales[modifyNum].quantity, sales[modifyNum].total, sales[modifyNum].taxAmount,
				sales[modifyNum].grandTotal, sales[modifyNum].commission, sales[modifyNum].paymentMethod, modifyS.salesDate.day, modifyS.salesDate.month, modifyS.salesDate.year);
			tableLineSales(141);
			break;

		case 8:
			system("cls");
			return;
		}

		printf("\nConfirm modify sales info? [Y/N]: ");
		rewind(stdin);
		scanf("%c", &confirm);
		confirm = toupper(confirm);

		//validation
		while (confirm != 'N' && confirm != 'Y') {
			printf("Invalid! Please key in accordingly\n");
			printf("Confirm modify sales info? [Y/N]: ");
			rewind(stdin);
			scanf("%c", &confirm);
			confirm = toupper(confirm);
		}

		if (confirm == 'Y') {
			switch (choice) {
			case 1:
				strcpy(sales[modifyNum].staffID, modifyS.staffID);
				strcpy(sales[modifyNum].memberID, modifyS.memberID);
				strcpy(sales[modifyNum].itemCode, modifyS.itemCode);
				sales[modifyNum].price = modifyS.price;
				sales[modifyNum].quantity = modifyS.quantity;
				sales[modifyNum].total = modifyS.total;
				sales[modifyNum].taxAmount = modifyS.taxAmount;
				sales[modifyNum].grandTotal = modifyS.grandTotal;
				sales[modifyNum].commission = modifyS.commission;
				sales[modifyNum].paymentMethod = modifyS.paymentMethod;
				sales[modifyNum].salesDate.day = modifyS.salesDate.day;
				sales[modifyNum].salesDate.month = modifyS.salesDate.month;
				sales[modifyNum].salesDate.year = modifyS.salesDate.year;
				break;
			case 2: strcpy(sales[modifyNum].staffID, modifyS.staffID); break;
			case 3: strcpy(sales[modifyNum].memberID, modifyS.memberID); break;
			case 4: 
				strcpy(sales[modifyNum].itemCode, modifyS.itemCode);
				sales[modifyNum].price = modifyS.price;
				sales[modifyNum].total = modifyS.total;
				sales[modifyNum].taxAmount = modifyS.taxAmount;
				sales[modifyNum].grandTotal = modifyS.grandTotal;
				sales[modifyNum].commission = modifyS.commission;
				break;
			case 5: 
				sales[modifyNum].quantity = modifyS.quantity;
				sales[modifyNum].total = modifyS.total;
				sales[modifyNum].taxAmount = modifyS.taxAmount;
				sales[modifyNum].grandTotal = modifyS.grandTotal;
				sales[modifyNum].commission = modifyS.commission;
				break;
			case 6: sales[modifyNum].paymentMethod = modifyS.paymentMethod; break;
			case 7:
				sales[modifyNum].salesDate.day = modifyS.salesDate.day;
				sales[modifyNum].salesDate.month = modifyS.salesDate.month;
				sales[modifyNum].salesDate.year = modifyS.salesDate.year;
				break;
			}

			writeSales(sales, totalSales);
			printf("\nItem modified successfully\n");
		}

		else {
			printf("\nCancelled adding item\n");
		}

		printf("\nContinue modify sales info? [Y/N]: ");
		confirm = decision();
	} while (confirm != 'N');
	system("cls");
}
void deleteSales(Sales* sales, int* totalSales) {
	int checkID, deleteNo;
	char ID[10], confirm, confirmDelete;


	do {
		system("cls");
		headingSales(5);
		do {
			printf("\nEnter sales id [S001]: ");
			rewind(stdin);
			scanf("%[^\n]", ID);
			checkID = idValidation(ID, "S", "Sales ID");
		} while (checkID == 0);
		printf("\n-------- Record to be delete --------\n");
		headingDisplay();

		do {
			deleteNo = -1;
			for (int no = 0; no < *totalSales; no++) {
				if (strcmp(ID, sales[no].salesOrderID) == 0) {
					tableLineSales(141);
					printf("|%-3d|%-9s|%-9s|%-9s|%-9s|RM%7.2f   |%-7d |RM%7.2f  |RM%7.2f   |RM%7.2f  |RM%7.2f |%-10c    |%02d/%02d/%4d |\n", no + 1, sales[no].salesOrderID,
						sales[no].staffID, sales[no].memberID, sales[no].itemCode, sales[no].price, sales[no].quantity, sales[no].total, sales[no].taxAmount,
						sales[no].grandTotal, sales[no].commission, sales[no].paymentMethod, sales[no].salesDate.day, sales[no].salesDate.month, sales[no].salesDate.year);
					tableLineSales(141);
					deleteNo = no;
				}
			}

			//Shows no matching records
			if (deleteNo == -1) {
				tableLineSales(141);
				printf("The following Sales item does not exist.\n");
				printf("\n-------- Record to be delete --------\n");
				do {
					printf("Enter sales id [S001]: ");
					rewind(stdin);
					scanf("%[^\n]", ID);
					checkID = idValidation(ID, "S", "Sales ID");
				} while (checkID == 0);
			}
		} while (deleteNo == -1);


		printf("\nConfirm delete records? [Y/N]: ");
		rewind(stdin);
		scanf("%c", &confirmDelete);
		confirmDelete = toupper(confirmDelete);
		//validation
		while (confirmDelete != 'N' && confirmDelete != 'Y') {
			printf("Invalid! Please key in accordingly\n");
			printf("Confirm delete records? [Y/N]: ");
			rewind(stdin);
			scanf("%c", &confirmDelete);
			confirmDelete = toupper(confirmDelete);
		}

		//Cancel delete process
		if (confirmDelete == 'N')
			printf("\nCancelled deleting item\n");

		//Delete item by shifting record to the front
		for (int i = deleteNo; i < *totalSales - 1; i++) {
			sales[i] = sales[i + 1];
		}
		(*totalSales)--;
		writeSales(sales, *totalSales);
		printf("\nItem deleted successfully\n");

		//confirmation
		printf("Continue delete staff info? [Y/N]: ");
		confirm = decision();
	} while (confirm != 'N');
	system("cls");
}
void salesReport(Sales* sales, int totalSales) {
	Sales checkSales[99];
	double total = 0, taxAmount = 0, grandTotal = 0, commission = 0;
	int select, num_items;
	int year, checkYear;;
	char check[45], confirm;

	do {
		num_items = readSales(checkSales);
		displaySales(checkSales, num_items);
		printf("\n\n");
		headingSales(6);
		printf("%80s", "==================================\n");
		printf("%80s", "|          SALES REPORT          |\n");
		printf("%80s", "|--------------------------------|\n");
		printf("%80s", "| 1) Total Sales by Years        |\n");
		printf("%80s", "|--------------------------------|\n");
		printf("%80s", "| 2) Total Tax Amount by Years   |\n");
		printf("%80s", "|--------------------------------|\n");
		printf("%80s", "| 3) Total Grand Total by Years  |\n");
		printf("%80s", "|--------------------------------|\n");
		printf("%80s", "| 4) Total Commission by Years   |\n");
		printf("%80s", "|--------------------------------|\n");
		printf("%80s", "| 5) Exit Sales Report           |\n");
		printf("%80s", "=================================\n");
		printf("%80s", "Select one of the options (1 - 5): ");

		//Invalid data type check
		if (scanf("%d", &select) != 1) {
			scanf("%[^\n]", check);
			select = 6;
			printf("\nError: '%s' is not an integer! Please enter a value of 1 - 5 only.\n\n", check);
		}
		//Out of range
		else if (select < 1 || select > 5) {
			printf("\nError: '%d' is not an option! Please enter a value of 1 - 5 only.\n\n", select);
		}

		switch (select) {
		case 1:
			system("cls");
			do {
				printf("Enter the year you want to search [2013 - 2023]: ");
				rewind(stdin);
				scanf("%d", &year);
				checkYear = yearValidation(year);
			} while (checkYear != 1);

			printf("\n-------- Display sales info ---------\n");
			tableLineSales(105);
			printf("%-3s %-9s %-9s %-9s %-9s %9s %10s %-9s %-10s %10s  %s\n",
				"No", "Sales ID", "Staff ID", "Member ID", "Item Code", "Price (RM)", "Quantity", "   Total   ", "Payment Method", "  Date  ", "|");
			printf("%-3s %-9s %-9s %-9s %-9s %9s %10s %-9s %-10s %10s%s\n",
				"===", "========", "========", "=========", "=========", "==========", "========", "===========", "==============", "============", "|");

			for (int no = 0; no < num_items; no++) {
				if (year == checkSales[no].salesDate.year) {
					printf("|%-3d|%-9s|%-9s|%-9s|%-9s|RM%7.2f   |%-7d |RM%7.2f  |%-10c    |%02d/%02d/%4d |\n", no + 1, checkSales[no].salesOrderID,
						checkSales[no].staffID, checkSales[no].memberID, checkSales[no].itemCode, checkSales[no].price, checkSales[no].quantity, checkSales[no].total,
						checkSales[no].paymentMethod, checkSales[no].salesDate.day, checkSales[no].salesDate.month, checkSales[no].salesDate.year);

					total += checkSales[no].total;
				}
			}
			tableLineSales(105);
			printf("\nTotal Sales: %.2f\n", total);
			break;
		case 2:
			system("cls");
			do {
				printf("Enter the year you want to search [2013 - 2023]: ");
				rewind(stdin);
				scanf("%d", &year);
				checkYear = yearValidation(year);
			} while (checkYear != 1);

			printf("\n-------- Display sales info ---------\n");
			tableLineSales(118);
			printf("%-3s %-9s %-9s %-9s %-9s %9s %10s %-9s %-9s %-10s %10s  %s\n",
				"No", "Sales ID", "Staff ID", "Member ID", "Item Code", "Price (RM)", "Quantity", "   Total   ", " Tax Amount ", "Payment Method", "  Date  ", "|");
			printf("%-3s %-9s %-9s %-9s %-9s %9s %10s %-9s %-9s %-10s %10s%s\n",
				"===", "========", "========", "=========", "=========", "==========", "========", "===========", "============", "==============", "============", "|");

			for (int no = 0; no < num_items; no++) {
				if (year == checkSales[no].salesDate.year) {
					printf("|%-3d|%-9s|%-9s|%-9s|%-9s|RM%7.2f   |%-7d |RM%7.2f  |RM%7.2f   |%-10c    |%02d/%02d/%4d |\n", no + 1, checkSales[no].salesOrderID,
						checkSales[no].staffID, checkSales[no].memberID, checkSales[no].itemCode, checkSales[no].price, checkSales[no].quantity, checkSales[no].total, checkSales[no].taxAmount,
						checkSales[no].paymentMethod, checkSales[no].salesDate.day, checkSales[no].salesDate.month, checkSales[no].salesDate.year);

					taxAmount += checkSales[no].taxAmount;
				}
			}
			tableLineSales(118);
			printf("\nTotal Tax Amount: %.2f\n", taxAmount);
			break;
		case 3:
			system("cls");
			do {
				printf("Enter the year you want to search [2013 - 2023]: ");
				rewind(stdin);
				scanf("%d", &year);
				checkYear = yearValidation(year);
			} while (checkYear != 1);

			printf("\n-------- Display sales info ---------\n");
			tableLineSales(105);
			printf("%-3s %-9s %-9s %-9s %-9s %9s %10s %-9s %-10s %10s  %s\n",
				"No", "Sales ID", "Staff ID", "Member ID", "Item Code", "Price (RM)", "Quantity", "Grand Total", "Payment Method", "  Date  ", "|");
			printf("%-3s %-9s %-9s %-9s %-9s %9s %10s %-9s %-10s %10s%s\n",
				"===", "========", "========", "=========", "=========", "==========", "========", "===========", "==============", "============", "|");

			for (int no = 0; no < num_items; no++) {
				if (year == checkSales[no].salesDate.year) {
					printf("|%-3d|%-9s|%-9s|%-9s|%-9s|RM%7.2f   |%-7d |RM%7.2f  |%-10c    |%02d/%02d/%4d |\n", no + 1, checkSales[no].salesOrderID,
						checkSales[no].staffID, checkSales[no].memberID, checkSales[no].itemCode, checkSales[no].price, checkSales[no].quantity, checkSales[no].grandTotal,
						checkSales[no].paymentMethod, checkSales[no].salesDate.day, checkSales[no].salesDate.month, checkSales[no].salesDate.year);

					grandTotal += checkSales[no].grandTotal;
				}
			}
			tableLineSales(105);
			printf("\nTotal Sales: %.2f\n", total);
			break;
		case 4:
			system("cls");
			do {
				printf("Enter the year you want to search [2013 - 2023]: ");
				rewind(stdin);
				scanf("%d", &year);
				checkYear = yearValidation(year);
			} while (checkYear != 1);

			printf("\n-------- Display sales info ---------\n");
			tableLineSales(104);
			printf("%-3s %-9s %-9s %-9s %-9s %9s %10s %-9s %-10s %10s  %s\n",
				"No", "Sales ID", "Staff ID", "Member ID", "Item Code", "Price (RM)", "Quantity", "Commission", "Payment Method", "  Date  ", "|");
			printf("%-3s %-9s %-9s %-9s %-9s %9s %10s %-9s %-10s %10s%s\n",
				"===", "========", "========", "=========", "=========", "==========", "========", "==========", "==============", "============", "|");

			for (int no = 0; no < num_items; no++) {
				if (year == checkSales[no].salesDate.year) {
					printf("|%-3d|%-9s|%-9s|%-9s|%-9s|RM%7.2f   |%-7d |RM%7.2f |%-10c    |%02d/%02d/%4d |\n", no + 1, sales[no].salesOrderID,
						sales[no].staffID, sales[no].memberID, sales[no].itemCode, sales[no].price, sales[no].quantity,
						sales[no].commission, sales[no].paymentMethod, sales[no].salesDate.day, sales[no].salesDate.month, sales[no].salesDate.year);

					commission += checkSales[no].commission;
				}
			}
			tableLineSales(104);
			printf("\nTotal Commission: %.2f\n", commission);
			break;
		case 5: return;
		}

		printf("\nContinue generate Sales Report? [Y/N]: ");
		confirm = decision();
	} while (confirm != 'N');
}

void headingDisplay() {
	tableLineSales(141);
	printf("%-3s %-9s %-9s %-9s %-9s %9s %10s %-9s %-9s %-9s %-9s %-10s %10s  %s\n",
		"No", "Sales ID", "Staff ID", "Member ID", "Item Code", "Price (RM)", "Quantity", "   Total   ", " Tax Amount ", "Grand Total", "Commission", "Payment Method", "  Date  ", "|");
	printf("%-3s %-9s %-9s %-9s %-9s %9s %10s %-9s %-9s %-9s %-9s %-10s %10s%s\n",
		"===", "========", "========", "=========", "=========", "==========", "========", "===========", "============", "===========", "==========", "==============", "============", "|");
}
void tableLineSales(int num) {
	for (int i = 0; i < num; i++)
		printf("-");
	printf("\n");
}
void headingSales(int type) {
	switch (type) {
	case 1:
		printf("***************************************************************************\n");
		printf("**                        A D D I N G    I T E M                         **\n");
		printf("***************************************************************************\n");
		break;

	case 2:
		printf("%100s", "***************************************************************************\n");
		printf("%100s", "**                      D I S P L A Y I N G   I T E M                    **\n");
		printf("%100s", "***************************************************************************\n");
		break;

	case 3:
		printf("***************************************************************************\n");
		printf("**                   S E A R C H I N G   F O R   I T E M                 **\n");
		printf("***************************************************************************\n");
		break;

	case 4:
		printf("***************************************************************************\n");
		printf("**                       M O D I F Y I N G   I T E M                     **\n");
		printf("***************************************************************************\n");
		break;

	case 5:
		printf("***************************************************************************\n");
		printf("**                        D E L E T I N G   I T E M                      **\n");
		printf("***************************************************************************\n");
		break;

	case 6:
		printf("%100s", "***************************************************************************\n");
		printf("%100s", "**                          S A L E S   R E P O R T                      **\n");
		printf("%100s", "***************************************************************************\n");
		break;

	case 7:
		printf("***************************************************************************\n");
		printf("**               R E C O R D   T O   B E   M O D I F I E D               **\n");
		printf("***************************************************************************\n");
		break;
	default:
		printf("Invalid choice. Please enter again.\n");
		system("pause");
	}
}

void appendSales(Sales* appendSales, int num) {
	FILE* salesA;
	salesA = fopen("sales.bin", "ab");
	if (!salesA) {
		printf("Error opening file! Please check your file\n");
		exit(-1);
	}

	fwrite(&appendSales[num], sizeof(Sales), 1, salesA);
	fclose(salesA);
	printf("\nData written to file successfully.\n");
}
int readSales(Sales* sales) {
	FILE* salesR;
	salesR = fopen("sales.bin", "rb");
	if (!salesR) {
		printf("Error opening file! Please check your file\n");
		exit(-1);
	}
	int no = 0;

	while (fread(&sales[no], sizeof(Sales), 1, salesR) == 1) {
		no++;
	}

	fclose(salesR);
	return(no);
}
void writeSales(Sales* writeSales, int num) {
	FILE* salesW;
	salesW = fopen("sales.bin", "wb");
	if (!salesW) {
		printf("Error opening file! Please check your file\n");
		exit(-1);
	}

	for (int i = 0; i < num; i++) {
		fwrite(&writeSales[i], sizeof(Sales), 1, salesW);
	}

	fclose(salesW);
	printf("\nData(s) written to file successfully.\n");
}

int salesIdUnique(Sales* sales, int num, char id[5]) {
	for (int i = 0; i < num; i++) {
		if (strcmp(id, sales[i].salesOrderID) == 0) {
			printf("Duplicate ID found! Please enter a different Sales ID.\n");
			return 0;
		}
	}
	return 1;
}
int yearValidation(int year) {
	if (year < 2013) {
		printf("Our branding, Adidas was established on 2013, please enter a year after 2013.\n\n");
		return 0;
	}
	else if (year > 2023) {
		printf("Please enter a valid year, between 2013 to 2023.\n\n");
		return 0;
	}
	return 1;
}

//Check if new Member ID exists in member file
int isExistingMember(char memberID[6], Member* member, int member_no) {
	for (int i = 0; i < member_no; i++) {
		if (strcmp(memberID, member[i].memberID) == 0) {
			return 1;
		}
	}
	printf("The Member ID you entered does not exist, please enter an existing Member ID from the member module.\n");
	return 0;
}
//Check if new Staff ID exists in staff file
int isExistingStaff(char staffID[6], Staff* staff, int staffNo) {
	for (int i = 0; i < staffNo; i++) {
		if (strcmp(staffID, staff[i].staffID) == 0) {
			return 1;
		}
	}
	printf("The Staff ID you entered does not exist, please enter an existing Staff ID from the staff module.\n");
	return 0;
}

int readStaff(Staff* staff) {
	FILE* stfr;
	stfr = fopen("staff.txt", "r");
	if (stfr == NULL) {
		printf("Error opening file! Please check your file\n");
		exit(-1);
	}
	int row = 0;

	while (!feof(stfr)) {
		fscanf(stfr, "|%[^|]|%[^|]|%[^|]|%[^|]|%c|%lf|%d|%d|%d|%d|\n", staff[row].staffID, staff[row].password,
			staff[row].name, staff[row].staffic, &staff[row].gender, &staff[row].salary, &staff[row].workminute,
			&staff[row].indate.day, &staff[row].indate.month, &staff[row].indate.year);
		row++;
	}

	fclose(stfr);
	return row;
}