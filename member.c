#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<Windows.h>
#pragma warning(disable:4996)

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

void memberMenu();
void addMember(Member* member, int* memberNo);
void displayMember(Member* member, int memberNo);
int searchMember(Member* member, int memberNo);
void modifyMember(Member* member, int memberNo);
void deleteMember(Member* member, int* memberNo);
void memberReport(Member* member, int memberNo);

void appendMember(Member* appendMember, int memberNo);
int readMember(Member* member);
void rewriteMember(Member* member, int memberNo);
int idValidation(char id[6], char idChar[3], char idChecked[]);
int intValidation(char dataField[], int num, int min, int max);
int dateValidation(int day, int month, int year);
int phoneNumValidation(char phoneNum[12]);
int icValidation(char id[15]);
int memberIdUnique(Member* member, int memberNo, char ID[5]);
int idUnique1(Member* member, int memberNo, char ID[5]);
int idUnique2(Member* member, int memberNo, char ID[5]);
char decision();

int menuMember(int type);
void tableLinesMember();
void headingMember(int type);


void memberMenu() {

	Member member[100];

	int choice, num_items, found;

	do {
		choice = menuMember(1);
		num_items = readMember(member);
		//Switch to respective functions
		switch (choice) {
		case 1:
			system("cls");
			addMember(member, &num_items);
			break;
		case 2:
			system("cls");
			tableLinesMember();
			headingMember(1);
			displayMember(member, num_items);
			tableLinesMember();
			printf("Number of members : %d\n\n", num_items);

			system("pause");
			system("cls");
			break;

		case 3:
			found = searchMember(member, num_items);
			system("cls");

			break;
		case 4:
			system("cls");
			modifyMember(member, num_items);
			break;
		case 5:
			system("cls");
			deleteMember(member, &num_items);
			break;

		case 6:
			system("cls");
			printf("%70s", "===== M E M B E R =====\n\n");
			memberReport(member, num_items);
			system("pause");
			system("cls");

			break;
		case 0:
			return;

		default:
			printf("Invalid choice\n");
			break;
		}
	} while (choice != 0);

}

void addMember(Member* member, int* memberNo) {
	char decision = 'Y', confirm;
	int addNo = 0;
	int checkID;
	SYSTEMTIME t;

	do {
		headingMember(2);
		printf("\n--- Enter member %d details ---\n", *memberNo + 1);

		//Member ID
		do {
			printf("Enter Member ID (M101)                        : ");
			rewind(stdin);
			scanf("%[^\n]", &member[*memberNo].memberID);
			checkID = idValidation(member[*memberNo].memberID, "M", "ID");
			if (checkID == 1)
				checkID = memberIdUnique(member, *memberNo, member[*memberNo].memberID);
		} while (checkID == 0);

		//Member Name
		do {
			printf("Enter Member Name (Chong Yee Qing)            : ");
			rewind(stdin);
			scanf("%[^\n]", &member[*memberNo].name);
			checkID = (strlen(member[*memberNo].name) <= 40);
			if (checkID == 0)
				printf("Item description has to be within 40 characters, please enter again\n\n");
		} while (checkID == 0);

		//Member Gender
		do {
			printf("Enter Member Gender (M/F)                     : ");
			rewind(stdin);
			if (scanf("%c", &member[*memberNo].gender) != 1)
				printf("Error: Gender must be M or F!!!\n\n");
		} while (member[*memberNo].gender != 'M' && member[*memberNo].gender != 'F');

		//Member IC
		do {
			printf("Enter Member IC (112222-33-4444)              : ");
			rewind(stdin);
			scanf("%[^\n]", &member[*memberNo].IC);
			checkID = icValidation(member[*memberNo].IC);
		} while (checkID == 0);

		//Member Contact Number
		do {
			printf("Enter Member Contact Number (011-91239567)    : ");
			rewind(stdin);
			scanf("%[^\n]", &member[*memberNo].contact);
			checkID = phoneNumValidation(member[*memberNo].contact);
		} while (checkID == 0);

		//Member Upline ID
		do {
			printf("Enter Member Upline ID (U001)                 : ");
			rewind(stdin);
			scanf("%[^\n]", member[*memberNo].upLineID);
			checkID = idValidation(member[*memberNo].upLineID, "U", "ID");
			if (checkID == 1) {
				checkID = idUnique1(member, *memberNo, member[*memberNo].upLineID);
			}
		} while (checkID == 0);

		//Member Referral Upline ID
		do {
			printf("Enter Member Referral Upline ID (U001)        : ");
			rewind(stdin);
			scanf("%[^\n]", &member[*memberNo].referral);
			checkID = idValidation(member[*memberNo].referral, "U", "ID");
		} while (checkID == 0);

		//Member no of purchase
		do {
			printf("Enter Member Number Of Purchase (6)           : ");
			rewind(stdin);
			if (scanf("%d", &member[*memberNo].numOfPurchase) != 1)
				printf("Error: Number of purchase must be a number\n\n");
		} while (intValidation("Number of purchase", member[*memberNo].numOfPurchase, 1, 100) == 0);

		//Confirmation
		do {
			printf("\nConfirm to enter details (Y/N)? ");
			rewind(stdin);
			scanf("%c", &confirm);
			confirm = toupper(confirm);
			if (confirm != 'N' && confirm != 'Y')
				printf("\nError: '%c' is not a valid option, please enter (Y/N).\n", confirm);
		} while (confirm != 'N' && confirm != 'Y');

		//Add item 
		if (confirm == 'Y') {
			appendMember(member, *memberNo);
			(*memberNo)++;
			addNo++;
			GetLocalTime(&t);
			printf("\nMember record added successfully at %d-%d-%d %d:%2d\n\n", t.wDay, t.wMonth, t.wYear, t.wHour, t.wMinute);
		}
		//Clear details
		else {
			member[*memberNo] = member[*memberNo + 1];
			printf("\nCancelled adding item\n");
		}

		//Continue to add item
		system("pause");
		do {
			printf("Continue to add another item (Y/N)? ");
			rewind(stdin);
			scanf("%c", &decision);
			decision = toupper(decision);
			if (decision != 'N' && decision != 'Y')
				printf("\nError: '%c' is not a valid option, please enter (Y/N).\n", decision);
		} while (decision != 'N' && decision != 'Y');

		system("cls");
	} while (decision != 'N');

	printf("%d items has been added succesfully.\n", addNo);
}
void displayMember(Member* member, int memberNo) {
	for (int i = 0; i < memberNo; i++) {
		printf("%-2d|%-10s|%-20s|%-7c|%-15s|%-15s|%-10s|%-19s|%-4d\n", i + 1, member[i].memberID, member[i].name,
			member[i].gender, member[i].IC, member[i].contact, member[i].upLineID, member[i].referral, member[i].numOfPurchase);
	}
}
int searchMember(Member* member, int memberNo) {
	Member search, searchResult[100];
	char decision = 'Y';
	int	numRecord = 0, choice;
	int checkID;

	while (decision != 'N') {
		system("cls");
		printf("%70s", "===== M E M B E R =====\n\n");
		choice = menuMember(2);

		switch (choice) {
		case 1:
			//Search by member ID 
			do {
				headingMember(3);
				printf("Enter Member ID (M101)                     : ");
				rewind(stdin);
				scanf("%[^\n]", &search.memberID);
				checkID = idValidation(search.memberID, "M", "ID");
			} while (checkID == 0);

			for (int i = 0; i < memberNo; i++) {
				if (strcmp(member[i].memberID, search.memberID) == 0) {
					searchResult[numRecord] = member[i];
					numRecord++;
				}
			}

			break;

		case 2:
			//Search by member upline ID
			do {
				headingMember(4);
				printf("Enter Member Upline ID (U001)              : ");
				rewind(stdin);
				scanf("%[^\n]", &search.upLineID);
				checkID = idValidation(search.upLineID, "U", "ID");
			} while (checkID == 0);

			for (int i = 0; i < memberNo; i++) {
				if (strstr(member[i].upLineID, search.upLineID)) {
					searchResult[numRecord] = member[i];
					numRecord++;
				}
			}

			break;

		case 3:
			//Search by member referral upline ID
			do {
				headingMember(5);
				printf("Enter Member Referral Upline ID (U001)              : ");
				rewind(stdin);
				scanf("%[^\n]", &search.referral);
				checkID = idValidation(search.referral, "U", "ID");
			} while (checkID == 0);

			for (int i = 0; i < memberNo; i++) {
				if (strstr(member[i].referral, search.referral)) {
					searchResult[numRecord] = member[i];
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

		tableLinesMember();
		headingMember(1);
		displayMember(searchResult, numRecord);
		tableLinesMember();

		if (numRecord == 0)
			printf("No matching records found!\n\n");
		else
			printf("Found %d matching records!\n\n", numRecord);
		numRecord = 0;

		system("pause");
		//Continue to search
		do {
			printf("Continue to search for item (Y/N)? ");
			rewind(stdin);
			scanf("%c", &decision);
			decision = toupper(decision);
			if (decision != 'N' && decision != 'Y')
				printf("\nError: '%c' is not a valid option, please enter (Y/N).\n", decision);
		} while (decision != 'N' && decision != 'Y');
	}
	return numRecord;
}
void modifyMember(Member* member, int memberNo) {
	Member modify;
	int numModified = 0, modifyNo = 0, choice;
	char proceed = 'Y', confirm;
	char modifyT[21], checkID;
	SYSTEMTIME t;

	do {
		system("cls");
		//Display all
		printf("%70s", "===== M E M B E R =====\n\n");
		tableLinesMember();
		headingMember(1);
		displayMember(member, memberNo);
		tableLinesMember();

		//Search member code to modify
		do {
			printf("--- Record to be modified ---\n");
			printf("Enter member ID (M101): ");
			rewind(stdin);
			scanf("%[^\n]", &modifyT);
			checkID = idValidation(modifyT, "M", "ID");
		} while (checkID == 0);

		//Search for matching records
		do {
			for (int i = 0; i < memberNo; i++) {
				if (strcmp(member[i].memberID, modifyT) == 0) {
					printf("\nThis is the record found: \n");
					tableLinesMember();
					headingMember(1);
					printf("%-2d|%-10s|%-20s|%-7c|%-15s|%-15s|%-10s|%-19s|%-4d\n", i + 1, member[i].memberID, member[i].name,
						member[i].gender, member[i].IC, member[i].contact, member[i].upLineID, member[i].referral, member[i].numOfPurchase);
					tableLinesMember();
					modifyNo = i;
				}

			}

			//Shows no matching records
			if (modifyNo == -1) {
				printf("The following Member ID does not exist. Please reenter again.\n");
				do {
					printf("--- Record to be modified ---\n");
					printf("Enter member ID (M101): ");
					rewind(stdin);
					scanf("%[^\n]", &modifyT);
					checkID = idValidation(modifyT, "M", "ID");
				} while (checkID == 0);
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

		choice = menuMember(3);
		//Modification choice
		switch (choice) {
		case 1:
			// Member ID
			do {
				printf("Enter updated member ID (M101): ");
				rewind(stdin);
				scanf("%[^\n]", &modify.memberID);
				checkID = idValidation(modify.memberID, "M", "ID");
				if (checkID == 1)
					checkID = memberIdUnique(member, memberNo, modify.memberID);
			} while (checkID == 0);
			break;

		case 2:
			// Member name
			do {
				printf("Enter updated member name: ");
				rewind(stdin);
				scanf("%[^\n]", &modify.name);
				checkID = (strlen(modify.name) <= 40);
				if (checkID == 0)
					printf("Member name has to be within 40 characters, please enter again\n\n");
			} while (checkID == 0);
			break;

		case 3:
			// Member gender
			do {
				printf("Enter updated Member Gender (M/F)                   : ");
				rewind(stdin);
				if (scanf("%c", &modify.gender) != 1)
					printf("Error: Gender must be M or F!!!\n\n");
			} while (modify.gender != 'M' && modify.gender != 'F');
			break;

		case 4:
			//Member IC
			do {
				printf("Enter updated Member IC (112222-33-4444)              : ");
				rewind(stdin);
				scanf("%[^\n]", &modify.IC);
				checkID = icValidation(modify.IC);
			} while (checkID == 0);
			break;

		case 5:
			//Member contact
			do {
				printf("Enter updated Member Contact Number (011-91239567)  : ");
				rewind(stdin);
				scanf("%[^\n]", &modify.contact);
				checkID = phoneNumValidation(modify.contact);
			} while (checkID == 0);
			break;

		case 6:
			//Member upline ID
			do {
				printf("Enter updated Member Upline ID (U001)      : ");
				rewind(stdin);
				scanf("%[^\n]", &modify.upLineID);
				checkID = idValidation(modify.upLineID, "U", "ID");
				if (checkID == 1)
					checkID = idUnique1(member, memberNo, modify.upLineID);
			} while (checkID == 0);
			break;

		case 7:
			//Member upline ID
			do {
				printf("Enter updated Member Referral Upline ID (U001)      : ");
				rewind(stdin);
				scanf("%[^\n]", &modify.referral);
				checkID = idValidation(modify.referral, "U", "ID");
			} while (checkID == 0);
			break;

		case 8:
			//Member number of purchase
			do {
				printf("Enter updated Member Number of Purchase (6)   : ");
				rewind(stdin);
				scanf("%d", &modify.numOfPurchase);
				if (scanf("%d", &modify.numOfPurchase) != 1)
					printf("Error: Number of purchase must be a number\n\n");
			} while (intValidation("Number of purchase", modify.numOfPurchase, 1, 100) == 0);

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
			case 1: strcpy(member[modifyNo].memberID, modify.memberID); break;
			case 2: strcpy(member[modifyNo].name, modify.name); break;
			case 3: member[modifyNo].gender = modify.gender; break;
			case 4: strcpy(member[modifyNo].IC, modify.IC); break;
			case 5: strcpy(member[modifyNo].contact, modify.contact); break;
			case 6: strcpy(member[modifyNo].upLineID, modify.upLineID); break;
			case 7: strcpy(member[modifyNo].referral, modify.referral); break;
			case 8: member[modifyNo].numOfPurchase = modify.numOfPurchase; break;
			default:
				printf("Item not modified successfully\n"); system("pause"); return; break;
			}
			numModified++;
			rewriteMember(member, memberNo);
			GetLocalTime(&t);
			printf("\nMember record modified successfully at %d-%d-%d %d:%2d\n\n", t.wDay, t.wMonth, t.wYear, t.wHour, t.wMinute);

			for (int i = 0; i < memberNo; i++) {
				if (strcmp(member[i].memberID, modifyT) == 0) {
					tableLinesMember();
					headingMember(1);
					printf("%-2d|%-10s|%-20s|%-7c|%-15s|%-15s|%-10s|%-19s|%-4d\n", i + 1, member[i].memberID, member[i].name,
						member[i].gender, member[i].IC, member[i].contact, member[i].upLineID, member[i].referral, member[i].numOfPurchase);
					tableLinesMember();
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
void deleteMember(Member* member, int* memberNo) {
	int	numRecord = 0, numDeleted = 0, checkID, deleteNo = 0;
	// int choice;
	char decision = 'Y', confirm;
	char deleteT[21];
	SYSTEMTIME t;

	//Display all
	printf("%70s", "===== M E M B E R =====\n\n");
	tableLinesMember();
	headingMember(1);
	displayMember(member, *memberNo);
	tableLinesMember();

	do {
		//Search item code to delete
		do {
			printf("--- Record to be deleted ---\n");
			printf("Enter member ID (M101): ");
			rewind(stdin);
			scanf("%[^\n]", &deleteT);
			checkID = idValidation(deleteT, "M", "ID");
		} while (checkID == 0);

		//Search for matching records
		do {
			for (int i = 0; i < *memberNo; i++) {
				if (strcmp(member[i].memberID, deleteT) == 0) {
					printf("\nThis is the record found: \n");
					tableLinesMember();
					headingMember(1);
					printf("%-2d|%-10s|%-20s|%-7c|%-15s|%-15s|%-10s|%-19s|%-4d\n", i + 1, member[i].memberID, member[i].name,
						member[i].gender, member[i].IC, member[i].contact, member[i].upLineID, member[i].referral, member[i].numOfPurchase);
					tableLinesMember();
					deleteNo = i;
				}
			}

			//Shows no matching records
			if (deleteNo == -1) {
				printf("The following Member item does not exist. Please reenter again.\n");
				printf("\n--- Record to be deleted ---\n");
				printf("Enter member ID (M101): ");
				rewind(stdin);
				scanf("%[^\n]", &deleteT);
			}
		} while (deleteNo == -1);

		//Confirmation
		do {
			printf("Confirm to delete item (Y/N)? ");
			rewind(stdin);
			scanf("%c", &confirm);
			confirm = toupper(confirm);
			if (confirm != 'N' && confirm != 'Y')
				printf("\nError: '%c' is not a valid option, please enter (Y/N).\n", confirm);
		} while (confirm != 'N' && confirm != 'Y');

		//Cancel delete process
		if (confirm == 'N') {
			printf("\nCancelled deleting item\n");
			return;
		}

		//Delete item by shifting record to the front
		for (int i = deleteNo; i < *memberNo - 1; i++) {
			member[i] = member[i + 1];
		}
		(*memberNo)--;
		numDeleted++;
		rewriteMember(member, *memberNo);
		GetLocalTime(&t);
		printf("\nMember record deleted successfully at %d-%d-%d %d:%2d\n\n", t.wDay, t.wMonth, t.wYear, t.wHour, t.wMinute);

		//Continue to delete
		do {
			printf("Continue to delete item (Y/N)? ");
			rewind(stdin);
			scanf("%c", &decision);
			decision = toupper(decision);
			if (decision != 'N' && decision != 'Y')
				printf("\nError: '%c' is not a valid option, please enter (Y/N).\n", decision);
		} while (decision != 'N' && decision != 'Y');

	} while (decision != 'N');
	printf("%d items deleted successfully\n", numDeleted);
}
void memberReport(Member* member, int memberNo) {
	Member temp, search, searchResult[100];
	int numRecord = 0;
	int total = 0, choice, valid;
	char invalid[50];
	SYSTEMTIME t;

	do {
		printf("%70s", "===== M E M B E R =====\n");
		GetLocalTime(&t);
		printf("\n%55s %d-%d-%d %d:%02d\n\n", "Report on", t.wDay, t.wMonth, t.wYear, t.wHour, t.wMinute);
		tableLinesMember();

		printf("\n%-34s > %d\n", "Total number of member's ", memberNo);
		total = 0;
		for (int i = 0; i < memberNo; i++)
			total += member[i].numOfPurchase;
		printf("\n%-34s > %d\n\n", "Total number of purchase's ", total);
		printf("%80s", "*********************************************\n");
		printf("%80s", "*       M E M B E R (S)  R E P O R T        *\n");
		printf("%80s", "*********************************************\n");
		printf("%80s", "* 1.    Purchased lesser or equals to 5     *\n");
		printf("%80s", "*-------------------------------------------*\n");
		printf("%80s", "* 2.     Purchased more or equals to 5      *\n");
		printf("%80s", "*-------------------------------------------*\n");
		printf("%80s", "* 0.         Back to main menu              *\n");
		printf("%80s", "*********************************************\n");
		printf("\nEnter an option (0 - 2) : ");
		rewind(stdin);
		if (scanf("%d", &choice) != 1) {
			scanf("%[^\n]", invalid);
			valid = 0;
			printf("\nError: '%s' is not an integer, please enter 0 to 2 only.\n\n", invalid);
		}
		else if (choice < 0 || choice > 2) {
			printf("\nError: '%d' is not an option, please enter 0 or 2 only.\n\n", choice);
			valid = 0;
		}
		else
			valid = 1;
	} while (valid == 0);

	//Filter results higher or lower
	search.numOfPurchase = 5;
	switch (choice) {
	case 1:
		for (int i = 0; i < memberNo; i++) {
			if (search.numOfPurchase >= member[i].numOfPurchase) {
				searchResult[numRecord] = member[i];
				numRecord++;
			}
		}

		//Sort descending according to stock quantity
		for (int i = 0; i < numRecord; i++) {
			for (int j = 0; j < numRecord - 1; j++) {
				if (searchResult[j].numOfPurchase > searchResult[j + 1].numOfPurchase) {
					temp = searchResult[j];
					searchResult[j] = searchResult[j + 1];
					searchResult[j + 1] = temp;
				}
			}
		}
		break;
	case 2:
		for (int i = 0; i < memberNo; i++) {
			if (search.numOfPurchase <= member[i].numOfPurchase) {
				searchResult[numRecord] = member[i];
				numRecord++;
			}
		}

		//Sort ascending according to stock quantity
		for (int i = 0; i < numRecord; i++) {
			for (int j = 0; j < numRecord - 1; j++) {
				if (searchResult[j].numOfPurchase < searchResult[j + 1].numOfPurchase) {
					temp = searchResult[j];
					searchResult[j] = searchResult[j + 1];
					searchResult[j + 1] = temp;
				}
			}
		}
		break;
	case 0:
		return;
		break;
	}

	tableLinesMember();
	headingMember(1);
	displayMember(searchResult, numRecord);
	tableLinesMember();

	switch (choice) {
	case 1: printf("\n%-34s > %d\n", "Lowest number of purchase is", searchResult[0].numOfPurchase); break;
	case 2: printf("\n%-34s > %d\n", "Highest number of purchase is", searchResult[0].numOfPurchase); break;
	}

	system("pause");
	system("cls");
}


//Write data into Member Text File
void appendMember(Member* appendMember, int memberNo) {
	FILE* memberW;
	memberW = fopen("member.txt", "a");
	if (!memberW) {
		printf("Error opening file.");
		exit(-1);
	}

	fprintf(memberW, "%s|%s|%c|%s|%s|%s|%s|%d\n", appendMember[memberNo].memberID, appendMember[memberNo].name,
		appendMember[memberNo].gender, appendMember[memberNo].IC, appendMember[memberNo].contact, appendMember[memberNo].upLineID,
		appendMember[memberNo].referral, appendMember[memberNo].numOfPurchase);
	fclose(memberW);
	printf("\nData written to file successfully.\n");
}
int readMember(Member* member) {
	FILE* memberR;
	memberR = fopen("member.txt", "r");
	if (!memberR) {
		printf("Error opening file.");
		exit(-1);
	}

	int memberNo = 0;

	while (fscanf(memberR, "%[^|]|%[^|]|%c|%[^|]|%[^|]|%[^|]|%[^|]|%d\n", member[memberNo].memberID, member[memberNo].name,
		&member[memberNo].gender, member[memberNo].IC, member[memberNo].contact, member[memberNo].upLineID, member[memberNo].referral, &member[memberNo].numOfPurchase) != EOF) {
		memberNo++;
	}

	fclose(memberR);
	return memberNo;
}
void rewriteMember(Member* member, int memberNo) {
	FILE* memberRW;
	memberRW = fopen("member.txt", "w");
	if (!memberRW) {
		printf("Error opening file! Please check your file\n");
		exit(-1);
	}

	for (int i = 0; i < memberNo; i++) {
		fprintf(memberRW, "%s|%s|%c|%s|%s|%s|%s|%d\n", member[i].memberID, member[i].name,
			member[i].gender, member[i].IC, member[i].contact, member[i].upLineID, member[i].referral, member[i].numOfPurchase);
	}
	fclose(memberRW);
	printf("\nData has been successfully written to the file!!!\n");
}

//Validation for IDs
int memberIdUnique(Member* member, int memberNo, char ID[5]) {
	for (int i = 0; i < memberNo; i++) {
		if (strcmp(ID, member[i].memberID) == 0) {
			printf("Duplicate ID found! Please enter a different Member ID.\n");
			return 0;
		}
	}
	return 1;
}
int idUnique1(Member* member, int memberNo, char ID[5]) {
	for (int i = 0; i < memberNo; i++) {
		if (strcmp(ID, member[i].upLineID) == 0) {
			printf("Duplicate ID found! Please enter a different Upline ID.\n");
			return 0;
		}
	}
	return 1;
}
int idUnique2(Member* member, int memberNo, char ID[5]) {
	for (int i = 0; i < memberNo; i++) {
		if (strcmp(member[i].upLineID, member[memberNo].referral) == 0 && strcmp(member[i].upLineID, member[i].referral) != 0) {
			printf("Referral ID not found! Please enter a existing Upline ID.\n");
			return 0;
		}
	}
	return 1;
}

// Menu
int menuMember(int type) {
	int choice = -1;
	char invalid[31];

	switch (type) {
	case 1:
		//Member menu
		do {
			printf("=============================================\n");
			printf("||        A D I D A S    M E M B E R       ||\n");
			printf("=============================================\n");
			printf("| 1.            Add new member(s)           |\n");
			printf("|-------------------------------------------|\n");
			printf("| 2.        Display list of member(s)       |\n");
			printf("|-------------------------------------------|\n");
			printf("| 3.          Search for member(s)          |\n");
			printf("|-------------------------------------------|\n");
			printf("| 4.          Modify member record          |\n");
			printf("|-------------------------------------------|\n");
			printf("| 5.          Delete member record          |\n");
			printf("|-------------------------------------------|\n");
			printf("| 6.            Member report               |\n");
			printf("|-------------------------------------------|\n");
			printf("| 0.          Back to main menu             |\n");
			printf("=============================================\n");
			printf("\nEnter an option (0 - 6) : ");
			rewind(stdin);

			//Invalid data type check
			if (scanf("%d", &choice) != 1) {
				scanf("%[^\n]", &invalid);
				choice = -1;
				printf("\nError: '%s' is not an integer, please enter a value of 0 - 6 only.\n\n", invalid);
			}
			//Out of range
			else if (choice < 0 || choice > 6) {
				printf("\nError: '%d' is not an option, 0 - 6 only.\n\n", choice);
			}
		} while (choice < 0 || choice > 6);
		break;

	case 2:
		do {
			printf("*********************************************\n");
			printf("*            S E A R C H   B Y              *\n");
			printf("*********************************************\n");
			printf("* 1.            Member ID                   *\n");
			printf("*-------------------------------------------*\n");
			printf("* 2.            Upline ID                   *\n");
			printf("*-------------------------------------------*\n");
			printf("* 3.         Referral Upline ID             *\n");
			printf("*-------------------------------------------*\n");
			printf("* 0.         Back to main menu              *\n");
			printf("*********************************************\n");
			printf("\nEnter an option (0 - 3) : ");
			rewind(stdin);

			//Invalid data type check
			if (scanf("%d", &choice) != 1) {
				scanf("%[^\n]", &invalid);
				choice = -1;
				printf("\nError: '%s' is not an integer, please enter a value of 0 - 3 only.\n\n", invalid);
			}
			//Out of range
			else if (choice < 0 || choice > 3) {
				printf("\nError: '%d' is not an option, 0 - 3 only.\n\n", choice);
			}
		} while (choice < 0 || choice > 3);
		break;

		//Modify menu
	case 3:
		do {
			printf("*********************************************\n");
			printf("*           M O D I F I C A T I O N         *\n");
			printf("*********************************************\n");
			printf("* 1.              Member ID                 *\n");
			printf("*-------------------------------------------*\n");
			printf("* 2.              Member name               *\n");
			printf("*-------------------------------------------*\n");
			printf("* 3.             Member gender              *\n");
			printf("*-------------------------------------------*\n");
			printf("* 4.              Member IC                 *\n");
			printf("*-------------------------------------------*\n");
			printf("* 5.            Member contact              *\n");
			printf("*-------------------------------------------*\n");
			printf("* 6.           Member upline ID             *\n");
			printf("*-------------------------------------------*\n");
			printf("* 7.        Member referral upline ID       *\n");
			printf("*-------------------------------------------*\n");
			printf("* 8.        Member number of purchase       *\n");
			printf("*-------------------------------------------*\n");
			printf("* 0.            Back to main menu           *\n");
			printf("*********************************************\n");
			printf("\nEnter an option (0 - 8) : ");
			rewind(stdin);

			//Invalid data type check
			if (scanf("%d", &choice) != 1) {
				scanf("%[^\n]", &invalid);
				choice = 7;
				printf("\nError: '%s' is not an integer, please enter a value of 0 - 6 only.\n\n", invalid);
			}
			//Out of range
			else if (choice < 0 || choice > 8) {
				printf("\nError: '%d' is not an option, 0 - 7 only.\n\n", choice);
			}
		} while (choice < 0 || choice > 8);
		break;

	default:
		printf("Invalid choice. Please enter again.\n");
		system("pause");
		break;
	}

	return choice;
}

//Display dashed lines (Table heading lines)
void tableLinesMember() {
	for (int i = 0; i < 119; i++)
		printf("-");
	printf("\n");
}

void headingMember(int type) {
	switch (type) {
	case 1:
		printf("%100s", "***************************************************************************\n");
		printf("%100s", "**          D I S P L A Y I N G   M E M B E R (S)  D E T A I L S         **\n");
		printf("%100s", "***************************************************************************\n");
		printf("-----------------------------------------------------------------------------------------------------------------------\n");
		printf("%-2s|%-10s|%-20s|%-7s|%-15s|%-15s|%-10s|%-19s|%-4s\n", "No", "Member ID", "Name", "Gender", "IC", "Contact", "Upline ID", "Referral Upline ID", "No Of Purchase");
		printf("-----------------------------------------------------------------------------------------------------------------------\n");
		break;

	case 2:
		printf("%100s", "***************************************************************************\n");
		printf("%100s", "**                     A D D I N G    M E M B E R (S)                    **\n");
		printf("%100s", "***************************************************************************\n");
		printf("-----------------------------------------------------------------------------------------------------------------------\n");
		break;

	case 3:
		printf("%100s", "***************************************************************************\n");
		printf("%100s", "**              S E A R C H I N G   F O R   M E M B E R (S)              **\n");
		printf("%100s", "***************************************************************************\n");
		printf("-----------------------------------------------------------------------------------------------------------------------\n");

		break;

	case 4:
		printf("%100s", "***************************************************************************\n");
		printf("%100s", "**              S E A R C H I N G   F O R   U P L I N E (S)              **\n");
		printf("%100s", "***************************************************************************\n");
		printf("-----------------------------------------------------------------------------------------------------------------------\n");

		break;

	case 5:
		printf("%100s", "***************************************************************************\n");
		printf("%100s", "**     S E A R C H I N G   F O R   R E F E R R A L  U P L I N E (S)      **\n");
		printf("%100s", "***************************************************************************\n");
		printf("-----------------------------------------------------------------------------------------------------------------------\n");
		break;

	case 6:
		printf("%100s", "***************************************************************************\n");
		printf("%100s", "**                  M E M B E R (S)  R E P O R T                         **\n");
		printf("%100s", "***************************************************************************\n");
		printf("-----------------------------------------------------------------------------------------------------------------------\n");
		break;

	}
}