#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<conio.h>
#include<Windows.h>
#pragma warning(disable:4996)

void staffMain();
void loginstaff(int row);
void addStaff(int new);
void deleteStaff(int id, int row);
void modifyStaff(int id, int row);
void displayStaff(int id);
void staffMenu(int id);
void mainmenu(int id);
void report(int row);
void displayall(int row);

int idValidation(char id[6], char idChar[3], char idChecked[]);
int icValidation(char id[15]);

void logo();
void exit_();

void mainMenu();
void memberMenu();
void salesMenu();
void stockMenu();
int menu(int type);
int menuChoice(int num_choice);

typedef struct {
    int day;
    int month;
    int year;
}Date;

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
    char loginstaffid;
    char loginstaffpassw;
}Login;

typedef struct {
    char memberID[10];
    char name[60];
    char gender[2];
    char IC[20];
    int contact;
    char upLineID[10];
}Member;

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

Staff staff[100];

//STAFF MAIN//
////////////////////////////////////////////
void staffMain() {
    int loginchoice, row = 0, ext = 0;
    do {
        int line = 0;
        system("cls");
        logo();
        FILE* stfr;
        stfr = fopen("staff.txt", "r");
        if (stfr == NULL) {
            printf("Error opening file! Please check your file\n");
            exit(-1);
        }
        printf("STAFF LIST:\n");
        printf("====================================================================================\n");
        row = 0;
        while (!feof(stfr)) {
            fscanf(stfr, "|%[^|]|%[^|]|%[^|]|%[^|]|%c|%lf|%d|%d|%d|%d|\n", &staff[row].staffID, &staff[row].password,
                &staff[row].name, &staff[row].staffic, &staff[row].gender, &staff[row].salary, &staff[row].workminute,
                &staff[row].indate.day, &staff[row].indate.month, &staff[row].indate.year);
            printf("|%s|%20s|", staff[row].staffID, staff[row].name);

            line++;

            if (line == 3) {
                line = 0;
                printf("\n====================================================================================\n");
            }
            row++;
        }
        if (line == 1) {
            printf("|            -             ||            -             |\n====================================================================================\n");
        }
        else if (line == 2) {
            printf("|            -             |====================================================================================\n");
        }
        row = row - 1;
        printf("\n\n");
        fclose(stfr);
        printf("%65s","=============================================\n");
        printf("%65s", "||                Hello user!              ||\n");
        printf("%65s", "=============================================\n");
        printf("%65s", "| 1.                 Login                  |\n");
        printf("%65s", "|-------------------------------------------|\n");
        printf("%65s", "| 2.                Sign Up                 |\n");
        printf("%65s", "|-------------------------------------------|\n");
        printf("%65s", "| 3.                 Exit                   |\n");
        printf("%65s", "=============================================\n");
        printf("%55s", "Which Number you choose?(1-3): ");
        rewind(stdin);
        scanf("%d", &loginchoice);
        while (loginchoice != 1 && loginchoice != 2 && loginchoice != 3) {
            printf("Invalid number please re-enter!!!\n");
            printf("Which Number you choose?(1-3): ");
            rewind(stdin);
            scanf("%d", &loginchoice);
        }
        switch (loginchoice) {
        case 1:
            loginstaff(row);
            break;
        case 2:
            addStaff(row);
            break;

        case 3:
            ext = 1;
            system("cls");
            break;
        }
    } while (ext != 1);
    // Exit program
}
//LOGIN STAFF//
//////////////////////////////////////////
void loginstaff(int row) {
    int num = 0, validid = 0, validpass = 0, check;
    char insertid[6];
    char insertpass[100];
    char call[5];
    char confirm;
    int i;
    char input;
    FILE* stfr;
    stfr = fopen("staff.txt", "r");
    if (stfr == NULL) {
        printf("Error opening file! Please check your file\n");
        exit(-1);
    }
    do {
        system("cls");
        logo();
        printf("Login Page\n");
        printf("---------------\n");
        do {
            i = 0;
            printf("Enter your ID(SFXXX):");
            rewind(stdin);
            scanf("%[^\n]", insertid);
            check = idValidation(insertid, "SF", "Staff ID");
        } while (check == 0);

        printf("Enter your password (XXXXXXX):");
        rewind(stdin);
        while (1) {
            input = getch();
            if (input == '\r') {
                break;
            }
            else if (input == '\b' && i > 0) {
                i--;
                printf("\b \b");
            }
            else if (input != '\b') {
                insertpass[i] = input;
                i++;
                printf("*");
            }
        }

        insertpass[i] = '\0';
        printf("\n");
        while (strlen(insertpass) != 7) {
            printf("The password should contain exactly 7 characters.\n");
            printf("\nEnter your password (XXXXXXX):");
            rewind(stdin);
            i = 0;
            while (1) {
                input = getch();
                if (input == '\r') {
                    break;
                }
                else if (input == '\b' && i > 0) {
                    i--;
                    printf("\b \b");
                }
                else if (input != '\b') {
                    insertpass[i] = input;
                    i++;
                    printf("*");
                }
            }
            insertpass[i] = '\0';
        }
        validid = 0, validpass = 0;
        for (num = 0; num <= row; num++) {
            if (strcmp(insertid, staff[num].staffID) == 0) {
                validid = 1;
                if (strcmp(insertpass, staff[num].password) == 0) {
                    validpass = 1;
                    mainmenu(num);
                    confirm = 'Y';
                    break;
                }
                else {
                    confirm = 'N';
                }
            }
            else {
                confirm = 'N';
            }
        }
        if (validid == 0) {
            printf("Wrong Type of ID!!Please re-enter\n");
            confirm = 'N';
            system("pause");
        }
        else if (validpass == 0) {
            printf("Wrong password!!!Re-enter\n");
            confirm = 'N';
            system("pause");
        }
    } while (toupper(confirm) != 'Y');
}
//ADD STAFF//
//////////////////////////////////////////
void addStaff(int new) {

    int num;
    char gend;
    FILE* stfw;
    FILE* stnumr;
    FILE* stnumw;
    stnumr = fopen("contantsid.txt", "r");
    stnumw = fopen("contantsid.txt", "w");
    stfw = fopen("staff.txt", "a");
    if (stfw == NULL) {
        printf("Error opening file! Please check your file\n");
        exit(-1);
    }
    char confirm;
    char confirm1;

    do {
        system("cls");
        logo();
        int icvalid;
        printf("Sign in? (Y = Yes/ N = No): ");
        rewind(stdin);
        scanf("%c", &confirm);
        //validation
        while (toupper(confirm) != 'N' && toupper(confirm) != 'Y') {
            printf("Invalid! Please key in accordingly\n");
            printf("Sign in? (Y = Yes/ N = No): ");
            rewind(stdin);
            scanf("%c", &confirm);
        }
        if (toupper(confirm) == 'Y') {
            num = new + 1;
            fprintf(stnumw, "SF%03d\n", num + 1);
            fclose(stnumw);
            fscanf(stnumr, "%[^\n]", staff[num].staffID);
            fclose(stnumr);
            printf("Your ID is %s\n", staff[num].staffID);
            printf("Enter your Password (XXXXXXX):");
            rewind(stdin);
            scanf("%[^\n]", &staff[num].password);
            while (strlen(staff[num].password) != 7) {
                printf("The %s should contain exactly 7 characters.\n", staff[num].password);
                printf("Enter your Password (XXXXXXX):");
                rewind(stdin);
                scanf("%[^\n]", staff[num].password);
            }
            printf("Enter staff name: ");
            rewind(stdin);
            scanf("%[^\n]", staff[num].name);
            do {
                printf("Enter staff IC: ");
                rewind(stdin);
                scanf("%[^\n]", staff[num].staffic);
                icvalid = icValidation(staff[num].staffic);
            } while (icvalid != 1);
            printf("Enter staff Gender(M/F): ");
            rewind(stdin);
            scanf("%c", &gend);
            staff[num].gender = toupper(gend);
            while (staff[num].gender != 'M' && staff[num].gender != 'F') {
                printf("Just only can enter M or F!!!!\n");
                printf("Enter staff Gender(M/F): ");
                rewind(stdin);
                scanf("%c", &gend);
                staff[num].gender = toupper(gend);
            }

            printf("Enter staff salary: ");
            rewind(stdin);
            scanf("%lf", &staff[num].salary);

            while (staff[num].salary == 0 || staff[num].salary > 99999.99) {
                printf("Invalid salary please insert again!!\n");
                printf("Enter staff salary: ");
                rewind(stdin);
                scanf("%lf", &staff[num].salary);
            }
            printf("Enter staff Working Minute (=>420):");
            rewind(stdin);
            scanf("%d", &staff[num].workminute);
            while (staff[num].workminute == 419 || staff[num].workminute > 1000) {
                printf("No Enough Time please re-enter!!!!\n");
                printf("Enter staff Working Hour:");
                rewind(stdin);
                scanf("%d", &staff[num].workminute);
            }
            SYSTEMTIME time;
            GetLocalTime(&time);
            staff[num].indate.year = time.wYear;
            staff[num].indate.day = time.wDay;
            staff[num].indate.month = time.wMonth;
            printf("Today Your Sign in Date is %02d-%02d-%d\n", staff[num].indate.day, staff[num].indate.month, staff[num].indate.year);
            printf("Confirm add info? (Y = Yes/N = No): ");
            rewind(stdin);
            scanf("%c", &confirm1);

            //validation
            while (toupper(confirm1) != 'N' && toupper(confirm1) != 'Y') {
                printf("Invalid! Please key in accordingly\n");
                printf("Confirm add info? (Y = Yes/ N = No): ");
                rewind(stdin);
                scanf("%c", &confirm1);
                fclose(stfw);
            }

            if (toupper(confirm1) == 'Y') {
                fprintf(stfw, "|%s|%s|%s|%s|%c|%0.2lf|%d|%02d|%02d|%02d|\n", staff[num].staffID, staff[num].password,
                    staff[num].name, staff[num].staffic, staff[num].gender, staff[num].salary, staff[num].workminute,
                    staff[num].indate.day, staff[num].indate.month, staff[num].indate.year);
                confirm = 'N';
                printf("Welcome to our company! %s\n", staff[num].name);
                fclose(stfw);
                mainmenu(num);

            }
            else {
                printf("\nYou have cancel your sign in\n");
                confirm1 = ' ';
            }
        }
    } while (toupper(confirm) != 'N');
}
//MAIN MENU//
///////////////////////////////////////////
void mainmenu(int id) {
    int select;
    char check[45];

    do {
        system("cls");
        printf("Welcome to Main menu %s\n\n", staff[id].name);
        printf("=============================================\n");
        printf("||                A D I D A S              ||\n");
        printf("=============================================\n");
        printf("| 1.             Staff  Module              |\n");
        printf("|-------------------------------------------|\n");
        printf("| 2.             Member Module              |\n");
        printf("|-------------------------------------------|\n");
        printf("| 3.              Sales Module              |\n");
        printf("|-------------------------------------------|\n");
        printf("| 4.              Stock Module              |\n");
        printf("|-------------------------------------------|\n");
        printf("| 5.           Back to login page           |\n");
        printf("=============================================\n");
        printf("        Welcome to Adidas Sales System       \n");
        printf("\nEnter an option [1-5]: ");
        rewind(stdin);
        //Invalid data type check
        if (scanf("%d", &select) != 1) {
            scanf("%[^\n]", &check);
            select = 6;
            printf("\nError: '%s' is not an integer! Please enter a value of 1 - 5 only.\n\n", check);
        }
        //Out of range
        else if (select < 1 || select > 5) {
            printf("\nError: '%d' is not an option! Please enter a value of 1 - 5 only.\n\n", select);
        }

        //Switch to respective functions
        switch (select) {
        case 1:
            system("cls");
            staffMenu(id);
            system("cls");
            break;
        case 2:
            system("cls");
            memberMenu();
            system("cls");
            break;

        case 3:
            system("cls");
            salesMenu();
            system("cls");
            break;
        case 4:
            system("cls");
            stockMenu();
            system("cls");
            break;
        case 5:
            return;
            break;
        }
    } while (select != 5);
}
//STAFF MENU//
////////////////////////////////////////////
void staffMenu(int id) {

    int select, row;
    char confirm;
    FILE* stfr;
    stfr = fopen("staff.txt", "r");
    if (stfr == NULL) {
        printf("Error opening file! Please check your file\n");
        exit(-1);
    }
    row = 0;
    while (!feof(stfr)) {
        fscanf(stfr, "|%[^|]|%[^|]|%[^|]|%[^|]|%c|%lf|%d|%d|%d|%d|\n", &staff[row].staffID, &staff[row].password,
            &staff[row].name, &staff[row].staffic, &staff[row].gender, &staff[row].salary
            , &staff[row].workminute, &staff[row].indate.day, &staff[row].indate.month, &staff[row].indate.year);
        row++;
    }
    row = row - 1;
    fclose(stfr);
    do {
        system("cls");
        logo();
        printf("Welcome to Staff menu %s\n", staff[id].name);
        printf("*********************************************\n");
        printf("*                 Staff info                *\n");
        printf("*********************************************\n");
        printf("* 1.             Modify info                *\n");
        printf("*-------------------------------------------*\n");
        printf("* 2.           Display your info            *\n");
        printf("*-------------------------------------------*\n");
        printf("* 3.             Delete info                *\n");
        printf("*-------------------------------------------*\n");
        printf("* 4.             Display Report             *\n");
        printf("*-------------------------------------------*\n");
        printf("* 5.             Display all                *\n");
        printf("*********************************************\n");
        printf("* 6.          Back To Main Menu             *\n");
        printf("*********************************************\n");
        printf("\nPlease select [1-6]: ");
        rewind(stdin);
        scanf("%d", &select);

        //validation
        while (select != 1 && select != 2 && select != 3 && select != 4 && select != 5 && select != 6) {
            system("cls");
            printf("Invalid! Please key in accordingly\n");
            printf("Welcome to Staff Menu %s\n", staff[id].name);
            printf("*********************************************\n");
            printf("*                 Staff info                *\n");
            printf("*********************************************\n");
            printf("* 1.             Modify info                *\n");
            printf("*-------------------------------------------*\n");
            printf("* 2.           Display your info            *\n");
            printf("*-------------------------------------------*\n");
            printf("* 3.             Delete info                *\n");
            printf("*-------------------------------------------*\n");
            printf("* 4.           Display Report               *\n");
            printf("*-------------------------------------------*\n");
            printf("* 5.             Display all                *\n");
            printf("*********************************************\n");
            printf("* 6.          Back To Main Menu             *\n");
            printf("*********************************************\n");
            printf("\nPlease select [1-6]: ");
            rewind(stdin);
            scanf("%d", &select);
        }


        switch (select) {
        case 1:
            system("cls");
            modifyStaff(id, row);
            break;
        case 2:
            system("cls");
            displayStaff(id);
            break;
        case 3:
            system("cls");
            deleteStaff(id, row);
            break;
        case 4:
            report(row);
            break;
        case 5:
            system("cls");
            displayall(row);
            break;
        case 6:
            return;
            break;

        }

        printf("Continue using? (Y = Yes/ N = No): ");
        rewind(stdin);
        scanf("%c", &confirm);

        confirm = toupper(confirm);

        //validation
        while (confirm != 'N' && confirm != 'Y') {

            printf("Invalid! Please key in accordingly\n");
            printf("Continue using? (Y = Yes/ N = No): ");
            rewind(stdin);
            scanf("%c", &confirm);
        }
        system("cls");
    } while (confirm != 'N');
}
//MODIFYSTAFF//
////////////////////////////////////////////
void modifyStaff(int id, int row) {
    int choice, newworktime, num;
    char newpass[8], confirm, ans, newname[60];
    double newsalary;
    do {
        system("cls");
        logo();
        printf("Modify Staff info? (Y = Yes/ N = No): ");
        rewind(stdin);
        scanf("%c", &ans);

        //validation
        while (toupper(ans) != 'N' && toupper(ans) != 'Y') {
            printf("Invalid! Please key in accordingly\n");
            printf("Modify Staff info? (Y = Yes/ N = No): ");
            rewind(stdin);
            scanf("%c", &ans);
        }

        if (toupper(ans) == 'Y') {
            printf("Welcome te Staff Modify  %s\n", staff[id].name);
            printf("*********************************************\n");
            printf("*               Modify Staff                *\n");
            printf("*********************************************\n");
            printf("* 1.              Password                  *\n");
            printf("*-------------------------------------------*\n");
            printf("* 2.                Name                    *\n");
            printf("*-------------------------------------------*\n");
            printf("* 3.               Salary                   *\n");
            printf("*-------------------------------------------*\n");
            printf("* 4.          Work Time in Minutes          *\n");
            printf("*-------------------------------------------*\n");
            printf("* 5.               Cancel                   *\n");
            printf("*********************************************\n");
            printf("Insert your choice [1-5]:");
            rewind(stdin);
            scanf("%d", &choice);
            while (choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5) {
                system("cls");
                printf("Invalid please re-enter!!!!\n");
                printf("Welcome te Staff Modify  %s\n", staff[id].name);
                printf("*********************************************\n");
                printf("*               Modify Staff                *\n");
                printf("*********************************************\n");
                printf("* 1.              Password                  *\n");
                printf("*-------------------------------------------*\n");
                printf("* 2.                Name                    *\n");
                printf("*-------------------------------------------*\n");
                printf("* 3.               Salary                   *\n");
                printf("*-------------------------------------------*\n");
                printf("* 4.          Work Time in Minutes          *\n");
                printf("*-------------------------------------------*\n");
                printf("* 5.               Cancel                   *\n");
                printf("*********************************************\n");
                printf("Insert your choice [1-5]:");
                rewind(stdin);
                scanf("%d", &choice);

            }
            switch (choice) {
            case 1:
                do {
                    printf("Change password\n");
                    printf("-----------------\n");
                    printf("New Password (XXXXXXX):");
                    rewind(stdin);
                    scanf("%[^\n]", &newpass);
                    while (strlen(newpass) != 7) {
                        printf("The %s should contain exactly 7 characters.\n\n", newpass);
                        printf("New Password (XXXXXXX):");
                        rewind(stdin);
                        scanf("%[^\n]", newpass);
                    }
                    printf("This is your new password |%s|?(Y=Yes/N=No)", newpass);
                    rewind(stdin);
                    scanf("%c", &confirm);
                    while (toupper(confirm) != 'Y' && toupper(confirm) != 'N') {
                        printf("Invalid only can enter Y or N)\n");
                        printf("This is your new password |%s|?(Y=Yes/N=No)", newpass);
                        rewind(stdin);
                        scanf("%c", &confirm);
                    }
                    if (toupper(confirm) == 'Y') {
                        strcpy(staff[id].password, newpass);
                    }
                } while (toupper(confirm) != 'Y');

                break;
            case 2:
                do {
                    printf("Change Name\n");
                    printf("-----------------\n");
                    printf("New Name:");
                    rewind(stdin);
                    scanf("%[^\n]", &newname);
                    printf("This is your new Name |%s|?(Y=Yes/N=No)", newname);
                    rewind(stdin);
                    scanf("%c", &confirm);
                    while (toupper(confirm) != 'Y' && toupper(confirm) != 'N') {
                        printf("Invalid only can enter Y or N)\n");
                        printf("This is your new Name |%s|?(Y=Yes/N=No)", newname);
                        rewind(stdin);
                        scanf("%c", &confirm);
                    }
                    if (toupper(confirm) == 'Y') {
                        strcpy(staff[id].name, newname);
                    }
                } while (toupper(confirm) != 'Y');

                break;
            case 3:
                do {
                    printf("Change Salary\n");
                    printf("-----------------\n");
                    printf("New Salary:");
                    rewind(stdin);
                    scanf("%lf", &newsalary);
                    while (newsalary == 0 || newsalary > 99999.99) {
                        printf("Invalid salary please insert again!!\n");
                        printf("New Salary: ");
                        rewind(stdin);
                        scanf("%lf", &newsalary);
                    }
                    printf("This is your new Salary |%0.2lf|?(Y=Yes/N=No)\n", newsalary);
                    rewind(stdin);
                    scanf("%c", &confirm);
                    while (toupper(confirm) != 'Y' && toupper(confirm) != 'N') {
                        printf("Invalid only can enter Y or N)\n");
                        printf("This is your new Salary |%0.2lf|?(Y=Yes/N=No)", newsalary);
                        rewind(stdin);
                        scanf("%c", &confirm);
                    }
                    if (toupper(confirm) == 'Y') {
                        staff[id].salary = newsalary;
                    }
                } while (toupper(confirm) != 'Y');

                break;
            case 4:
                do {
                    printf("Change Work Time in Minutes\n");
                    printf("-----------------\n");
                    printf("New Work Time in Minutes:");
                    rewind(stdin);
                    scanf("%d", &newworktime);
                    while (newworktime == 419 || newworktime > 1000) {
                        printf("No Enough Time please re-enter!!!!\n");
                        printf("New Work Time in Minutes:");
                        rewind(stdin);
                        scanf("%d", &newworktime);
                    }
                    printf("This is your new Work Time in Minutes |%d|?(Y=Yes/N=No)", newworktime);
                    rewind(stdin);
                    scanf("%c", &confirm);
                    while (toupper(confirm) != 'Y' && toupper(confirm) != 'N') {
                        printf("Invalid only can enter Y or N)\n");
                        printf("This is your new Work Time in Minutes |%d|?(Y=Yes/N=No)", newworktime);
                        rewind(stdin);
                        scanf("%c", &confirm);
                    }
                    if (toupper(confirm) == 'Y') {
                        staff[id].workminute = newworktime;
                    }
                } while (toupper(confirm) != 'Y');
                break;
            case 5:
                return;
                break;
            }
            FILE* stfw;
            stfw = fopen("staff.txt", "w");
            if (stfw == NULL) {
                printf("Error opening file! Please check your file\n");
                exit(-1);
            }
            for (num = 0; num < row + 1; num++) {
                fprintf(stfw, "|%s|%s|%s|%s|%c|%0.2lf|%d|%d|%d|%d|\n", staff[num].staffID, staff[num].password, staff[num].name, staff[num].staffic, staff[num].gender,
                    staff[num].salary, staff[num].workminute, staff[num].indate.day, staff[num].indate.month, staff[num].indate.year);
            }
            fclose(stfw);
            printf("\nWant to modify again? (Y=yes/N=No): ");
            rewind(stdin);
            scanf("%c", &ans);
        }

    } while (toupper(ans) != 'N');
}
//DISPLAYSTAFF//
////////////////////////////////////////////
void displayStaff(int id) {
    system("cls");
    logo();
    printf("Display staff (%s,%s)\n", staff[id].staffID, staff[id].name);
    printf("=============================================================\n");
    printf("|%s|%s|%s|%s|%c|%0.2lf|%d|%02d-%02d-%02d|\n", staff[id].staffID, staff[id].password, staff[id].name,
        staff[id].staffic, staff[id].gender, staff[id].salary, staff[id].workminute, staff[id].indate.day, staff[id].indate.month, staff[id].indate.year);
    printf("=============================================================\n");

}
//DELETESTAFF//
//////////////////////////////////////////
void deleteStaff(int id, int row) {
    char ans, nameid[10], confirm;
    FILE* ptw;
    system("cls");
    logo();
    printf("Welcome te Delete Staff  %s\n", staff[id].name);
    printf("Delete Staff info? (Y = Yes/ N = No): ");
    rewind(stdin);
    scanf("%c", &ans);

    //validation
    while (toupper(ans) != 'N' && toupper(ans) != 'Y') {
        printf("Invalid! Please key in accordingly\n");
        printf("Delete Staff info? (Y = Yes/ N = No): ");
        rewind(stdin);
        scanf("%c", &ans);
    }

    if (toupper(ans) == 'Y') {
        printf("Do You Want To Delete your Staff Detail?(Y=Yes/N=NO): ");
        rewind(stdin);
        scanf("%c", &confirm);
        while (toupper(confirm) != 'N' && toupper(confirm) != 'Y') {
            printf("Invalid! Please key in accordingly\n");
            printf("Do You Want To Delete your Staff Detail?(Y=Yes/N=NO): ");
            rewind(stdin);
            scanf("%c", &confirm);
        }
        if (toupper(confirm) != 'y') {
            for (id; id < row; id++)
            {
                strcpy(staff[id].password, staff[id + 1].password);
                strcpy(staff[id].name, staff[id + 1].name);
                strcpy(staff[id].staffic, staff[id + 1].staffic);
                staff[id].gender = staff[id + 1].gender;
                staff[id].salary = staff[id + 1].salary;
                staff[id].workminute = staff[id + 1].workminute;
                staff[id].indate.day = staff[id + 1].indate.day;
                staff[id].indate.month = staff[id + 1].indate.month;
                staff[id].indate.year = staff[id + 1].indate.year;
            }
            ptw = fopen("staff.txt", "w");
            for (int num = 0; num < row; num++) {
                fprintf(ptw, "|%s|%s|%s|%s|%c|%0.2lf|%d|%d|%d|%d|\n", staff[num].staffID, staff[num].password, staff[num].name, staff[num].staffic, staff[num].gender,
                    staff[num].salary, staff[num].workminute, staff[num].indate.day, staff[num].indate.month, staff[num].indate.year);
                confirm = ' ';
            }
            fclose(ptw);
        }
    }
}
//DISPLAY ALL//
//////////////////////////////////////////
void displayall(int row) {
    int num;
    printf("All The Staff Details\n");
    for (num = 0; num <= row; num++) {
        printf("===================================================================\n");
        printf("|%s|%18s|%s|%c|%0.2lf|%d|%02d-%02d-%02d|\n", staff[num].staffID, staff[num].name,
            staff[num].staffic, staff[num].gender, staff[num].salary, staff[num].workminute, staff[num].indate.day, staff[num].indate.month, staff[num].indate.year);
        printf("===================================================================\n");
    }
}
//ALL Salary between lest than RM2000 and more than RM3000//
///////////////////////////////////////////
void report(int row) {
    int count;
    system("cls");
    logo();
    printf("Staff salary more than RM3000\n");
    for (count = 0; count <= row; count++) {
        if (staff[count].salary > 3000) {
            printf("-------------------------------------------------\n");
            printf("|%s|%18s|%s|%0.2lf|\n", staff[count].staffID, staff[count].name, staff[count].staffic, staff[count].salary);
            printf("-------------------------------------------------\n");
        }
    }
    printf("\n");
    printf("Staff salary less than RM3000\n");
    for (count = 0; count <= row; count++) {
        if (staff[count].salary < 3000) {
            printf("-------------------------------------------------\n");
            printf("|%s|%18s|%s|%0.2lf|\n", staff[count].staffID, staff[count].name, staff[count].staffic, staff[count].salary);
            printf("-------------------------------------------------\n");
        }
    }


}