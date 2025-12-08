#include "racuni.h"
#include <stdio.h>

int main() {
    RacunHead head = NULL;

    read_racuni("racuni.txt", &head);

    printf("\n--- SVI RACUNI SORTIRANI ---\n");
    ispis_artikli(head);

    char artikl[MAX_NAME];
    char start[11], end[11];

    printf("\nUnesite naziv artikla: ");
    scanf("%s", artikl);

    printf("Unesite pocetni datum (YYYY-MM-DD): ");
    scanf("%s", start);

    printf("Unesite krajnji datum (YYYY-MM-DD): ");
    scanf("%s", end);

    pregled_po_datumu(head, artikl, start, end);

    free_all(head);

    return 0;
}
