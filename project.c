#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

typedef struct User {
    char name[50];
    float balance;
    struct User* next;
} User;

typedef struct Transaction {
    User* from;
    User* to;
    float amount;
} Transaction;

User* createUser(char* name) {
    User* newUser = (User*)malloc(sizeof(User));
    strcpy(newUser->name, name);
    newUser->balance = 0;
    newUser->next = NULL;
    return newUser;
}

void addUser(User** head, char* name) {
    User* newUser = createUser(name);
    newUser->next = *head;
    *head = newUser;
}

User* findUser(User* head, char* name) {
    User* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

void addExpense(User* head, char* payer, int num, char** names, float amount) {
    User* user = findUser(head, payer);
    if (user != NULL) {
        user->balance += amount;
        float splitAmount = amount / num;
        for (int i = 0; i < num; i++) {
            user = findUser(head, names[i]);
            if (user != NULL) {
                user->balance -= splitAmount;
            }
        }
    }
}

void showBalance(User* head) {
    User* temp = head;
    while (temp != NULL) {
        printf("%s: %.2f\n", temp->name, temp->balance);
        temp = temp->next;
    }
}

void addUserSorted(User** head, char* name) {
    User* newUser = createUser(name);
    User* current = *head;
    User* prev = NULL;

    while (current != NULL && strcmp(current->name, name) < 0) {
        prev = current;
        current = current->next;
    }

    if (prev == NULL) {
        newUser->next = *head;
        *head = newUser;
    } else {
        prev->next = newUser;
        newUser->next = current;
    }
}

User* findHighestLender(User* head) {
    User* highestLender = NULL;
    User* current = head;

    while (current != NULL) {
        if (current->balance > 0) {
            if (highestLender == NULL || current->balance > highestLender->balance) {
                highestLender = current;
            }
        }
        current = current->next;
    }

    return highestLender;
}

User* findHighestBorrower(User* head) {
    User* highestBorrower = NULL;
    User* current = head;

    while (current != NULL) {
        if (current->balance < 0) {
            if (highestBorrower == NULL || current->balance < highestBorrower->balance) {
                highestBorrower = current;
            }
        }
        current = current->next;
    }

    return highestBorrower;
}

User* removeNode(User* head, User* keyNode) 
{
    User* current = head;
    User* prev = NULL;

    while (current != NULL && current != keyNode) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) 
    {
        return head;
    }

    if (prev == NULL) 
    {
      head = current->next;
    } 
    else
    {
        prev->next = current->next;
    }
    free(current);

    return head;
}

User* duplicateGraph(User* head) {
    User* duplicatedHead = NULL;
    User* current = head;

    while (current != NULL) {
        User* newUser = createUser(current->name);
        newUser->balance = current->balance;
        newUser->next = duplicatedHead;
        duplicatedHead = newUser;
        current = current->next;
    }

    return duplicatedHead;
}

void minimizeTransactions(User* head) 
{
    User* highestLender = findHighestLender(head);
    User* highestBorrower = findHighestBorrower(head);

    while (highestLender != NULL && highestBorrower != NULL) 
    {
        float transferAmount;

        if (highestLender->balance > -highestBorrower->balance) {
            transferAmount = -(highestBorrower->balance);
            if(transferAmount != 0)
                printf("%s has to pay %.2f to %s\n", highestBorrower->name, transferAmount, highestLender->name);
            highestBorrower->balance = 0;
            highestLender->balance -= transferAmount;
            highestBorrower = findHighestBorrower(head);
        } else {
            transferAmount = highestLender->balance;
            if(transferAmount != 0)
                printf("%s has to pay %.2f to %s\n", highestBorrower->name, transferAmount, highestLender->name);
            highestLender->balance = 0;
            highestBorrower->balance += transferAmount;
            highestLender = findHighestLender(head);
        }
    }
}

void uploadBalanceSheet(User* head) 
{
    FILE* file = fopen("balancesheet.csv", "a");

    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    fprintf(file, "Name, Balance\n");

    User* current = head;
    while (current != NULL) 
    {
        fprintf(file, "%s,%.2f\n", current->name, current->balance);
        current = current->next;
    }

    fclose(file); // Close the CSV file
    printf("Balance sheet uploaded successfully to 'balancesheet.csv'\n");
}

int main() {
    User* head = NULL;
    int head_size = 0;
    while(head != NULL)
    {
        head_size++;
        head = head->next;
    }

    int choice;
    char name[50];
    int num;
    float amount;

    do {
        printf("\nUser Expense Management System\n");
        printf("1. Add User\n");
        printf("2. Add Expense\n");
        printf("3. Show Balances\n");
        printf("4. Minimize Transactions\n");
        printf("5. Upload Balance Sheet\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter the user's name: ");
                scanf("%s", name);
                addUser(&head, name);
                break;
            case 2:
                printf("Enter the payer's name: ");
                scanf("%s", name);

            
                    printf("Enter the number of people sharing the expense: ");
                    scanf("%d", &num);

            

                char** names = (char**)malloc(num * sizeof(char*));
                for (int i = 0; i < num; i++) {
                    names[i] = (char*)malloc(50);
                    printf("Enter name %d: ", i + 1);
                    scanf("%s", names[i]);
                }
                printf("Enter the expense amount: ");
                scanf("%f", &amount);
                addExpense(head, name, num, names, amount);
                for (int i = 0; i < num; i++) {
                    free(names[i]);
                }
                free(names);
                break;
            case 3:
                showBalance(head);
                break;
            case 4:
                minimizeTransactions(head);
                break;
            case 5: 
                uploadBalanceSheet(head);
            case 6:
                // Exit the program
                break;
            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (choice != 6);

    return 0;
}