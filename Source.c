#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50

typedef struct Directory {
    char name[MAX_NAME];
    struct Directory* child;
    struct Directory* sibling;
    struct Directory* parent;
} Directory;

Directory* createDirectory(const char* name, Directory* parent);
int makeDirectory(Directory* current, const char* name);
Directory* changeDirectory(Directory* current, const char* name);
Directory* goToParent(Directory* current);
void listDirectory(Directory* current);
void printCurrentPath(Directory* current);
void freeDir(Directory* dir);

int main() {
    Directory* root = createDirectory("C:", NULL);
    Directory* current = root;

    int choice;
    char dirName[MAX_NAME];

    printf("========================================\n");
    printf("  SIMULATOR STRUKTURE DIREKTORIJA\n");
    printf("========================================\n\n");

    while (1) {
        printCurrentPath(current);
        printf(">\n");
        printf("\n--- MENI ---\n");
        printf("1. md (napravi direktorij)\n");
        printf("2. cd dir (promijeni direktorij)\n");
        printf("3. cd.. (vrati se na roditeljski direktorij)\n");
        printf("4. dir (ispisi sadrzaj direktorija)\n");
        printf("5. Izlaz\n");
        printf("Izbor: ");

        if (scanf("%d", &choice) != 1) {
            printf("Neispravan unos!\n");
            while (getchar() != '\n');  // cisti preostali unos
            continue;
        }

        // cisti preostali unos linije nakon scanf
        while (getchar() != '\n');

        switch (choice) {
        case 1:
            printf("Unesite ime novog direktorija: ");
            fgets(dirName, MAX_NAME, stdin);
            dirName[strcspn(dirName, "\n")] = 0;  // Uklanja '\n' znak
            makeDirectory(current, dirName);
            break;
        case 2:
            printf("Unesi ime direktorija: ");
            fgets(dirName, MAX_NAME, stdin);
            dirName[strcspn(dirName, "\n")] = 0;  // Uklanja '\n' znak
            current = changeDirectory(current, dirName);
            break;
        case 3:
            current = goToParent(current);
            break;
        case 4:
            listDirectory(current);
            break;
        case 5:
            printf("\nIzlazak\n");
            freeDir(root);
            return 0;
        default:
            printf("Greska!\n");
        }
    }

    return 0;
}

Directory* createDirectory(const char* name, Directory* parent) {
    Directory* newDir = (Directory*)malloc(sizeof(Directory));
    if (newDir == NULL) {
        printf("Greska pri alokaciji memorije!\n");
        return NULL;
    }

    strcpy(newDir->name, name);
    newDir->child = NULL;
    newDir->sibling = NULL;
    newDir->parent = parent;

    return newDir;
}

int makeDirectory(Directory* current, const char* name) {
    if (current == NULL || name == NULL || strlen(name) == 0) {
        return 0;
    }

    // Provjera postoji li vec direktorij s tim imenom
    Directory* temp = current->child;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            printf("Direktorij '%s' vec postoji!\n", name);
            return 0;
        }
        temp = temp->sibling;
    }

    // Kreiranje novog direktorija
    Directory* newDir = createDirectory(name, current);
    if (newDir == NULL) {
        return 0;
    }

    // Dodavanje na pocetak liste djece
    newDir->sibling = current->child;
    current->child = newDir;

    printf("Direktorij '%s' uspjesno kreiran.\n", name);
    return 1;
}

Directory* changeDirectory(Directory* current, const char* name) {
    if (current == NULL || name == NULL) {
        return current;
    }

    Directory* temp = current->child;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            printf("Presao u direktorij: %s\n", name);
            return temp;
        }
        temp = temp->sibling;
    }

    printf("Direktorij '%s' ne postoji!\n", name);
    return current;
}

Directory* goToParent(Directory* current) {
    if (current->parent != NULL) {
        printf("Vraceno u direktorij: %s\n", current->parent->name);
        return current->parent;
    }
    else {
        printf("Vec ste u root direktoriju!\n");
        return current;
    }
}

void listDirectory(Directory* current) {
    if (current == NULL) {
        return;
    }

    printf("\nSadrzaj direktorija [%s]:\n", current->name);
    printf("----------------------------------------\n");

    if (current->child == NULL) {
        printf("  (prazan direktorij)\n");
    }
    else {
        Directory* temp = current->child;
        int count = 0;
        while (temp != NULL) {
            printf("  [DIR]  %s\n", temp->name);
            count++;
            temp = temp->sibling;
        }
        printf("----------------------------------------\n");
        printf("Ukupno: %d direktorij(a)\n", count);
    }
}

void printCurrentPath(Directory* current) {
    if (current == NULL) {
        return;
    }

    // Rekurzivno ispisujemo putanju
    if (current->parent != NULL) {
        printCurrentPath(current->parent);
        printf("\\");
    }
    printf("%s", current->name);
}

void freeDir(Directory* dir) {
    if (dir == NULL) {
        return;
    }

    // Rekurzivno oslobodi djecu
    Directory* child = dir->child;
    while (child != NULL) {
        Directory* next = child->sibling;
        freeDir(child);
        child = next;
    }

    // Oslobodi trenutni direktorij
    free(dir);
}

