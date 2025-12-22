#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#define MAX 100

typedef struct Node {
	int data;
	struct Node* left;
	struct Node* right;
}Node;

Node* createNode(int value);
Node* insert(Node* root, int value);
int inorder(Node* root);
int preorder(Node* root);
int postorder(Node* root);
int levelorder(Node* root);
int search(Node* root, int value);
Node* deleteNode(Node* root, int value);

int main() {
	Node* root = NULL;
	int choice, choice2;
	int value;
	do {
		printf("\nIzaberite zeljenu radnju: \n");
		printf("1. Unos novog elementa\n");
		printf("2. Ispis elemenata: \n");
		printf("3. Brisanje\n");
		printf("4. Pronalazenje\n");
		printf("5. Kraj programa\n");
		scanf("%d", &choice);

		switch (choice) {
		case 1:
			printf("Koji element zelite unijeti: ");
			scanf("%d", &value);
			root = insert(root, value);
			break;
		case 2:
			printf("Izaberi nacin ispisa:\n");
			printf("   1. Inorder\n");
			printf("   2. Preorder\n");
			printf("   3. Postorder\n");
			printf("   4. Level order\n");
			scanf("%d", &choice2);
			switch (choice2) {
			case 1:
				inorder(root);
				break;
			case 2:
				preorder(root);
				break;
			case 3:
				postorder(root);
				break;
			case 4:
				levelorder(root);
				break;
			default:
				printf("Pogresan izbor!\n");
			}
			break;
		case 3:
			printf("Koji element zelite izbrisati: ");
			scanf("%d", &value);
			root = deleteNode(root, value);
			break;
		case 4:
			printf("Koji element zelite pronaci: ");
			scanf("%d", &value);
			if (search(root, value)) {
				printf("\nElement je pronadjen.\n");
			}
			else {
				printf("Element nije pronadjen.\n");
			}
			break;
		case 5:
			printf("\nKraj programa.\n");
			break;
		default:
			printf("\nPogresan izbor!\n");
		}
	} while (choice != 5);


	return 0;
}

Node* createNode(int value) {
	Node* newNode = (Node*)malloc(sizeof(Node));
	if (newNode == NULL) {
		printf("Alokacija nije uspjela!\n");
		return NULL;
	}

	newNode->data = value;
	newNode->left = NULL;
	newNode->right = NULL;

	return newNode;
}

Node* insert(Node* root, int value) {
	if (root == NULL)              //ako je stablo prazno:
		return createNode(value); //pravimo novi cvor
	if (value < root->data)
		root->left = insert(root->left, value);
	else if (value > root->data)
		root->right = insert(root->right, value);
	else
		printf("\nElement vec postoji.\n"); //ako postoji nista se ne mijenja

	return root;
}

int inorder(Node* root) {
	if (root == NULL)
		return 0;
	int lijevo = inorder(root->left);
	printf("%d ", root->data);
	int desno = inorder(root->right);

	return lijevo + 1 + desno; //1=>trenutni cvor
}

int preorder(Node* root) {
	if (root == NULL)
		return 0;
	printf("%d", root->data);
	int lijevo = preorder(root->left);
	int desno = preorder(root->right);

	return 1 + lijevo + desno;
}

int postorder(Node* root) {
	if (root == NULL)
		return 0;
	int lijevo = postorder(root->left);
	int desno = postorder(root->right);
	printf("%d ", root->data);

	return lijevo + desno + 1;
}

int levelorder(Node* root) {
	if (root == NULL)
		return 0;

	struct Node* queue[MAX]; //1. stvaramo red
	int front = 0;
	int rear = 0;
	int count = 0;

	queue[rear++] = root; //1.2. dodajemo korijen u red

	//dok red nije prazan
	while (front < rear) {
		struct Node* current = queue[front++]; //2. uzimamo el s pocetka
		printf("\n%d ", current->data);          //2.1. i ispisujemo ga
		count++;

		if (current->left != NULL) {         //ako postoji: 
			queue[rear++] = current->left;  //dodajemo lijevo dijete u red
		}
		if (current->right != NULL) {
			queue[rear++] = current->right; //isto za desno
		}
	}
	return count;
}

int search(Node* root, int value) {
	Node* current = root;

	while (current != NULL) {
		if (current->data == value) {
			printf("Pronadjeno: %d\n", current->data);
			return 1;
		}
		else if (value < current->data) {
			current = current->left;
		}
		else {
			current = current->right;
		}
	}
	return 0;
}

Node* deleteNode(Node* root, int value) {
	Node* parent = NULL;
	Node* current = root;

	//pronalazak cvora kojeg treba obrisati
	while (current != NULL && current->data != value) {
		parent = current;
		if (value < current->data)
			current = current->left;
		else
			current = current->right;
	}
	if (current == NULL) {
		printf("\nElement %d nije pronadjen!\n", value);
		return root;
	}
	//cvor ima 2 djeteta
	if (current->left != NULL && current->right != NULL) {
		Node* succParent = current;
		Node* succ = current->right;
		while (succ->left != NULL) {
			succParent = succ;
			succ = succ->left;
		}
		current->data = succ->data; //zamjena vrijednosti
		current = succ;
		parent = succParent;
	}
	//cvor ima 0/1 dijete
	Node* child = NULL;
	if (current->left != NULL)
		child = current->left;
	else 
		child = current->right;
	if (parent == NULL) {
		free(current); //brisemo root
		return child; //novi root
	}
	if (parent->left == current)
		parent->left = child;
	else
		parent->right = child;

	free(current);
	return root;
}
