#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define ERROR_OF -1
#define MAX_BODOVI 35

typedef struct student {
	char ime[30];
	char prezime[30];
	int bodovi;
}Student;

int brojStudenata(char* filename);
Student* citanje(char* filename, int broj_studenata);
void ispis(Student* studenti, int broj_studenata);

int main() 
{
	//broj studenata
	char* filename = "studenti.txt";
	int broj_studenata = brojStudenata(filename); //poziv
	printf("Ukupan broj studenata je: %d\n", broj_studenata);

	//alokacija
	Student* studenti = citanje(filename, broj_studenata);
	if (studenti == NULL) 
	{
		return NULL;
	}

	//ispis
	ispis(studenti, broj_studenata);

	//oslobadanje memorije
	free(studenti);
	return 0;
}

int brojStudenata(char*filename) 
{
	//otvaranje .txt datoteke
	FILE* file = fopen(filename, "r");
	if (file == NULL) 
	{
		printf("Datoteka se ne moze otvorii.");
		return 0;
	}

	//brojanje redaka
	char red[200];
	int brojac = 0;

	while (fgets(red, sizeof(red), file) != NULL) 
	{
		brojac++;
	}

	fclose(file);
	return brojac;
}

Student* citanje(char* filename, int broj_studenata) 
{
	//otvaranje datoteke
	FILE* file = fopen(filename, "r");
	if (file == NULL) 
	{
		printf("neuspjelo otvaranje");
		return 1;
	}

	//alokacija
	Student* studenti = (Student*)malloc(broj_studenata * sizeof(Student));
	if (studenti == NULL) 
	{
		printf("Alokacija nije uspjela");
		return 1;
	}

	//citanje podataka
	for (int i = 0; i < broj_studenata; i++) 
	{
		fscanf(file, "%s %s %d", studenti[i].ime, studenti[i].prezime, &studenti[i].bodovi);
	}
	fclose(file);
	return studenti;
}

void ispis(Student* studenti, int broj_studenata) {

	for (int i = 0; i < broj_studenata; i++) {
		//zadana formula za relativne bodove
		double relativni_bodovi = ((double)studenti[i].bodovi / MAX_BODOVI) * 100;
		printf("Ime: %s\tPrezime: %s\tBodovi: %d\tRelativni: %.2f\n", studenti[i].ime, studenti[i].prezime, studenti[i].bodovi, relativni_bodovi);
	}
}
