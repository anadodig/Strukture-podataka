#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef RACUNI_H
#define RACUNI_H

#define MAX_NAME 100

typedef struct Artikl {
    char naziv[MAX_NAME];
    int kolicina;
    float cijena;
    struct Artikl* next;
} Artikl;

typedef struct Racun {
    char datum[11];     
    Artikl* artikli;    
    struct Racun* next;
} Racun;

typedef Racun* RacunHead;
typedef Artikl* ArtiklHead;

void read_racuni(char* filename, RacunHead* head);

void insert_artikl_sorted(ArtiklHead* head, ArtiklHead novi);

void insert_racun_sorted(RacunHead* head, RacunHead novi);

int compare_dates(const char* date1, const char* date2);

void free_all(RacunHead head);

void pregled_po_datumu(RacunHead head, char* naziv, const char* start_date, const char* end_date);

void ispis_artikli(RacunHead head);

#endif

