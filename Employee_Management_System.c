#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "employees.dat"

struct Employee {
    int id;
    char name[50];
    char role[30];
    float salary;
    int is_active;
};

struct Employee *emp = NULL;
int count = 0;

int search(int id) {
    for (int i = 0; i < count; i++) {
        if (emp[i].id == id)
            return i;
    }
    return -1;
}

/* ---------- Load saved data at program startup ---------- */
void loadFromFile() {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (fp == NULL) {
        return; // no saved file yet — start fresh
    }

    struct Employee temp;
    while (fread(&temp, sizeof(struct Employee), 1, fp) == 1) {
        struct Employee *newBlock = (struct Employee*) realloc(emp, (count + 1) * sizeof(struct Employee));
        emp = newBlock;
        emp[count] = temp;
        count++;
    }

    fclose(fp);

    if (count > 0)
        printf("Loaded %d employee record(s) from previous session.\n", count);
}

/* ---------- Save all data before program exits ---------- */
void saveToFile() {
    FILE *fp = fopen(FILE_NAME, "wb");
    if (fp == NULL) {
        printf("Error: could not save data to file.\n");
        return;
    }

    fwrite(emp, sizeof(struct Employee), count, fp);
    fclose(fp);
}

void buildTable() {
    int n;

    if (emp != NULL) {
        printf("\nEmployee table is already built.\n");
        return;
    }

    printf("\nHow many employees do you want to add : ");
    scanf("%d", &n);
    getchar();

    if (n <= 0) {
        printf("Invalid number of employees.\n");
        return;
    }

    emp = (struct Employee*) malloc(n * sizeof(struct Employee));

    for (int i = 0; i < n; i++) {
        printf("\n---------- Employee %d ----------\n", i + 1);

        printf("ID: ");
        scanf("%d", &emp[count].id);
        getchar();

        if (search(emp[count].id) != -1) {
            printf("Employee ID already exists.\n");
            i--;
            continue;
        }

        printf("Name: ");
        fgets(emp[count].name, sizeof(emp[count].name), stdin);
        emp[count].name[strcspn(emp[count].name, "\n")] = '\0';

        printf("Role: ");
        fgets(emp[count].role, sizeof(emp[count].role), stdin);
        emp[count].role[strcspn(emp[count].role, "\n")] = '\0';

        printf("Salary: ");
        scanf("%f", &emp[count].salary);
        getchar();

        emp[count].is_active = 1;
        count++;
    }

    printf("\nEmployee table created successfully.\n");
    printf("Total employees: %d\n", count);
}

void insertEntry() {
    struct Employee *temp;
    temp = (struct Employee*) realloc(emp, (count + 1) * sizeof(struct Employee));
    emp = temp;

    printf("\n---------- New Employee ----------\n");

    printf("ID: ");
    scanf("%d", &emp[count].id);
    getchar();

    if (search(emp[count].id) != -1) {
        printf("Employee ID already exists.\n");
        return;
    }

    printf("Name: ");
    fgets(emp[count].name, sizeof(emp[count].name), stdin);
    emp[count].name[strcspn(emp[count].name, "\n")] = '\0';

    printf("Role: ");
    fgets(emp[count].role, sizeof(emp[count].role), stdin);
    emp[count].role[strcspn(emp[count].role, "\n")] = '\0';

    printf("Salary: ");
    scanf("%f", &emp[count].salary);
    getchar();

    emp[count].is_active = 1;
    count++;

    printf("\nEmployee inserted successfully.\n");
    printf("Total employees: %d\n", count);
}

void deleteEntry() {
    int id;

    if (emp == NULL || count == 0) {
        printf("\nNo employees available.\n");
        return;
    }

    printf("\nEnter ID to delete: ");
    scanf("%d", &id);
    getchar();

    int idx = search(id);
    if (idx == -1) {
        printf("Employee not found.\n");
        return;
    }

    for (int i = idx; i < count - 1; i++) {
        emp[i] = emp[i + 1];
    }

    count--;

    if (count == 0) {
        free(emp);
        emp = NULL;
    } else {
        struct Employee *temp = realloc(emp, count * sizeof(struct Employee));
        if (temp != NULL) {
            emp = temp;
        }
    }

    printf("Employee deleted successfully.\n");
}

void searchRecord() {
    int id;

    if (emp == NULL || count == 0) {
        printf("\nNo employees available.\n");
        return;
    }

    printf("\nEnter Employee ID to search: ");
    scanf("%d", &id);
    getchar();

    int idx = search(id);
    if (idx == -1) {
        printf("Employee not found.\n");
        return;
    }

    printf("\n========== Employee Details ==========\n");
    printf("ID     : %d\n", emp[idx].id);
    printf("Name   : %s\n", emp[idx].name);
    printf("Role   : %s\n", emp[idx].role);
    printf("Salary : %.2f\n", emp[idx].salary);
    printf("Status : %s\n", emp[idx].is_active ? "Currently Working" : "Resigned");
}

void markResigned() {
    int id;

    if (emp == NULL || count == 0) {
        printf("\nNo employees available.\n");
        return;
    }

    printf("\nEnter Employee ID: ");
    scanf("%d", &id);
    getchar();

    int idx = search(id);
    if (idx == -1) {
        printf("Employee not found.\n");
        return;
    }

    if (emp[idx].is_active == 0) {
        printf("Employee already resigned.\n");
        return;
    }

    emp[idx].is_active = 0;
    printf("Employee marked as resigned.\n");
}

void displayEmployees() {
    if (emp == NULL || count == 0) {
        printf("\nNo employees available.\n");
        return;
    }

    printf("\n ALL EMPLOYEES \n");

    for (int i = 0; i < count; i++) {
        printf("\nEmployee %d\n", i + 1);
        printf("ID     : %d\n", emp[i].id);
        printf("Name   : %s\n", emp[i].name);
        printf("Role   : %s\n", emp[i].role);
        printf("Salary : %.2f\n", emp[i].salary);
        printf("Status : %s\n", emp[i].is_active ? "Currently Working" : "Resigned");
    }
}

int main() {
    int choice;

    loadFromFile(); // restore previous session's data, if any

    do {
        printf("\nEMPLOYEE MANAGEMENT SYSTEM\n");
        printf("1. Build Employee Table\n");
        printf("2. Insert Employee\n");
        printf("3. Delete Employee\n");
        printf("4. Search Employee\n");
        printf("5. Mark Employee as Resigned\n");
        printf("6. Display Employees\n");
        printf("0. Exit\n");

        printf("\nEnter choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: buildTable(); break;
            case 2: insertEntry(); break;
            case 3: deleteEntry(); break;
            case 4: searchRecord(); break;
            case 5: markResigned(); break;
            case 6: displayEmployees(); break;
            case 0:
                saveToFile(); // persist data before exiting
                printf("\nData saved. Exiting program...\n");
                printf("Exited.\n");
                break;
            default:
                printf("\nInvalid choice...!\n");
        }

    } while (choice != 0);

    free(emp);
    return 0;
}