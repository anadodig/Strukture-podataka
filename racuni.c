#include "racuni.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int compare_dates(const char* d1, const char* d2) {
    return strcmp(d1, d2);  
}

void insert_artikl_sorted(ArtiklHead* head, ArtiklHead novi) {
    if (*head == NULL || strcmp(novi->naziv, (*head)->naziv) < 0) {
        novi->next = *head;
        *head = novi;
        return;
    }

    ArtiklHead temp = *head;

    while (temp->next != NULL && strcmp(novi->naziv, temp->next->naziv) > 0) {
        temp = temp->next;
    }

    novi->next = temp->next;
    temp->next = novi;
}

void insert_racun_sorted(RacunHead* head, RacunHead novi) {
    if (*head == NULL || compare_dates(novi->datum, (*head)->datum) < 0) {
        novi->next = *head;
        *head = novi;
        return;
    }

    RacunHead temp = *head;

    while (temp->next != NULL && compare_dates(novi->datum, temp->next->datum) > 0) {
        temp = temp->next;
    }

    novi->next = temp->next;
    temp->next = novi;
}

void read_racuni(char* filename, RacunHead* head) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Error opening %s\n", filename);
        return;
    }

    char rac_filename[MAX_NAME];

    while (fscanf(f, "%s", rac_filename) == 1) {
        FILE* rf = fopen(rac_filename, "r");
        if (!rf) {
            printf("Error opening %s\n", rac_filename);
            continue;
        }

        RacunHead novi_racun = malloc(sizeof(Racun));
        novi_racun->artikli = NULL;
        novi_racun->next = NULL;

        fscanf(rf, "%s", novi_racun->datum);

        char naziv[MAX_NAME];
        int kol;
        float cij;

        while (fscanf(rf, "%s %d %f", naziv, &kol, &cij) == 3) {
            ArtiklHead novi = malloc(sizeof(Artikl));
            strcpy(novi->naziv, naziv);
            novi->kolicina = kol;
            novi->cijena = cij;
            novi->next = NULL;

            insert_artikl_sorted(&novi_racun->artikli, novi);
        }

        insert_racun_sorted(head, novi_racun);
        fclose(rf);
    }

    fclose(f);
}

void ispis_artikli(RacunHead head) {
    while (head) {
        printf("\nRacun: %s\n", head->datum);

        ArtiklHead a = head->artikli;
        while (a) {
            printf("  %-15s  x%d  %.2f\n", a->naziv, a->kolicina, a->cijena);
            a = a->next;
        }

        head = head->next;
    }
}

void pregled_po_datumu(RacunHead head, char* naziv,
    const char* start_date, const char* end_date)
{
    float ukupno = 0;
    int ukupna_kolicina = 0;

    while (head) {
        if (compare_dates(head->datum, start_date) >= 0 &&
            compare_dates(head->datum, end_date) <= 0)
        {
            ArtiklHead a = head->artikli;
            while (a) {
                if (strcmp(a->naziv, naziv) == 0) {
                    ukupno += a->kolicina * a->cijena;
                    ukupna_kolicina += a->kolicina;
                }
                a = a->next;
            }
        }
        head = head->next;
    }

    printf("\n--- REZULTAT ---\n");
    printf("Artikl: %s\n", naziv);
    printf("Ukupno potroseno: %.2f\n", ukupno);
    printf("Ukupna kolicina:  %d\n", ukupna_kolicina);
}

void free_all(RacunHead head) {
    while (head) {
        RacunHead tempR = head;
        head = head->next;

        ArtiklHead a = tempR->artikli;
        while (a) {
            ArtiklHead tempA = a;
            a = a->next;
            free(tempA);
        }

        free(tempR);
    }
}

  
