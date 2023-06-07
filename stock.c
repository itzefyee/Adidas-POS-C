#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include <time.h>
#include<Windows.h>
#pragma warning(disable:4996)

typedef struct {
	char memberID[10];
	char name[60];
	char gender;
	char IC[20];
	char contact[15];
	char upLineID[10];
}Member;

//Stock Module Structures
typedef struct {
	int day;
	int month;
	int year;
}Date;

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

//Functions
void stockMenu();
void addStock(Stock* stock, int* totalStock);
void displayStock(Stock* stock, int totalStock);
int searchStock(Stock* stock, int totalStock);
void modifyStock(Stock* stock, int totalStock);
void deleteStock(Stock* stock, int* totalStock);
void stockReport(Stock* stock, int totalStock);
void sorting(Stock* stock, int totalStock);

//Access files
void appendStock(Stock* appendStock, int totalStock);
int readStock(Stock* stock);
void rewriteStock(Stock* rewriteStock, int totalStock);

//Validations
char decision();
int idUnique(Stock* stock, int totalStock, char id[5]);
int idValidation(char id[6], char idChar[3], char idChecked[]);
int intValidation(char dataField[], int num, int min, int max);
int decValidation(char dataField[], double num, int min, int max);
int dateValidation(int day, int month, int year);
int highOrLow();
int beforeOrAfter(Stock* stock, int totalStock, Stock search, Stock* searchResult, int numRecord);

//Display
void easter();
int menu(int type);
int menuChoice(int num_choice);
void tableLines();
void heading(int type);
void logo();
void exit_();

//Others
int icValidation(char id[15]);
int phoneNumValidation(char phoneNum[12]);
int isExistingItem(char itemCode[6], Stock* stock, int totalStock);
int isMinimumLevel(char itemCode[6], Stock* stock, int totalStock);
int checkAvailableStock(int quantity, char itemCode[6], Stock* stock, int totalStock);
void updateStock(int quantity, char itemCode[6], Stock* stock, int totalStock);


void stockMenu() {
	Stock stock[100];

	int menuChoice, num_items;

	do {
		menuChoice = menu(1);
		num_items = readStock(stock);
		//Switch to respective functions
		switch (menuChoice) {
		case 1:
			system("cls");
			addStock(stock, &num_items);
			break;
		case 2:
			system("cls");

			heading(3);
			tableLines();
			heading(1);
			displayStock(stock, num_items);
			tableLines();
			printf("Number of items : %d\n\n", num_items);

			system("pause");
			system("cls");
			break;

		case 3:
			searchStock(stock, num_items);
			system("cls");
			break;
		case 4:
			modifyStock(stock, num_items);
			system("cls");
			break;
		case 5:
			deleteStock(stock, &num_items);
			system("cls");
			break;
		case 6:
			stockReport(stock, num_items);
			system("cls");
			break;
		case 7:
			sorting(stock, num_items);
			system("cls");
			break;
		case 0:
			return;
		default:
			printf("Invalid choice\n");
			system("pause");
			break;
		}
	} while (menuChoice != 0);
}

void addStock(Stock* stock, int* totalStock) {
	char proceed, confirm;
	int addNo = 0;
	int check;

	do {
		heading(2);
		printf("\n--- Enter Item %d details ---\n", *totalStock + 1);

		//Item Code
		do {
			printf("Item code (P001)                             : ");
			rewind(stdin);
			scanf("%[^\n]", &stock[*totalStock].itemCode);
			check = idValidation(stock[*totalStock].itemCode, "P", "ItemCode");
			if (check == 1)
				check = idUnique(stock, *totalStock, stock[*totalStock].itemCode);
		} while (check == 0);

		//Item description
		do {
			printf("Item description (Ultraboost 5.0 Core Black) : ");
			rewind(stdin);
			scanf("%[^\n]", &stock[*totalStock].description);
			check = (strlen(stock[*totalStock].description) <= 40);
			if (check == 0)
				printf("Item description has to be within 40 characters, please enter again\n\n");
		} while (check == 0);

		//Item price
		do {
			printf("Price in RM(500)                             : ");
			rewind(stdin);
			if (scanf("%lf", &stock[*totalStock].itemPrice) != 1)
				printf("Error: Price must be a number\n\n");
			check = decValidation("product price", stock[*totalStock].itemPrice, 15, 1499);
		} while (check == 0);

		//Quantity in stock
		do {
			printf("Quantity in stock (500)                      : ");
			rewind(stdin);
			if (scanf("%d", &stock[*totalStock].stockQty) != 1)
				printf("Error: Quantity in stock must be a number\n\n");
			check = intValidation("quantity in stock", stock[*totalStock].stockQty, 50, 999);
		} while (check == 0);

		//minimum stock level
		do {
			printf("Minimum stock level (50)                     : ");
			rewind(stdin);
			if (scanf("%d", &stock[*totalStock].minStock) != 1)
				printf("Error: Stock level must be a number\n\n");
			check = intValidation("minumum stock level", stock[*totalStock].minStock, 50, 300);
		} while (check == 0);

		//Reorder Quantity
		do {
			printf("Reorder Quantity (300)                       : ");
			rewind(stdin);
			if (scanf("%d", &stock[*totalStock].reorderQty) != 1)
				printf("Error: Reorder quantity must be a number\n\n");
			check = intValidation("reorder quantity", stock[*totalStock].reorderQty, 200, 600);
		} while (check == 0);

		//Last Restock Date
		do {
			printf("Last Restock Date (DD/MM/YYYY)               : ");
			rewind(stdin);
			scanf("%d/%d/%d", &stock[*totalStock].lastRestock.day, &stock[*totalStock].lastRestock.month, &stock[*totalStock].lastRestock.year);
			check = dateValidation(stock[*totalStock].lastRestock.day, stock[*totalStock].lastRestock.month, stock[*totalStock].lastRestock.year);
		} while (check == 0);

		//Staff ID
		do {
			printf("Managed by staff (SF001)                     : ");
			rewind(stdin);
			scanf("%[^\n]", &stock[*totalStock].staffID);
			check = idValidation(stock[*totalStock].staffID, "SF", "Staff ID");
			if (check == 1)
				check = idUnique(stock, *totalStock, stock[*totalStock].staffID);
		} while (check == 0);

		//Confirmation
		printf("\nConfirm to enter details (Y/N)? ");
		confirm = decision();

		//Add item 
		if (confirm == 'Y') {
			appendStock(stock, *totalStock);
			(*totalStock)++;
			addNo++;
			printf("\nItem added successfully\n");
		}
		//Clear details
		else {
			stock[*totalStock] = stock[*totalStock + 1];
			printf("\nCancelled adding item\n");
		}

		//Continue to add item
		system("pause");
		printf("Continue to add another item (Y/N)? ");
		proceed = decision();
		system("cls");

	} while (proceed != 'N');

	printf("%d items has been added succesfully.\n", addNo);
}
void displayStock(Stock* stock, int totalStock) {
	for (int i = 0; i < totalStock; i++) {
		printf("|%2d|%-9s|%-38s|RM %7.2f|%9d|%14d|%11d|  %02d/%02d/%4d|%11s|\n",
			i + 1, stock[i].itemCode, stock[i].description, stock[i].itemPrice,
			stock[i].stockQty, stock[i].minStock, stock[i].reorderQty,
			stock[i].lastRestock.day, stock[i].lastRestock.month, stock[i].lastRestock.year,
			stock[i].staffID);
	}
}
int searchStock(Stock* stock, int totalStock) {
	Stock search, searchResult[100];
	char proceed = 'Y';
	int	numRecord = 0, choice;
	int check, numCompare;

	while (proceed != 'N') {
		system("cls");
		choice = menu(2);

		switch (choice) {
		case 1:
			//Search by item code 
			do {
				printf("Enter item code (P001) : ");
				rewind(stdin);
				scanf("%[^\n]", &search.itemCode);
				check = idValidation(search.itemCode, "P", "Item code");
			} while (check == 0);

			for (int i = 0; i < totalStock; i++) {
				if (strcmp(stock[i].itemCode, search.itemCode) == 0) {
					searchResult[numRecord] = stock[i];
					numRecord++;
				}
			}

			break;

		case 2:
			//Search by item description
			do {
				printf("Enter item description (Ultraboost 5.0 Core Black) : ");
				rewind(stdin);
				scanf("%[^\n]", &search.description);
				check = (strlen(search.description) <= 40);
				if (check == 0)
					printf("Item description has to be within 40 characters, please enter again\n\n");
			} while (check == 0);

			for (int i = 0; i < totalStock; i++) {
				if (strstr(stock[i].description, search.description)) {
					searchResult[numRecord] = stock[i];
					numRecord++;
				}
			}

			break;

		case 3:
			//Search by item price
			do {
				printf("Enter price in RM(500) : ");
				rewind(stdin);
				if (scanf("%lf", &search.itemPrice) != 1)
					printf("Error: Price must be a number\n\n");
				check = decValidation("product price", search.itemPrice, 15, 1499);
			} while (check == 0);

			numCompare = highOrLow();
			switch (numCompare) {
			case 1:
				for (int i = 0; i < totalStock; i++) {
					if (search.itemPrice <= stock[i].itemPrice) {
						searchResult[numRecord] = stock[i];
						numRecord++;
					}
				}
				break;

			case 2:
				for (int i = 0; i < totalStock; i++) {
					if (search.itemPrice >= stock[i].itemPrice) {
						searchResult[numRecord] = stock[i];
						numRecord++;
					}
				}
				break;
			}

			break;

		case 4:
			//Search by quantity in stock
			do {
				printf("Enter quantity in stock (500) : ");
				rewind(stdin);
				if (scanf("%d", &search.stockQty) != 1)
					printf("Error: Quantity in stock must be a number\n\n");
				check = intValidation("quantity in stock", search.stockQty, 50, 999);
			} while (check == 0);

			numCompare = highOrLow();
			switch (numCompare) {
			case 1:
				for (int i = 0; i < totalStock; i++) {
					if (search.stockQty <= stock[i].stockQty) {
						searchResult[numRecord] = stock[i];
						numRecord++;
					}
				}
				break;

			case 2:
				for (int i = 0; i < totalStock; i++) {
					if (search.stockQty >= stock[i].stockQty) {
						searchResult[numRecord] = stock[i];
						numRecord++;
					}
				}
				break;
			}

			break;

		case 5:
			//Search by minimum stock level
			do {
				printf("Enter minimum stock level (50) : ");
				rewind(stdin);
				if (scanf("%d", &search.minStock) != 1)
					printf("Error: Stock level must be a number\n\n");
				check = intValidation("minumum stock level", search.minStock, 50, 300);
			} while (check == 0);

			numCompare = highOrLow();
			switch (numCompare) {
			case 1:
				for (int i = 0; i < totalStock; i++) {
					if (search.minStock <= stock[i].minStock) {
						searchResult[numRecord] = stock[i];
						numRecord++;
					}
				}
				break;

			case 2:
				for (int i = 0; i < totalStock; i++) {
					if (search.minStock >= stock[i].minStock) {
						searchResult[numRecord] = stock[i];
						numRecord++;
					}
				}
				break;
			}

			break;

		case 6:
			//Search by reorder quantity
			do {
				printf("Enter reorder quantity (300) : ");
				rewind(stdin);
				if (scanf("%d", &search.reorderQty) != 1)
					printf("Error: Reorder quantity must be a number\n\n");
				check = intValidation("reorder quantity", search.reorderQty, 200, 600);
			} while (check == 0);

			numCompare = highOrLow();
			switch (numCompare) {
			case 1:
				for (int i = 0; i < totalStock; i++) {
					if (search.reorderQty <= stock[i].reorderQty) {
						searchResult[numRecord] = stock[i];
						numRecord++;
					}
				}
				break;

			case 2:
				for (int i = 0; i < totalStock; i++) {
					if (search.reorderQty >= stock[i].reorderQty) {
						searchResult[numRecord] = stock[i];
						numRecord++;
					}
				}
				break;
			}

			break;

		case 7:
			//Search by last restock date
			do {
				printf("Enter last Restock Date (DD/MM/YYYY): ");
				rewind(stdin);
				scanf("%d/%d/%d", &search.lastRestock.day, &search.lastRestock.month, &search.lastRestock.year);
				check = dateValidation(search.lastRestock.day, search.lastRestock.month, search.lastRestock.year);

			} while (check == 0);

			numRecord = beforeOrAfter(stock, totalStock, search, searchResult, numRecord);
			break;

		case 8:
			//Search by staff ID 
			do {
				printf("Enter staff ID (SF001) : ");
				rewind(stdin);
				scanf("%[^\n]", &search.staffID);
				check = idValidation(search.staffID, "SF", "Staff ID");
			} while (check == 0);

			for (int i = 0; i < totalStock; i++) {
				if (strcmp(stock[i].staffID, search.staffID) == 0) {
					searchResult[numRecord] = stock[i];
					numRecord++;
				}
			}

			break;

		case 0:
			return 0;
			break;

		default:
			printf("Invalid choice.\n");
			system("pause");
			break;
		}

		tableLines();
		heading(1);
		displayStock(searchResult, numRecord);
		tableLines();

		if (numRecord == 0) {
			printf("No matching records found!\n\n");
			system("pause");
		}
		else {
			printf("Found %d matching records!\n\n", numRecord);
			system("pause");
		}
		numRecord = 0;


		//Continue to search
		printf("Continue to search for item (Y/N)? ");
		proceed = decision();
	}
	return numRecord;
}
void modifyStock(Stock* stock, int totalStock) {
	Stock modify;
	int numModified = 0, modifyNo = 0, choice;
	char proceed = 'Y', confirm;
	char modifyT[21], check;
	//Modify term

	do {
		system("cls");
		heading(5);

		//Search item code to modify
		do {
			printf("--- Record to be modified ---\n");
			printf("Search item code (P001): ");
			rewind(stdin);
			scanf("%[^\n]", &modifyT);
			check = idValidation(modifyT, "P", "Item code");
		} while (check == 0);

		//Search for matching records
		do {
			for (int i = 0; i < totalStock; i++) {
				if (strcmp(stock[i].itemCode, modifyT) == 0) {
					printf("\nThis is the record found: \n");
					tableLines();
					heading(1);
					printf("|%2d|%-9s|%-38s|RM %7.2f|%9d|%14d|%11d|  %02d/%02d/%4d|%11s|\n",
						i + 1, stock[i].itemCode, stock[i].description, stock[i].itemPrice,
						stock[i].stockQty, stock[i].minStock, stock[i].reorderQty,
						stock[i].lastRestock.day, stock[i].lastRestock.month, stock[i].lastRestock.year,
						stock[i].staffID);
					tableLines();
					modifyNo = i;
				}
			}

			//Shows no matching records
			if (modifyNo == -1) {
				printf("The following Stock item does not exist. Please reenter again.\n");
				do {
					printf("--- Record to be modified ---\n");
					printf("Search item code (P001): ");
					rewind(stdin);
					scanf("%[^\n]", &modifyT);
					check = idValidation(modifyT, "P", "Item code");
				} while (check == 0);
			}
		} while (modifyNo == -1);

		//Confirmation
		printf("\nContinue to modify item (Y/N)? ");
		proceed = decision();

		//Back to menu
		if (proceed == 'N') {
			system("cls");
			return;
		}

		choice = menu(3);
		//Modification choice
		switch (choice) {
		case 1:
			// Item code
			do {
				printf("Enter updated item code: ");
				rewind(stdin);
				scanf("%[^\n]", &modify.itemCode);
				check = idValidation(modify.itemCode, "P", "Item code");
				if (check == 1)
					check = idUnique(stock, totalStock, modify.itemCode);
			} while (check == 0);
			break;

		case 2:
			// Item description
			do {
				printf("Enter updated item description: ");
				rewind(stdin);
				scanf("%[^\n]", &modify.description);
				check = (strlen(modify.description) <= 40);
				if (check == 0)
					printf("Item description has to be within 40 characters, please enter again\n\n");
			} while (check == 0);
			break;

		case 3:
			// Item price
			do {
				printf("Enter updated item price: ");
				rewind(stdin);
				if (scanf("%lf", &modify.itemPrice) != 1)
					printf("Error: Price must be a number\n\n");
				check = decValidation("product price", modify.itemPrice, 15, 1499);
			} while (check == 0);
			break;

		case 4:
			//Quantity in stock
			do {
				printf("Enter updated quantity in stock (500): ");
				rewind(stdin);
				if (scanf("%d", &modify.stockQty) != 1)
					printf("Error: Quantity in stock must be a number\n\n");
				check = intValidation("quantity in stock", modify.stockQty, 50, 999);
			} while (check == 0);
			break;

		case 5:
			//Minimum stock level
			do {
				printf("Enter updated minimum stock level (50): ");
				rewind(stdin);
				if (scanf("%d", &modify.minStock) != 1)
					printf("Error: Stock level must be a number\n\n");
				check = intValidation("minumum stock level", modify.minStock, 50, 300);
			} while (check == 0);
			break;

		case 6:
			//Reorder Quantity
			do {
				printf("Enter updated reorder quantity (200): ");
				rewind(stdin);
				if (scanf("%d", &modify.reorderQty) != 1)
					printf("Error: Reorder quantity must be a number\n\n");
				check = intValidation("reorder quantity", modify.reorderQty, 200, 600);
			} while (check == 0);
			break;

		case 7:
			//Last restock date
			do {
				printf("Enter updated last Restock Date (DD/MM/YYYY): ");
				rewind(stdin);
				scanf("%d/%d/%d", &modify.lastRestock.day, &modify.lastRestock.month, &modify.lastRestock.year);
				check = dateValidation(modify.lastRestock.day, modify.lastRestock.month, modify.lastRestock.year);
			} while (check == 0);
			break;

		case 8:
			// Staff ID
			do {
				printf("Enter updated staff ID: ");
				rewind(stdin);
				scanf("%[^\n]", &modify.staffID);
				check = idValidation(modify.staffID, "SF", "Staff ID");
			} while (check == 0);
			break;

		case 0:
			return;
			break;

		default:
			printf("Invalid choice! Please enter again\n");
			break;
		}

		//Confirmation
		printf("Confirm to modify details (Y/N)? ");
		confirm = decision();

		//Modify item details
		if (confirm == 'Y')
		{
			switch (choice) {
			case 1: strcpy(stock[modifyNo].itemCode, modify.itemCode); break;
			case 2: strcpy(stock[modifyNo].description, modify.description); break;
			case 3: stock[modifyNo].itemPrice = modify.itemPrice; break;
			case 4: stock[modifyNo].stockQty = modify.stockQty; break;
			case 5: stock[modifyNo].minStock = modify.minStock; break;
			case 6: stock[modifyNo].reorderQty = modify.reorderQty; break;
			case 7:
				stock[modifyNo].lastRestock.day = modify.lastRestock.day;
				stock[modifyNo].lastRestock.month = modify.lastRestock.month;
				stock[modifyNo].lastRestock.year = modify.lastRestock.year;
				break;
			case 8: strcpy(stock[modifyNo].staffID, modify.staffID); break;

				break;
			default:
				printf("Item not modified successfully\n"); system("pause"); return; break;
			}
			numModified++;
			rewriteStock(stock, totalStock);
			printf("\nItem modified successfully\n");

			for (int i = 0; i < totalStock; i++) {
				if (strcmp(stock[i].itemCode, modifyT) == 0) {
					tableLines();
					heading(1);
					printf("|%2d|%-9s|%-38s|RM %7.2f|%9d|%14d|%11d|  %02d/%02d/%4d|%11s|\n",
						i + 1, stock[i].itemCode, stock[i].description, stock[i].itemPrice,
						stock[i].stockQty, stock[i].minStock, stock[i].reorderQty,
						stock[i].lastRestock.day, stock[i].lastRestock.month, stock[i].lastRestock.year,
						stock[i].staffID);
					tableLines();
					modifyNo = i;
				}
			}
		}
		//Cancel modifying process
		else {
			printf("\nCancelled adding item\n");
		}

		//Continue to modify details
		printf("Continue to modify item (Y/N)? ");
		proceed = decision();

	} while (proceed != 'N');
}
void deleteStock(Stock* stock, int* totalStock) {
	int	numRecord = 0, numDeleted = 0, checkID, deleteNo = 0;
	// int choice;
	char proceed = 'Y', confirm;
	char deleteT[21];

	do {
		system("cls");
		heading(6);
		//Search item code to delete
		do {
			printf("--- Record to be deleted ---\n");
			printf("Enter item code (P001): ");
			rewind(stdin);
			scanf("%[^\n]", &deleteT);
			checkID = idValidation(deleteT, "P", "Item code");
		} while (checkID == 0);

		//Search for matching records
		do {
			for (int i = 0; i < *totalStock; i++) {
				if (strcmp(stock[i].itemCode, deleteT) == 0) {
					printf("\nThis is the record found: \n");
					tableLines();
					heading(1);
					printf("|%2d|%-9s|%-38s|RM %7.2f|%9d|%14d|%11d|  %02d/%02d/%4d|%11s|\n",
						i + 1, stock[i].itemCode, stock[i].description, stock[i].itemPrice,
						stock[i].stockQty, stock[i].minStock, stock[i].reorderQty,
						stock[i].lastRestock.day, stock[i].lastRestock.month, stock[i].lastRestock.year,
						stock[i].staffID);
					tableLines();
					deleteNo = i;
				}
			}

			//Shows no matching records
			if (deleteNo == -1) {
				printf("The following Stock item does not exist. Please reenter again.\n");
				printf("\n--- Record to be deleted ---\n");
				printf("Enter item code (P001): ");
				rewind(stdin);
				scanf("%[^\n]", &deleteT);
			}
		} while (deleteNo == -1);

		//Confirmation
		printf("Confirm to delete item (Y/N)? ");
		confirm = decision();

		//Cancel delete process
		if (confirm == 'N') {
			printf("\nCancelled deleting item\n");
			return;
		}

		//Delete item by shifting record to the front
		for (int i = deleteNo; i < *totalStock - 1; i++) {
			stock[i] = stock[i + 1];
		}
		(*totalStock)--;
		numDeleted++;
		rewriteStock(stock, *totalStock);
		printf("Item deleted successfully\n");

		//Continue to delete
		printf("Continue to delete item (Y/N)? ");
		proceed = decision();

	} while (proceed != 'N');
	printf("%d items deleted successfully\n", numDeleted);
}
void stockReport(Stock* stock, int totalStock) {
	Stock temp, search, searchResult[100];
	int numRecord = 0, totalQty = 0;
	int choice, valid;
	char invalid[50];
	double totalPrice = 0;

	system("cls");
	heading(7);

	time_t date = time(NULL);
	printf("\n%57s %s", "Report for:", ctime(&date));

	//Filter results
	do {
		printf("\n%73s\n", "STOCK STATUS REPORT");
		printf("%90s", "(1 = Insufficient stock, 2 = Sufficient Stock) > ");
		rewind(stdin);
		if (scanf("%d", &choice) != 1) {
			scanf("%[^\n]", &invalid);
			valid = 0;
			printf("\nError: '%s' is not an integer, please enter 1 or 2 only.\n\n", invalid);
		}
		else if (choice < 1 || choice > 2) {
			printf("\nError: '%d' is not an option, please enter 1 or 2 only.\n\n", choice);
			valid = 0;
		}
		else
			valid = 1;
	} while (valid == 0);

	//Filter results higher or lower
	search.stockQty = 300;
	switch (choice) {
	case 1:
		for (int i = 0; i < totalStock; i++) {
			if (search.stockQty >= stock[i].stockQty) {
				searchResult[numRecord] = stock[i];
				numRecord++;
			}
		}
		break;
	case 2:
		for (int i = 0; i < totalStock; i++) {
			if (search.stockQty <= stock[i].stockQty) {
				searchResult[numRecord] = stock[i];
				numRecord++;
			}
		}
		break;
	}

	//Sort according to stock quantity
	for (int i = 0; i < numRecord; i++) {
		for (int j = 0; j < numRecord - 1; j++) {
			if (searchResult[j].stockQty > searchResult[j + 1].stockQty) {
				temp = searchResult[j];
				searchResult[j] = searchResult[j + 1];
				searchResult[j + 1] = temp;
			}
		}
	}

	for (int i = 0; i < numRecord; i++) {
		totalPrice += searchResult[i].itemPrice;
		totalQty += searchResult[i].stockQty;
	}

	//Display filtered results
	tableLines();
	heading(1);
	displayStock(searchResult, numRecord);
	tableLines();

	switch (choice) {
	case 1:
		printf("\nTotal number of items with insufficient stock found: %d\n", numRecord);
		printf("Total quantity in stock found: %d\n", totalQty);
		printf("Total value of insufficient stock found: RM %.2f\n", totalPrice);
		break;
	case 2:
		printf("\nTotal number of items with sufficient stock found: %d\n", numRecord);
		printf("Total quantity in stock found: %d\n", totalQty);
		printf("Total value of sufficient stock found: RM%.2f\n", totalPrice);
		break;
	}
	printf("\n");
	totalQty = 0;
	totalPrice = 0;
	system("pause");
}

//Record Sorting Function
void sorting(Stock* stock, int totalStock) {
	Stock temp;
	char proceed;

	do {
		system("cls");

		tableLines();
		heading(1);
		displayStock(stock, 5);
		tableLines();
		printf("Preview of 5 rows before sorting.\n\n");
		system("pause");

		switch (menu(4)) {
		case 1:
			//Sort according to Item code
			for (int i = 0; i < totalStock; i++) {
				for (int j = 0; j < totalStock - 1; j++) {
					if (strcmp(stock[j].itemCode, stock[j + 1].itemCode) > 0) {
						temp = stock[j];
						stock[j] = stock[j + 1];
						stock[j + 1] = temp;
					}
				}
			}

			break;
		case 2:
			//Sort according to Item code
			for (int i = 0; i < totalStock; i++) {
				for (int j = 0; j < totalStock - 1; j++) {
					if (strcmp(stock[j].itemCode, stock[j + 1].itemCode) > 0) {
						temp = stock[j];
						stock[j] = stock[j + 1];
						stock[j + 1] = temp;
					}
				}
			}
			//Sort according to Item description
			for (int i = 0; i < totalStock; i++) {
				for (int j = 0; j < totalStock - 1; j++) {
					if (strcmp(stock[j].description, stock[j + 1].description) > 0) {
						temp = stock[j];
						stock[j] = stock[j + 1];
						stock[j + 1] = temp;
					}
				}
			}

			break;
		case 3:
			//Sort according to Item code
			for (int i = 0; i < totalStock; i++) {
				for (int j = 0; j < totalStock - 1; j++) {
					if (strcmp(stock[j].itemCode, stock[j + 1].itemCode) > 0) {
						temp = stock[j];
						stock[j] = stock[j + 1];
						stock[j + 1] = temp;
					}
				}
			}
			//Sort according to Item price
			for (int i = 0; i < totalStock; i++) {
				for (int j = 0; j < totalStock - 1; j++) {
					if (stock[j].itemPrice > stock[j + 1].itemPrice) {
						temp = stock[j];
						stock[j] = stock[j + 1];
						stock[j + 1] = temp;
					}
				}
			}

			break;
		case 4:
			//Sort according to Item code
			for (int i = 0; i < totalStock; i++) {
				for (int j = 0; j < totalStock - 1; j++) {
					if (strcmp(stock[j].itemCode, stock[j + 1].itemCode) > 0) {
						temp = stock[j];
						stock[j] = stock[j + 1];
						stock[j + 1] = temp;
					}
				}
			}
			//Sort according to stock quantity
			for (int i = 0; i < totalStock; i++) {
				for (int j = 0; j < totalStock - 1; j++) {
					if (stock[j].stockQty > stock[j + 1].stockQty) {
						temp = stock[j];
						stock[j] = stock[j + 1];
						stock[j + 1] = temp;
					}
				}
			}

			break;
		case 5:
			//Sort according to Item code
			for (int i = 0; i < totalStock; i++) {
				for (int j = 0; j < totalStock - 1; j++) {
					if (strcmp(stock[j].itemCode, stock[j + 1].itemCode) > 0) {
						temp = stock[j];
						stock[j] = stock[j + 1];
						stock[j + 1] = temp;
					}
				}
			}
			//Sort according to minimum stock level
			for (int i = 0; i < totalStock; i++) {
				for (int j = 0; j < totalStock - 1; j++) {
					if (stock[j].minStock > stock[j + 1].minStock) {
						temp = stock[j];
						stock[j] = stock[j + 1];
						stock[j + 1] = temp;
					}
				}
			}

			break;

		case 6:
			//Sort according to Item code
			for (int i = 0; i < totalStock; i++) {
				for (int j = 0; j < totalStock - 1; j++) {
					if (strcmp(stock[j].itemCode, stock[j + 1].itemCode) > 0) {
						temp = stock[j];
						stock[j] = stock[j + 1];
						stock[j + 1] = temp;
					}
				}
			}
			//Sort according to minimum stock level
			for (int i = 0; i < totalStock; i++) {
				for (int j = 0; j < totalStock - 1; j++) {
					if (stock[j].reorderQty > stock[j + 1].reorderQty) {
						temp = stock[j];
						stock[j] = stock[j + 1];
						stock[j + 1] = temp;
					}
				}
			}

			break;

		case 7:
			//Sort according to Item code
			for (int i = 0; i < totalStock; i++) {
				for (int j = 0; j < totalStock - 1; j++) {
					if (strcmp(stock[j].itemCode, stock[j + 1].itemCode) > 0) {
						temp = stock[j];
						stock[j] = stock[j + 1];
						stock[j + 1] = temp;
					}
				}
			}
			//Sort according to Last Restock Date
			for (int i = 0; i < totalStock; i++) {
				for (int j = 0; j < totalStock - 1; j++) {
					if (stock[j].lastRestock.year > stock[j + 1].lastRestock.year) {
						temp = stock[j];
						stock[j] = stock[j + 1];
						stock[j + 1] = temp;
					}
					else if (stock[j].lastRestock.year == stock[j + 1].lastRestock.year && stock[j].lastRestock.month > stock[j + 1].lastRestock.month) {
						temp = stock[j];
						stock[j] = stock[j + 1];
						stock[j + 1] = temp;
					}
					else if (stock[j].lastRestock.year == stock[j + 1].lastRestock.year && stock[j].lastRestock.month == stock[j + 1].lastRestock.month && stock[j].lastRestock.day > stock[j + 1].lastRestock.day) {
						temp = stock[j];
						stock[j] = stock[j + 1];
						stock[j + 1] = temp;
					}
				}
			}

			break;
		case 8:
			//Sort according to Item code
			for (int i = 0; i < totalStock; i++) {
				for (int j = 0; j < totalStock - 1; j++) {
					if (strcmp(stock[j].itemCode, stock[j + 1].itemCode) > 0) {
						temp = stock[j];
						stock[j] = stock[j + 1];
						stock[j + 1] = temp;
					}
				}
			}
			//Sort according to Staff ID
			for (int i = 0; i < totalStock; i++) {
				for (int j = 0; j < totalStock - 1; j++) {
					if (strcmp(stock[j].staffID, stock[j + 1].staffID) > 0) {
						temp = stock[j];
						stock[j] = stock[j + 1];
						stock[j + 1] = temp;
					}
				}
			}

			break;
		default:
			//Back to stock menu
			printf("\nGoing back to stock menu... ");
			return;
		}

		//Reset stock file and rewrite rearranged records
		rewriteStock(stock, totalStock);
		printf("Stock records rearranged successfully!\n");

		tableLines();
		heading(1);
		displayStock(stock, 5);
		tableLines();
		printf("Preview of 5 rows after sorting.\n\n");

		printf("Continue to sort by results (Y/N):");
		proceed = decision();
	} while (proceed != 'N');
}

//Write data into Stock Binary File
void appendStock(Stock* appendStock, int totalStock) {
	FILE* stockW;
	stockW = fopen("stock.bin", "ab");
	if (!stockW) {
		printf("Error opening file.");
		exit(-1);
	}

	fwrite(&appendStock[totalStock], sizeof(Stock), 1, stockW);

	fclose(stockW);
}

int readStock(Stock* stock) {
	FILE* stockR;
	stockR = fopen("stock.bin", "rb");
	if (!stockR) {
		printf("Error opening file.");
		exit(-1);
	}
	int totalStock = 0;

	while (fread(&stock[totalStock], sizeof(Stock), 1, stockR) == 1) {
		totalStock++;
	}

	fclose(stockR);
	return(totalStock);
}

//Rewrite stock binary file
void rewriteStock(Stock* rewriteStock, int totalStock) {
	FILE* stockRW;
	stockRW = fopen("stock.bin", "wb");
	if (!stockRW) {
		printf("Error, File could not be open");
		exit(-1);
	}

	for (int i = 0; i < totalStock; i++) {
		fwrite(&rewriteStock[i], sizeof(Stock), 1, stockRW);
	}

	fclose(stockRW);
}


//Check if new Item Code exists in stock file
int isExistingItem(char itemCode[6], Stock* stock, int totalStock) {
	for (int i = 0; i < totalStock; i++) {
		if (strcmp(itemCode, stock[i].itemCode) == 0) {
			return 1;
		}
	}
	printf("The item code you entered does not exist, please enter an existing Item Code from the stock module.\n");
	return 0;
}

//Check if Item Stock Quantity is at min level
int isMinimumLevel(char itemCode[6], Stock* stock, int totalStock) {
	for (int i = 0; i < totalStock; i++) {
		if (strcmp(itemCode, stock[i].itemCode) == 0 && stock[i].stockQty == stock[i].minStock) {
			printf("The item code you entered is at minimum stock level, please enter a different Item Code from the stock module.\n");
			return 0;
		}
	}
	return 1;
}

//Check if there is enough quantity of stock to be sold
int checkAvailableStock(int quantity, char itemCode[6], Stock* stock, int totalStock) {
	int j = 0;
	//Find the matching itemCode first
	for (int i = 0; i < totalStock; i++) {
		if (strcmp(itemCode, stock[i].itemCode) == 0) {
			j = i;
		}
	}

	//Check if the quantity sold exceeds the stock quantity min level
	if (stock[j].stockQty - quantity < stock[j].minStock) {
		printf("Insufficient stock quantity to carry out this transaction. Please try a different quantity.\n");
		return 0;
	}
	return 1;
}

//Minus Stock Quantity with Sales Quantity
void updateStock(int quantity, char itemCode[6], Stock* stock, int totalStock) {
	//Find the matching itemCode first
	for (int i = 0; i < totalStock; i++) {
		if (strcmp(itemCode, stock[i].itemCode) == 0) {
			//Update stock quantity with sales
			stock[i].stockQty = stock[i].stockQty - quantity;
		}
	}
	rewriteStock(stock, totalStock);
}


//Check for unique ID
int idUnique(Stock* stock, int totalStock, char id[5]) {
	for (int i = 0; i < totalStock; i++) {
		if (strcmp(id, stock[i].itemCode) == 0) {
			printf("Duplicate ID found! Please enter a different Item code.\n");
			return 0;
		}
	}
	return 1;
}

//Validation for IDs
int idValidation(char id[6], char idChar[3], char idChecked[]) {
	if (strlen(idChar) == 1) {
		if (strlen(id) != 4) {
			printf("The %s should contain exactly 4 characters.\n\n", idChecked);
			return 0;
		}
		else if (id[0] != idChar[0]) {
			printf("The %s should start with '%s', please enter again.\n\n", idChecked, idChar);
			return 0;
		}
		else if (!isdigit(id[1]) || !isdigit(id[2]) || !isdigit(id[3])) {
			printf("There should be 3 digits followed by the character(s) '%s', please enter again.\n\n", idChar);
			return 0;
		}
	}
	else if (strlen(idChar) == 2) {
		if (strlen(id) != 5) {
			printf("The %s should contain exactly 5 characters.\n\n", idChecked);
			return 0;
		}
		else if (id[0] != idChar[0] || id[1] != idChar[1]) {
			printf("The %s should start with '%s', please enter again.\n\n", idChecked, idChar);
			return 0;
		}
		else if (!isdigit(id[2]) || !isdigit(id[3]) || !isdigit(id[4])) {
			printf("There should be 3 digits followed by the character(s) '%s', please enter again.\n\n", idChar);
			return 0;
		}
	}

	return 1;
}

// IC Validation
int icValidation(char id[15]) {
	for (int i = 0; i < 14; i++)
	{
		if (strlen(id) != 14) {
			printf("IC should contain exactly 14 characters including dashes(-).\n\n");
			return 0;
		}
		else if (id[6] != '-' || id[9] != '-') {
			printf("Please enter in the following format with dashes(-): '001111-22-3333'.\n\n");
			return 0;
		}
		else if (!isdigit(id[i]) && i != 6 && i != 9) {
			printf("IC should contain exactly 12 numbers and 2 dashes in between.\n\n");
			return 0;
		}
		else if (id == "000000-00-0000") {
			printf("Invalid IC, please enter again.\n");
			return 0;
		}
	}
	return 1;
}

// Phone Num Validation
int phoneNumValidation(char phoneNum[12]) {
	for (int i = 0; i < 11; i++)
	{
		if (strlen(phoneNum) != 11) {
			printf("Phone number should contain exactly 11 characters including a dash(-) in between.\n\n");
			return 0;
		}
		else if (phoneNum[3] != '-') {
			printf("Please enter in the following format with a dash(-): '012-3459999'.\n\n");
			return 0;
		}
		else if (!isdigit(phoneNum[i]) && i != 3) {
			printf("Phone number should contain exactly 10 numbers and a dash in between.\n\n");
			return 0;
		}
		else if (phoneNum == "000-0000000") {
			printf("Invalid phone number, please enter again.\n");
			return 0;
		}
	}
	return 1;
}

//Validation for integer
int intValidation(char dataField[], int num, int min, int max) {
	if (num < min) {
		printf("Minimum %s should be at least %d, please enter a value of more than %d.\n", dataField, min, min);
		return 0;
	}
	else if (num > max) {
		printf("Maximum for %s is limited to %d, please enter a value of less than %d.\n", dataField, max, max + 1);
		return 0;
	}
	return 1;
}
//Validation for double
int decValidation(char dataField[], double num, int min, int max) {
	if (num < min) {
		printf("Our %s should be at least RM%d, please enter a value of more than %d.\n", dataField, min, min);
		return 0;
	}
	else if (num > max) {
		printf("Maximum for %s does not exceed RM%d, please enter a value of less than %d.\n", dataField, max, max + 1);
		return 0;
	}
	return 1;
}

//Date Validation
int dateValidation(int day, int month, int year) {
	int maxDays = 31;
	//Year Check
	if (year < 2013) {
		printf("Our branding, Adidas was established on 2013, please enter a year after 2013.\n");
		return 0;
	}
	else if (year > 2023) {
		printf("Please enter a valid year, between 2013 to 2023.\n");
		return 0;
	}
	//Month Check
	if (month < 1 || month > 12) {
		printf("Please enter a valid month of whole numbers (1-12).\n");
		return 0;
	}

	//Setting days according to months
	if (month == 4 || month == 6 || month == 9 || month == 11) {
		maxDays = 30;
	}
	else if (month == 2) {
		if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) {
			maxDays = 29;
		}
		else {
			maxDays = 28;
		}
	}

	//Day Check
	if (day < 1) {
		printf("Please enter a valid day...\n");
		return 0;
	}
	else if (day > maxDays) {
		printf("There are only %d for the %dth month , please enter again.\n", maxDays, month);
		return 0;
	}

	return 1;
}


//Display result of higher or lower numbers
int highOrLow() {
	int choice, valid;
	char invalid[50];

	do {
		printf("\nSearch for records higher and equal than this value or lower and equal than this value?\n");
		printf("(1 = Higher and Equal, 2 = Lower and Equal) > ");
		rewind(stdin);
		if (scanf("%d", &choice) != 1) {
			scanf("%[^\n]", &invalid);
			valid = 0;
			printf("\nError: '%s' is not an integer, please enter 1 or 2 only.\n\n", invalid);
		}
		else if (choice < 1 || choice > 2) {
			printf("\nError: '%d' is not an option, please enter 1 or 2 only.\n\n", choice);
			valid = 0;
		}
		else
			valid = 1;
	} while (valid == 0);

	return choice;
}

int beforeOrAfter(Stock* stock, int totalStock, Stock search, Stock* searchResult, int numRecord) {
	int choice, valid = 0;
	char invalid[50];

	do {
		printf("\nSearch for records starting from this date and after or before?\n\n");
		printf("(1 = This and After, 2 = This and Before) > ");
		rewind(stdin);
		if (scanf("%d", &choice) != 1) {
			scanf("%[^\n]", &invalid);
			choice = 0;
			printf("\nError: '%s' is not an integer, please enter a value of 1 - 2.\n\n", invalid);
		}
		else if (choice < 1 || choice > 2) {
			printf("\nError: '%d' is not an option, please enter a value of 1 - 2.\n\n", choice);
			valid = 0;
		}
		else
			valid = 1;
	} while (valid == 0);

	switch (choice) {
	case 1:
		for (int i = 0; i < totalStock; i++) {
			if (search.lastRestock.year < stock[i].lastRestock.year) {
				searchResult[numRecord] = stock[i];
				numRecord++;
			}
			else if (search.lastRestock.month < stock[i].lastRestock.month && search.lastRestock.year == stock[i].lastRestock.year) {
				searchResult[numRecord] = stock[i];
				numRecord++;
			}
			else if (search.lastRestock.day < stock[i].lastRestock.day && search.lastRestock.month == stock[i].lastRestock.month && search.lastRestock.year == stock[i].lastRestock.year) {
				searchResult[numRecord] = stock[i];
				numRecord++;
			}
			else if (search.lastRestock.day == stock[i].lastRestock.day && search.lastRestock.month == stock[i].lastRestock.month && search.lastRestock.year == stock[i].lastRestock.year) {
				searchResult[numRecord] = stock[i];
				numRecord++;
			}
		}

		break;

	case 2:
		for (int i = 0; i < totalStock; i++) {
			if (search.lastRestock.year > stock[i].lastRestock.year) {
				searchResult[numRecord] = stock[i];
				numRecord++;
			}
			else if (search.lastRestock.month > stock[i].lastRestock.month && search.lastRestock.year == stock[i].lastRestock.year) {
				searchResult[numRecord] = stock[i];
				numRecord++;
			}
			else if (search.lastRestock.day > stock[i].lastRestock.day && search.lastRestock.month == stock[i].lastRestock.month && search.lastRestock.year == stock[i].lastRestock.year) {
				searchResult[numRecord] = stock[i];
				numRecord++;
			}
			else if (search.lastRestock.day == stock[i].lastRestock.day && search.lastRestock.month == stock[i].lastRestock.month && search.lastRestock.year == stock[i].lastRestock.year) {
				searchResult[numRecord] = stock[i];
				numRecord++;
			}
		}
		break;
	}
	return numRecord;
}

void easter() {
	printf("== Watch Star Wars in Command Prompt ==\n");
	printf("Copy and paste the following\n");
	printf("Step 1: telnet\n");
	printf("Step 2: o\n");
	printf("Step 3: towel.blinkenlights.nl\n\n");
	system("pause");
}

// Menu
int menu(int type) {
	int choice = -1, num_choice = 0;

	switch (type) {
	case 0:
		//Main menu
		num_choice = 5;
		printf("=============================================\n");
		printf("||                A D I D A S               ||\n");
		printf("=============================================\n");
		printf("| 1.             Staff  Module              |\n");
		printf("|-------------------------------------------|\n");
		printf("| 2.             Member Module              |\n");
		printf("|-------------------------------------------|\n");
		printf("| 3.              Sales Module              |\n");
		printf("|-------------------------------------------|\n");
		printf("| 4.              Stock Module              |\n");
		printf("|-------------------------------------------|\n");
		printf("| 0.           Back to main menu            |\n");
		printf("=============================================\n");
		printf("        Welcome to Adidas Sales System       \n");
		break;

	case 1:
		//Stock menu
		num_choice = 8;
		printf("=============================================\n");
		printf("||          A D I D A S    S T O C K       ||\n");
		printf("=============================================\n");
		printf("| 1.            Add new item(s)             |\n");
		printf("|-------------------------------------------|\n");
		printf("| 2.         Display list of items          |\n");
		printf("|-------------------------------------------|\n");
		printf("| 3.           Search for item(s)           |\n");
		printf("|-------------------------------------------|\n");
		printf("| 4.           Modify item record           |\n");
		printf("|-------------------------------------------|\n");
		printf("| 5.          Delete item record            |\n");
		printf("|-------------------------------------------|\n");
		printf("| 6.             Stock report               |\n");
		printf("|-------------------------------------------|\n");
		printf("| 7.             Sort by record             |\n");
		printf("|-------------------------------------------|\n");
		printf("| 0.           Back to main menu            |\n");
		printf("=============================================\n");
		printf("Welcome to Adidas Sales System - Stock Module\n");
		break;

	case 2:
		//Search menu
		num_choice = 9;
		printf("*********************************************\n");
		printf("*             S E A R C H   B Y             *\n");
		printf("*********************************************\n");
		printf("* 1.              Item code                 *\n");
		printf("*-------------------------------------------*\n");
		printf("* 2.           Item description             *\n");
		printf("*-------------------------------------------*\n");
		printf("* 3.              Item price                *\n");
		printf("*-------------------------------------------*\n");
		printf("* 4.           Quantity in stock            *\n");
		printf("*-------------------------------------------*\n");
		printf("* 5.          Minimum stock level           *\n");
		printf("*-------------------------------------------*\n");
		printf("* 6.            Reorder quantity            *\n");
		printf("*-------------------------------------------*\n");
		printf("* 7.           Last restock date            *\n");
		printf("*-------------------------------------------*\n");
		printf("* 8.          Managed by Staff(ID)          *\n");
		printf("*-------------------------------------------*\n");
		printf("* 0.           Back to main menu            *\n");
		printf("*********************************************\n");
		break;

	case 3:
		//Modify menu
		num_choice = 9;
		printf("*********************************************\n");
		printf("*           M O D I F I C A T I O N         *\n");
		printf("*********************************************\n");
		printf("* 1.              Item code                 *\n");
		printf("*-------------------------------------------*\n");
		printf("* 2.           Item description             *\n");
		printf("*-------------------------------------------*\n");
		printf("* 3.              Item price                *\n");
		printf("*-------------------------------------------*\n");
		printf("* 4.           Quantity in stock            *\n");
		printf("*-------------------------------------------*\n");
		printf("* 5.          Minimum stock level           *\n");
		printf("*-------------------------------------------*\n");
		printf("* 6.            Reorder quantity            *\n");
		printf("*-------------------------------------------*\n");
		printf("* 7.           Last restock date            *\n");
		printf("*-------------------------------------------*\n");
		printf("* 8.          Managed by Staff(ID)          *\n");
		printf("*-------------------------------------------*\n");
		printf("* 0.           Back to main menu            *\n");
		printf("*********************************************\n");
		break;

	case 4:
		//Sort menu
		num_choice = 9;
		printf("*********************************************\n");
		printf("*              S O R T    B Y               *\n");
		printf("*********************************************\n");
		printf("* 1.              Item code                 *\n");
		printf("*-------------------------------------------*\n");
		printf("* 2.           Item description             *\n");
		printf("*-------------------------------------------*\n");
		printf("* 3.              Item price                *\n");
		printf("*-------------------------------------------*\n");
		printf("* 4.           Quantity in stock            *\n");
		printf("*-------------------------------------------*\n");
		printf("* 5.          Minimum stock level           *\n");
		printf("*-------------------------------------------*\n");
		printf("* 6.            Reorder quantity            *\n");
		printf("*-------------------------------------------*\n");
		printf("* 7.           Last restock date            *\n");
		printf("*-------------------------------------------*\n");
		printf("* 8.          Managed by Staff(ID)          *\n");
		printf("*-------------------------------------------*\n");
		printf("* 0.           Back to main menu            *\n");
		printf("*********************************************\n");
		break;

	default:
		printf("Invalid choice. Please enter again.\n");
		system("pause");
		break;
	}

	return menuChoice(num_choice);
}

//Return menu choices (1 to num_choice)
int menuChoice(int num_choice) {
	int choice;
	char invalid[51];

	do {
		printf("\nEnter an option (0 - %d) : ", num_choice - 1);
		rewind(stdin);

		//Invalid data type check
		if (scanf("%d", &choice) != 1) {
			scanf("%[^\n]", &invalid);
			choice = -1;
			printf("\nError: '%s' is not an integer, please enter a value of 0 - 7 only.\n\n", invalid);
		}
		//Out of range
		else if (choice < 0 || choice > num_choice) {
			printf("\nError: '%d' is not an option, 0 - 7 only.\n\n", choice);
		}

	} while (choice < 0 || choice > num_choice - 1);
	return choice;
}

//Return Yes or No choices
char decision() {
	char choice[51];
	do {
		rewind(stdin);
		scanf("%[^\n]", choice);
		if (strlen(choice) > 1) {
			printf("\nError '%s' is not a valid option, please enter Y or N.\n", choice);
		}
		else if (toupper(choice[0]) != 'Y' && toupper(choice[0]) != 'N') {
			printf("\nError '%c' is not a valid option, please enter Y or N.\n", choice[0]);
		}
	} while (toupper(choice[0]) != 'Y' && toupper(choice[0]) != 'N');
	return toupper(choice[0]);
}

//Display dashed lines (Table heading lines)
void tableLines() {
	for (int i = 0; i < 126; i++)
		printf("-");
	printf("\n");
}

void heading(int type) {
	switch (type) {
	case 1:
		printf("|%-2s|%9s|%-38s|%10s|%9s|%14s|%11s|%12s|%11s|\n", "No", "Item Code", "Description", "Price", "StockQty", "MinStockLevel", "ReorderQty", "LastRestock", "Managed by");
		printf("|%-2s|%9s|%-38s|%10s|%8s|%13s|%10s|%12s|%11s|\n", "==", "=========", "======================================", "==========", "=========", "==============", "===========", "============", "===========");
		break;

	case 2:
		printf("***************************************************************************\n");
		printf("**                        A D D I N G    I T E M                         **\n");
		printf("***************************************************************************\n");
		break;

	case 3:
		printf("%100s", "***************************************************************************\n");
		printf("%100s", "**                      D I S P L A Y I N G   I T E M                    **\n");
		printf("%100s", "***************************************************************************\n");
		break;

	case 4:
		printf("***************************************************************************\n");
		printf("**                   S E A R C H I N G   F O R   I T E M                 **\n");
		printf("***************************************************************************\n");
		break;

	case 5:
		printf("***************************************************************************\n");
		printf("**                       M O D I F Y I N G   I T E M                     **\n");
		printf("***************************************************************************\n");
		break;

	case 6:
		printf("***************************************************************************\n");
		printf("**                        D E L E T I N G   I T E M                      **\n");
		printf("***************************************************************************\n");
		break;

	case 7:
		printf("%100s", "***************************************************************************\n");
		printf("%100s", "**                          S T O C K   R E P O R T                      **\n");
		printf("%100s", "***************************************************************************\n");
		break;
	}
}

// Display logo
void logo()
{
	printf("    :::      :::::::::   ::::::::::  :::::::::       :::        ::::::::\n");
	printf("  :+: :+:    :+:    :+:     :+:      :+:    :+:    :+: :+:     +:     +: \n");
	printf(" +:+   +:+   +:+    +:+     +:+      +:+    +:+   +:+   +:+   +:+        \n");
	printf("+#++:++#++:  +#+    +:+     +#+      +#+    +:+  +#++:++#++:  +#++:++#++ \n");
	printf("+#+     +#+  +#+    +#+     +#+      +#+    +#+  +#+     +#+         +#+ \n");
	printf("#+#     #+#  #+#    #+#     #+#      #+#    #+#  #+#     #+#  #+     #+# \n");
	printf("###     ###  #########  ###########  #########   ###     ###   ########  \n");
	printf("*************************************************************************\n\n");
}

// Exit program
void exit_()
{
	SYSTEMTIME t;

	//output
	GetLocalTime(&t);
	printf("Exiting Adidas Sales System - at %d-%d-%d %d:%2d\n", t.wDay, t.wMonth, t.wYear, t.wHour, t.wMinute);
	printf("%19s %s\n", "", "+--------------------+");
	printf("%19s %s\n", "", "|  THANK YOU         |");
	printf("%19s %s\n", "", "|      BYE BYE       |");
	printf("%19s %s\n", "", "|        COME AGAIN  |");
	printf("%19s %s\n", "", "+--------------------+");
	printf("%23s %s\n", "", "(\\_/) ||");
	printf("%23s %s\n", "", "(^.^) ||");
	printf("%23s %s\n\n", "", " />---||");

	system("pause");
	//close program
}
