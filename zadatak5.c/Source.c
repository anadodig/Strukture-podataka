#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<ctype.h>
#define MAX_BUFFER 1024

typedef struct _postfix* Position;
typedef struct _postfix{
	double number;
    Position next;
}Postfix;

int Push(Position head, double num);
double Pop(Position head);
void read_file(char* filename, Position head);

int main() {
	Postfix head = { .number = 0, .next = NULL }; //broj postavlja na 0, a stog je prazan

	read_file("postfix.txt", &head); //pozivamo funkciju i saljemo head liste

	return 0;
}

//dodavanje elementa na stog
int Push(Position head, double number) {
	//alokacija novog elementa
	Position q =(Position)malloc(sizeof(Postfix));
	if (q == NULL) {
		printf("Greska pri alokaciji memorije");
		return -1;
	}

	q->number = number;      //stavljamo zeljeni broj u novo alocirani element
	q->next = head->next;//novi el pokazuje na trenutni prvi el
	head->next = q;     //postavljamo head da pokazuje na q
}

//skidanje elementa sa stoga
double Pop(Position head) {
	//provjera jel stog prazan
	if (head->next == NULL) {
		printf("stog je prazan\n");
		return 0;
	}
	Position temp = head->next;  //temp cuva pokazivac na el koji zelimo ukloniti a to je prvi el
	double value = temp->number;//value je broj koji je bio na vrhu stoga, znaci cuva vrijednost onoga sto uklanjamo
	head->next = temp->next;   //preskacemo prvi el
	free(temp);               //oslobadjamo prvi
	return value;            //vraca vrijednost broja koji smo uklonili
}

//citanje iz datoteke
void read_file(char* filename, Position head) {
	//otvaranje datoteke
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("Neuspjelo otvaranje datoteke");
		return;
	}

	char buffer[MAX_BUFFER]; //za citanje linija iz datoteke
	//int lineNum = 0;

	while (fgets(buffer, sizeof(buffer), file)) {
		char* token = strtok(buffer, " \t\n");
		while (token != NULL) {
			if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
				//broj stavi na stog
				double number = atof(token);
				Push(head, number);
			}
			else if (strlen(token) == 1 && strchr("+-*/", token[0])) {
				//uzmi 2 br i izracunaj
				double b = Pop(head);
				double a = Pop(head);
				double result;

				switch (token[0]) {
				case '+': result = a + b; break;
				case '-': result = a - b; break;
				case '*': result = a * b; break;
				case '/':
					if (b != 0) result = a / b;
					else {
						printf("Greska! Dijeljenje s nulom!\n");
						fclose(file);
						return;
					}
					break;
				}
				Push(head, result);
			}
			token = strtok(NULL, " \t\n");
		}
	}
	//rez je na vrhu stoga
	double rezultat = Pop(head);
	printf("Rezultat: %.2f\n", rezultat);

	fclose(file);
}