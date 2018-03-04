//подсчет числа элементов
//найти макс/мин
//удалять повторяющиеся
//удалять список
//удалять элемент
//добавлять элементы в список по алфавиту

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *FirstName;
    char *MiddleName;
    char *LastName;
    char *HomePostalAddr;
    int Group;
    float Rating;
    int ID;
} Student;

typedef struct node {
    Student *data;
    struct node *next;
} Node;

typedef struct {
    Node *head;
    Node *tail;
    unsigned int length;
} Students;

void printMenu(void);
int menu(void);

// float avgRating(Students *s);

void LoadStudents(Students *s);
void SortStudents(Students *s);
void FindMaxStudents(Students *s);
void FindMinStudents(Students *s);
void DeduplicateStudents(Students *s);
void RemoveStudents(Students *s);
void RemoveStudent(Students *s);
void PrintStudents(Students *s);
void setStudentByIndex(Students *s, int index, Student *newStudent);
Student *getStudentByIndex(Students *s, int index);

void swap(Students *s, int left, int right) {
    Student *temp = getStudentByIndex(s, left);
    setStudentByIndex(s, left, getStudentByIndex(s, right));
    setStudentByIndex(s, right, temp);
}

void setStudentByIndex(Students *s, int index, Student *newStudent) {
    Node *curr = s->head;
    for (int i = 0; i <= index; i++) {
        if (curr == NULL) {
            return;
        }
        if (i == index) {
            curr->data = newStudent;
            return;
        }
        curr = curr->next;
    }
}

int main(void) {
    printf("Students management by Anna Gudinskaya🙃\n");
    return menu();
}

void printerr(char *err) { printf("Error: %s. Please try again.\n", err); }

void ignoreGarbage(void) {
    int buffer;
    while ((buffer = getchar()) != EOF && buffer != '\n')
        ;
}

int readInt(void) {
    bool success = false;
    int res = 0;

    while (!success) {
        success = scanf(" %d", &res);

        if (!success) {
            printerr("Invalid input");
            printf("Enter value: ");
        }
        ignoreGarbage();
    }

    return res;
}

float readFloat(void) {
    bool success = false;
    float res = 0;

    while (!success) {
        success = scanf(" %f", &res);

        if (!success) {
            printerr("Invalid input");
            printf("Enter value: ");
        }
        ignoreGarbage();
    }

    return res;
}

char *readString(char *whatWeWant) {
    char *str;
    size_t len = 0;  // for 'getline', that is 'getс' in c11 standard
    bool firstRun = true;
    do {
        if (!firstRun) {
            printerr("Invalid input. Value must be more than 1 character long");
            printf("Enter value: ");
        }
        if (firstRun && strlen(whatWeWant) > 0) {
            printf("Enter %s: ", whatWeWant);
        }
        firstRun = false;
        getline(&str, &len, stdin);
    } while (strlen(str) <= 1);

    return str;
}

int menu(void) {
    Students *students =
        (Students *)malloc(sizeof(Students));  // изначальное выделение памяти

    do {
        printMenu();
        printf("Enter your choise: ");
        switch (readInt()) {
            case 1:
                LoadStudents(students);
                break;

            case 2:
                printf("> Students count is %d\n", students->length);
                break;

            case 3:
                FindMaxStudents(students);
                break;

            case 4:
                FindMinStudents(students);
                break;

            case 5:
                DeduplicateStudents(students);
                break;

            case 6:
                RemoveStudents(students);
                break;

            case 7:
                RemoveStudent(students);
                break;

            case 8:
                PrintStudents(students);
                break;

            case 9:
                return 0;
        }
    } while (1);
}

void printMenu(void) {
    printf("\t1. Input students\n");  // loads user input
    printf("\t2. Count students\n");
    printf("\t3. Search students with max rating\n");
    printf("\t4. Search students with min rating\n");
    printf("\t5. Remove the same students\n");
    printf("\t6. Remove all students\n");
    printf("\t7. Remove a student\n");
    printf("\t8. Print students\n");
    printf("\t9. Exit\n");
}

Student *readStudentInfo(void) {
    // gets was removed in C11.
    // we should use getline, which uses
    // char** instead of char*.
    Student *s = malloc(sizeof(Student));
    printf("Student's ID: ");
    s->ID = readInt();
    s->FirstName = readString("Student's First Name");
    s->MiddleName = readString("Student's Middle Name");
    s->LastName = readString("Student's Last Name");
    s->HomePostalAddr =
        readString("Student's Home Postal Address (one-line format)");

    printf("Enter Student's Group: ");
    s->Group = readInt();

    printf("Enter Student's Rating: ");
    s->Rating = readFloat();

    return s;
}

bool needsExit() {
    int choise = 0;
    do {
        if (choise != 0) {
            printf("Error: Invalid input. Please, try again.\n");
        }
        printf("\t1. Continue\n");
        printf("\t2. Exit\n");
        printf("Enter choise: ");
        choise = readInt();
    } while (choise != 1 && choise != 2);

    return choise == 2;
}

void LoadStudents(Students *s) {
    if (s->head == NULL) {
        s->head = s->tail = malloc(sizeof(Node));
        s->head->data = readStudentInfo();
        s->length = 1;

        if (needsExit()) {
            return;
        }
    }

    for (;;) {
        s->tail->next = malloc(sizeof(Node));
        s->tail->next->data = readStudentInfo();
        s->tail = s->tail->next;
        s->length++;

        if (needsExit()) {
            return;
        }
    }
}

void printStudent(Student *s) {
    // getline() leaves us a new line, so omit it here
    printf(">\tID:                  %d\n", s->ID);
    printf(">\tFirst Name:          %s", s->FirstName);
    printf(">\tMiddle Name:         %s", s->MiddleName);
    printf(">\tLast Name:           %s", s->LastName);
    printf(">\tHome Postal Address: %s", s->HomePostalAddr);
    printf(">\tGroup:               %d\n", s->Group);
    printf(">\tRating:              %.4g\n", s->Rating);
    printf("--------------------------\n\n");
}
void PrintStudents(Students *s) {
    if (!s->length) {
        printf("Got no students, give me some data before printing it 🙃\n");
        LoadStudents(s);
    }
    Node *curr = s->head;
    for (;;) {
        if (curr == NULL) {
            return;
        }
        printStudent(curr->data);
        curr = curr->next;
    }
}
void RemoveStudents(Students *s) {
    Node *curr = s->head;
    Node *next;
    for (;;) {
        if (curr == NULL) {
            break;
        }
        next = curr->next;
        free(curr->data);
        free(curr);
        curr = next;
    }
    s->length = 0;
    s->head = s->tail = NULL;
}

void RemoveStudentByID(Students *s, int ID) {
    Node *curr = s->head;
    Node *prev = NULL;
    for (;;) {
        if (curr == NULL) {
            return;
        }
        if (curr->data->ID == ID) {
            if (prev == NULL && curr->next == NULL) {
                RemoveStudents(s);
                return;
            }
            free(curr->data);
            if (prev == NULL) {
                s->head = curr->next;
            } else {
                prev->next = curr->next;
            }
            if (curr->next == NULL) {
                s->tail = prev;
            }
            s->length--;
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}
void RemoveStudent(Students *s) {
    printf("Enter student's ID: ");
    int ID = readInt();
    RemoveStudentByID(s, ID);
}
Student *getStudentByIndex(Students *s, int index) {
    Node *curr = s->head;
    for (int i = 0; i <= index; i++) {
        if (curr == NULL) {
            return NULL;
        }
        if (i == index) {
            return curr->data;
        }
        curr = curr->next;
    }
    return NULL;
}

Students *copyStudents(Students *s) {
    Students *newS = malloc(sizeof(Students));
    newS->head = malloc(sizeof(Node));
    Node *newCurr = newS->head;
    Node *oldCurr = s->head;
    for (int i = 0; i < s->length; i++) {
        memcpy(
            newCurr, oldCurr,
            sizeof(Node));  // копирует инфу newCurr из oldCurr в размере Node
        newCurr->next = malloc(sizeof(Node));
        newCurr = newCurr->next;
        oldCurr = oldCurr->next;
    }

    newS->length = s->length;
    return newS;
}
int partition(Students *s, int left, int right, int pivotIndex,
              bool (*shouldSwap)(Student *, Student *)) {
    Student *pivotValue = getStudentByIndex(s, pivotIndex);
    int storeIndex = left;

    if (pivotValue == NULL) {
        printf("pivotValue is NULL on %d\n", pivotIndex);
    }

    swap(s, pivotIndex, right);
    for (int i = left; i < right; i++) {
        if (getStudentByIndex(s, i) == NULL) {
            printf("s->list[i] is NULL on %d\n", i);
        }
        if (shouldSwap(getStudentByIndex(s, i), pivotValue)) {
            swap(s, i, storeIndex);
            storeIndex++;
        }
    }
    swap(s, storeIndex, right);
    return storeIndex;
}

void quicksort(Students *s, int left, int right,
               bool (*shouldSwap)(Student *, Student *)) {
    int pivotIndex = left;
    int pivotNewIndex;

    while (right > left) {
        pivotNewIndex = partition(s, left, right, pivotIndex, shouldSwap);
        quicksort(s, left, pivotNewIndex - 1, shouldSwap);
        pivotIndex = left = pivotNewIndex + 1;
    }
}

bool firstNameComparator(Student *a, Student *b) {
    return strcmp(a->FirstName, b->FirstName) <= 0;
}

bool middleNameComparator(Student *a, Student *b) {
    return strcmp(a->MiddleName, b->MiddleName) <= 0;
}

bool lastNameComparator(Student *a, Student *b) {
    return strcmp(a->LastName, b->LastName) <= 0;
}
bool idComparator(Student *a, Student *b) { return a->ID <= b->ID; }

void SortStudents(Students *s) {
    if (s->length < 2) {
        printerr("At least 2 records required to use sort");
        return;
    }
    printf("Sort by:\n");
    printf("\t1. ID\n");
    printf("\t2. First Name\n");
    printf("\t3. Middle Name\n");
    printf("\t4. Last Name\n");
    // shouldSwap function must accept two students, compare them
    // and return bool if we should swap values.
    // the signature is in the following pattern:
    //     functionReturnType(*pointerName)(functionArgumentList)
    printf("Enter your choise: ");
    int choise = readInt();
    Students *sorted = copyStudents(s);

    switch (choise) {
        case 1:  // id, asc
            quicksort(sorted, 0, s->length - 1, &idComparator);
            break;
        case 2:  // FirstName, asc
            quicksort(sorted, 0, s->length - 1, &firstNameComparator);
            break;
        case 3:  // MiddleName, asc
            quicksort(sorted, 0, s->length - 1, &middleNameComparator);
            break;
        case 4:  // LastName, asc
            quicksort(sorted, 0, s->length - 1, &lastNameComparator);
            break;
    }

    Node *curr = s->head;
    Node *next;
    for (;;) {
        if (curr == s->tail) {
            free(s->tail);
            break;
        }
        next = curr->next;
        free(curr);
        curr = next;
    }
    s->head = sorted->head;
    s->tail = sorted->tail;
}
void FindMaxStudents(Students *s) {
    Student *max = s->head->data;
    Node *curr = s->head;
    while (curr != NULL) {
        // If max is less then head->data then
        // assign value of head->data to max
        // otherwise node point to next node.
        if (max->Rating < curr->data->Rating) {
            max = curr->data;
        }
        curr = curr->next;
    }
    printStudent(max);
}
void FindMinStudents(Students *s) {
    Student *min = s->head->data;
    Node *curr = s->head;
    while (curr != NULL) {
        // If max is less then head->data then
        // assign value of head->data to max
        // otherwise node point to next node.
        if (min->Rating > curr->data->Rating) {
            min = curr->data;
        }
        Node *curr;
        curr = curr->next;
    }
    printStudent(min);
}

void RemoveStudentByPointer(Students *s, Node* node) {
    Node *curr = s->head;
    Node *prev = NULL;
    for (;;) {
        if (curr == NULL) {
            return;
        }
        if (curr == node) {
            if (prev == NULL && curr->next == NULL) {
                RemoveStudents(s);
                return;
            }
            free(curr->data);
            if (prev == NULL) {
                s->head = curr->next;
            } else {
                prev->next = curr->next;
            }
            if (curr->next == NULL) {
                s->tail = prev;
            }
            s->length--;
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void DeduplicateStudents(Students *s) {
    Node *curr = s->head;
    for (;;) {
        if (curr->next == NULL) {
            return;
        }
        Node *localCurr = s->head;
        for (;;) {
            if (localCurr->next == NULL) {
                break;
            }
            if (curr != localCurr) {
                if (strcmp(curr->data->FirstName, localCurr->data->FirstName) == 0 &&
                    strcmp(curr->data->MiddleName, localCurr->data->MiddleName) == 0 &&
                    strcmp(curr->data->LastName, localCurr->data->LastName) == 0 &&
                    strcmp(curr->data->HomePostalAddr, localCurr->data->HomePostalAddr) == 0 &&
                    curr->data->Group == localCurr->data->Group &&
                    curr->data->Rating == localCurr->data->Rating &&
                    curr->data->ID == localCurr->data->ID) {
                    RemoveStudentByPointer(s, localCurr);
                }
            }
            localCurr = localCurr->next;
        }
        curr = curr->next;
    }
}
