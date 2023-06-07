#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<Windows.h>
#pragma warning(disable:4996)

typedef struct {
	int day;
	int month;
	int year;
}Date;

typedef struct {
	char salesID[10];
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

void staffMain();

void home();
void logo();
void easter();
void exit_();

void home()
{
	char menuChoice;

	do {
		system("cls");
		logo();

		//input to view menu, sales order or exit
		printf("        Welcome to Adidas Sales System       \n\n");
		printf("View Easter Egg(V), Start System(S), Exit(E) > ");
		rewind(stdin);
		scanf("%c", &menuChoice);
		menuChoice = toupper(menuChoice);

		// Validation
		while (menuChoice != 'V' && menuChoice != 'S' && menuChoice != 'E') {
			printf("Invalid input!! Please reenter : ");
			rewind(stdin);
			scanf("%c", &menuChoice);
			menuChoice = toupper(menuChoice);
		}
		//switch to another function
		switch (menuChoice)
		{
		case 'V':
			system("cls");
			logo();
			easter();
			break;    // ADD SOMETHING :D
		case 'S':
			system("cls");
			staffMain();
			break;
		case 'E':
			system("cls");
			logo();
			exit_();
			break;
		default:
			printf("Invalid choice\n");
			system("pause");
			system("cls");
			break;
		}
	} while (menuChoice != 'E');
}

//Adidas Modules Main Menu
void main()
{
	home();
}