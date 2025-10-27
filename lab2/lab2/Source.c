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
int addBehind(Person* P, char* name, char* surname, int birthYear);
Person* addBefore(Person* head, char*target, char* name, char* surname, int birthYear);
void writeToFile(Person* head, char* filename);
Person* readFromFile(Person* head, char* filename);
Person* sortList(Person* head);

int main() {
    Person* head = NULL;
    char name[30], surname[30], existingSurname[30];
    int izbor, birthYear;
    char filename[50]; 

    while (1) {
        printf("---------------------------------\n");
        printf("1. Dodaj osobu na pocetak: \n");
        printf("2. Ispisi listu: \n");
        printf("3. Izlaz\n");
        printf("4. Dodaj osobu na kraj: \n");
        printf("5. Pronadji po prezimenu: \n");
        printf("6. Izbrisi element: \n");
        printf("7. Dodaj iza elementa: \n");
        printf("8. Dodaj ispred elementa: \n");
        printf("9. Sortiraj listu po prezimenima: \n");
        printf("10. Upisi listu u datoteku: \n");
        printf("11. Ucitaj listu iz datoteke: \n");
        printf("izbor: ");
        scanf("%d", &izbor);
        getchar();

        switch (izbor) {
        case 1:
            printf("Unesite ime: ");
            fgets(name, 30, stdin);  //unos za string
            name[strcspn(name, "\n")] = 0; //otklanjanje new linea

            printf("Unesite prezime: ");
            fgets(surname, 30, stdin);
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
            fgets(name, 30, stdin);
            name[strcspn(name, "\n")] = 0;

            printf("Unesite prezime: ");
            fgets(surname, 30, stdin);
            surname[strcspn(surname, "\n")] = 0;

            printf("Unesite godinu rodjenja: ");
            scanf("%d", &birthYear);
            getchar();

            head = addAtEnd(head, name, surname, birthYear);
            printf("Uspjesan unos!\n");
            break;

        case 5:
            printf("Unesi zeljeno prezime: ");
            fgets(surname, 30, stdin);
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
            printf("Unesi prezime za brisanje: ");
            fgets(surname, 30, stdin);
            surname[strcspn(surname, "\n")] = 0;

            head = deleteElement(head, surname);
            break;

        case 7:
            printf("Unesi prezime osobe iza koje zelis unijeti novo: ");
            fgets(existingSurname, 30, stdin);
            existingSurname[strcspn(existingSurname, "\n")] = 0;

            Person* target = searchBySurname(head, existingSurname);
            if (target == NULL) {
                printf("Osoba nije pronadjena.\n");
                break;
            }

            printf("Unesi ime nove osobe: ");
            fgets(name, 30, stdin);
            name[strcspn(name, "\n")] = 0;

            printf("Unesi prezime nove osobe: ");
            fgets(surname, 30, stdin);
            surname[strcspn(surname, "\n")] = 0;

            printf("Unesi godinu nove osobe: ");
            scanf("%d", &birthYear);
            getchar();

            addBehind(target, name, surname, birthYear);
            printf("Uspjesan unos!\n");
            break;

        case 8:
            //char existingSurname2[30];
            printf("Unesi prezime osobe ispred koje zelis unijeti novo: ");
            fgets(existingSurname, 30, stdin);
            existingSurname[strcspn(existingSurname, "\n")] = 0;

            printf("Unesi ime nove osobe: ");
            fgets(name, 30, stdin);
            name[strcspn(name, "\n")] = 0;

            printf("Unesi prezime nove osobe: ");
            fgets(surname, 30, stdin);
            surname[strcspn(surname, "\n")] = 0;

            printf("Unesi godinu nove osobe: ");
            scanf("%d", &birthYear);
            getchar();

            head=addBefore(head, existingSurname, name, surname, birthYear);
            printf("Uspjesan unos!\n");
            break;

        case 9:
            head = sortList(head);
            printf("Lista sortirana po prezimenima.\n");
            break;

        case 10:
            printf("Unesi ime datoteke: ");
            fgets(filename, 50, stdin);
            filename[strcspn(filename, "\n")] = 0;

            writeToFile(head, filename);
            break;

        case 11:
            printf("Unesi ime datoteke: ");
            fgets(filename, 50, stdin);
            filename[strcspn(filename, "\n")] = 0;

            head = readFromFile(head, filename);
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
        return head;
    }
    //popunjavanje podataka
    strcpy(newPerson->name, name);
    strcpy(newPerson->surname, surname);
    newPerson->birthYear = year;

    //u next upisujemo adresu od head
    newPerson->next = head; 
    //update heada, pocetak liste je novi cvor, sada head pokazuje na novi
    head = newPerson;       
    //vracanje updateanog heada
    return head;
}

Person* addAtEnd(Person* head, char* name, char* surname, int year) {
    //alokacija novog cvora
    Person* newPerson = (Person*)malloc(sizeof(Person));
    //provjera
    if (newPerson == NULL) {
        printf("Alokacija nije uspjela");
        return head; 
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

int addBehind(Person* p, char* name, char* surname, int birthYear) {
    if (p == NULL) {
        printf("Cvor ne postoji");
        return -1;
    }                   
    Person* newPerson=(Person*)malloc(sizeof(Person));
    if (newPerson == NULL) {
        printf("Alokacija nije uspjela");
        return -1;
    }

    strcpy(newPerson->name, name);
    strcpy(newPerson->surname, surname);
    newPerson->birthYear = birthYear;

    newPerson->next = p->next;   //nova osoba pokazuje na sta je pokazivala p
    p->next = newPerson;        //p sada pokazuje na novu osobu

    return 0;
}

Person* addBefore(Person* head, char*target, char* name, char* surname, int birthYear) {
    if (head == NULL) {
        printf("Lista je prazna\n");
        return head;
    }

    if (strcmp(head->surname, target) == 0) {
        return addAtBeg(head, name, surname, birthYear);
    }

    //trazimo el PRIJE "existing"
    Person* temp = head;
    while (temp->next != NULL && strcmp(temp->next->surname, target) != 0) {
        temp = temp->next;
    }

    //ako nismo nasli:
    if (temp->next == NULL) {
        printf("Osoba sa prezimenom '%s' nije pronadjena\n", target);
        return head;
    }

    //stvaramo novu osobu
    Person* newPerson = (Person*)malloc(sizeof(Person));
    if (newPerson == NULL) {
        printf("alokacija nije uspjela");
        return head;
    }

    strcpy(newPerson->name, name);
    strcpy(newPerson->surname, surname);
    newPerson->birthYear = birthYear;

    newPerson->next = temp->next;
    temp->next = newPerson;
    printf("Uspjesan unos!\n");

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
    Person* temp = head;        //novi pokazivac temp i postavljamo na isti cvor kao head
    Person* next;              //deklariramo novi pokazivac

    while (temp!= NULL) {
        next = temp->next;  //spremamo adresu slj cvora
        free(temp);        //brisemo trenutni
        temp = next;      //nastavljamo na slj cvor
    }
}

void writeToFile(Person* head, char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Neuspjelo otvaranje");
        return;
    }

    Person* temp = head;
    while (temp != NULL) {
        fprintf(file, "%s %s %d\n", temp->name, temp->surname, temp->birthYear);
        temp = temp->next;
    }

    fclose(file);
    printf("Upisano u datoteku %s!\n", filename);
}

Person* readFromFile(Person* head, char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Neuspjelo otvaranje");
        return head;
    }

    //oslobadjamo postojecu listu
    freeList(head);
    head = NULL;

    char name[30], surname[30];
    int birthYear;

    while (fscanf(file, "%s %s %d", name, surname, &birthYear) == 3) {
        head = addAtEnd(head, name, surname, birthYear);
    }

    fclose(file);
    printf("Lista uspjesno ucitana iz %s!\n", filename);
    return head;
}

Person* sortList(Person* head) {
    if (head == NULL || head->next == NULL) {
        return head;
    }

    int swapped;
    Person* temp;
    Person* last = NULL;

    do {
        swapped = 0;
        temp = head;

        while (temp->next != last) {
            if (strcmp(temp->surname, temp->next->surname) > 0) {
                char tempName[30], tempSurname[30];
                int tempYear;

                strcpy(tempName, temp->name);
                strcpy(tempSurname, temp->surname);
                tempYear = temp->birthYear;

                strcpy(temp->name, temp->next->name);
                strcpy(temp->surname, temp->next->surname);
                temp->birthYear = temp->next->birthYear;

                strcpy(temp->next->name, tempName);
                strcpy(temp->next->surname, tempSurname);
                temp->next->birthYear = tempYear;

                swapped = 1;
            }
            temp = temp->next;
        }
        last = temp;
    } while (swapped);
    return head;
}

