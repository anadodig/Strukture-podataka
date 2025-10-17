#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct person;
typedef struct person* position;
typedef struct person {
    char name[30];
    char surname[30];
    int birthYear;
    position next;
} Person;

Person* addAtBeg(Person* head, char*name, char*surname, int birthYear);
void printList(Person* head);
Person* addAtEnd(Person* head, char* name, char* surname, int birthYear);
Person* searchBySurname(Person* head, char* surname);
Person* deleteElement(Person* head, char* surname);
void freeList(Person* head);

int main() {
    Person* head = NULL;
    char name[30], surname[30];
    int izbor, birthYear;

    while (1) {
        printf("1. Dodaj osobu na pocetak: \n");
        printf("2. Ispisi listu: \n");
        printf("3. Izlaz\n");
        printf("4. Dodaj osobu na kraj: \n");
        printf("5. Pronadji po prezimenu: \n");
        printf("6. Izbrisi element: \n");
        printf("izbor: ");
        scanf("%d", &izbor);
        getchar();

        switch (izbor) {
        case 1:
            printf("Unesite ime: ");
            fgets(name, 50, stdin);  //unos za string
            name[strcspn(name, "\n")] = 0; //otklanjanje new linea

            printf("Unesite prezime: ");
            fgets(surname, 50, stdin);
            surname[strcspn(surname, "\n")] = 0;

            printf("Unesite godinu rodjenja: ");
            scanf("%d", &birthYear);  //unos za int
            getchar(); //ciscenje buffera

            head = addAtBeg(head, name, surname, birthYear); //poziv funkcije
            printf("\nUspjesan unos!\n");
            break;

        case 2:
            printList(head);
            break;

        case 3:
            freeList(head);
            return 0;

        case 4:
            printf("Unesite ime: ");
            fgets(name, 50, stdin);
            name[strcspn(name, "\n")] = 0;

            printf("Unesite prezime: ");
            fgets(surname, 50, stdin);
            surname[strcspn(surname, "\n")] = 0;

            printf("Unesite godinu rodjenja: ");
            scanf("%d", &birthYear);
            getchar();

            head = addAtEnd(head, name, surname, birthYear);
            printf("Uspjesan unos!\n");
            break;

        case 5:
            printf("Unesi zeljeno prezime: ");
            fgets(surname, 50, stdin);
            surname[strcspn(surname, "\n")] = 0;
            //poziv
            Person* find = searchBySurname(head, surname);
            //provjera
            if (find != NULL) {
                printf("OSOBA PRONADJENA:\n");
                printf("Ime: %s\n", find->name);
                printf("Surname: %s\n", find->surname);
                printf("Godina rodjenja: %d\n", find->birthYear);
            }
            else {
                printf("Osoba sa prezimenom %s nije pronadjena!\n", surname);
            }
            break;

        case 6:
            printf("Unesi osobu za brisanje: ");
            fgets(surname, 50, stdin);
            surname[strcspn(surname, "\n")] = 0;

            head = deleteElement(head, surname);
            break;

        default:
            printf("greska!");
        }
    }
    return 0;
}

Person* addAtBeg(Person* head, char*name, char*surname, int year) {
    //dinamicka alokacija memorije za novu osobu
    Person* newPerson = (Person*)malloc(sizeof(Person));
    //provjera uspjesnosti
    if (newPerson == NULL) {
        printf("alokacija nije uspjela");
        return NULL;
    }
    //popunjavanje podataka
    strcpy(newPerson->name, name);
    strcpy(newPerson->surname, surname);
    newPerson->birthYear = year;

    //novi element pokazuje na trenutni head
    newPerson->next = head; 
    //update heada
    head = newPerson;       
    //vracanje updateanog heada
    return head;
}

Person* addAtEnd(Person* head, char* name, char* surname, int year) {
    Person* newPerson = (Person*)malloc(sizeof(Person));
    //provjera
    if (newPerson == NULL) {
        printf("Alokacija nije uspjela");
        return NULL;
    }
    //popunjavanje
    strcpy(newPerson->name, name);
    strcpy(newPerson->surname, surname);
    newPerson->birthYear = year;
    newPerson->next = NULL;

    //ako je lista prazna, nova osoba postaje head
    if (head == NULL) {
        return newPerson;
    }

    //pronalazak zadnjeg elementa
    Person* temp = head;
    while (temp->next != NULL) {
        temp = temp->next;
    }

    //dodaj novi na kraj
    temp->next = newPerson;

    return head;
}

Person* searchBySurname(Person* head, char* surname) {
    Person* temp = head;

    while (temp != NULL) {
        if (strcmp(temp->surname, surname) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL; //nije pronadjeno
}

Person* deleteElement(Person* head, char* surname) {
    //ako je lista prazna nemamo sta izbrisati
    if (head == NULL) {
        printf("Lista je prazna\n");
        return NULL;
    }

    Person* temp = head; //pokazivac koji se giba kroz listu
    Person* last = NULL; //cuva prethodne elemente

    //ako je element za brisanje prvi(tada nemamo prethodni element), brisemo head
    if (strcmp(head->surname, surname) == 0) {
        head = head->next; //novi head je slj tj drugi element
        free(temp); //oslobadjamo stari head
        printf("Osoba sa prezimenom %s je obrisana\n", surname);
        return head;
    }
    //trazenje u ostatku liste
    while (temp != NULL && strcmp(temp->surname, surname) != 0) {
        last = temp;  //pamti trenutni kao prethodni
        temp = temp->next; //pomjeranje
    }

    if (temp == NULL) {
        printf("Osoba sa prezimenom '%s' nije pronadjena.\n", surname);
        return head;
    }
    //brisanje(preskakanje trenutnog):
    last->next = temp->next; //povezuje prethodni el sa onim koji dolazi nakon trenutnog
    free(temp);
    printf("Osoba sa prezimenom '%s' je obrisana.\n", surname);

    return head;
}

void printList(Person* head) {
    if (head == NULL) {
        printf("lista je prazna\n");
        return;
    }
    Person* temp = head;
    while (temp != NULL) {
        printf("%s %s %d\n", temp->name, temp->surname, temp->birthYear);
        temp = temp->next;
    }
}

void freeList(Person* head) {
    Person* temp = head;
    Person* next;

    while (temp!= NULL) {
        next = temp->next;
        free(temp);
        temp = next;
    }
}




