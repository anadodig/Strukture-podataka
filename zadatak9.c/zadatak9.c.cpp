#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RANDOM_MIN 10
#define RANDOM_MAX 90
#define MAX_QUEUE 100

typedef struct Node* Position;
typedef struct Node {
    int data;
    Position left;
    Position right;
} Node;

typedef struct {
    Position elements[MAX_QUEUE];
    int front;
    int rear;
} Queue;

Position createNode(int value);
Position insert(Position root, int value);
int replace(Position root);
void inorder(Position root, FILE* file);
void freeTree(Position root);

void initQueue(Queue* q) {
    q->front = 0;
    q->rear = 0;
}

int isEmpty(Queue* q) {
    return q->front == q->rear;
}

void enqueue(Queue* q, Position p) {
    q->elements[q->rear++] = p;
}

Position dequeue(Queue* q) {
    return q->elements[q->front++];
}

Position createNode(int value) {
    Position p = (Position)malloc(sizeof(Node));
    if (!p) {
        printf("Greska pri alokaciji memorije!\n");
        return NULL;
    }
    p->data = value;
    p->left = NULL;
    p->right = NULL;
    return p;
}

Position insert(Position root, int value) {
    Position newNode = createNode(value);
    if (!root)
        return newNode;

    Queue q;
    initQueue(&q);
    enqueue(&q, root);

    while (!isEmpty(&q)) {
        Position current = dequeue(&q);

        if (!current->left) {
            current->left = newNode;
            return root;
        }
        else if (!current->right) {
            current->right = newNode;
            return root;
        }
        else {
            enqueue(&q, current->left);
            enqueue(&q, current->right);
        }
    }
    return root;
}

int replace(Position root) {
    if (!root)
        return 0;

    int leftSum = replace(root->left);
    int rightSum = replace(root->right);

    int oldValue = root->data;
    root->data = leftSum + rightSum;

    return oldValue + root->data;
}

void inorder(Position root, FILE* file) {
    if (!root)
        return;

    inorder(root->left, file);
    fprintf(file, "%d ", root->data);
    inorder(root->right, file);
}

void freeTree(Position root) {
    if (!root)
        return;

    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    Position root = NULL;
    int n = 10;

    srand((unsigned)time(NULL));

    for (int i = 0; i < n; i++) {
        int x = rand() % (RANDOM_MAX - RANDOM_MIN + 1) + RANDOM_MIN;
        root = insert(root, x);
    }

    FILE* file = fopen("file.txt", "w");
    if (!file) {
        printf("Greska pri otvaranju datoteke!\n");
        return 1;
    }

    fprintf(file, "Inorder prije replace:\n");
    inorder(root, file);
    fprintf(file, "\n");

    replace(root);

    fprintf(file, "Inorder poslije replace:\n");
    inorder(root, file);
    fprintf(file, "\n");

    fclose(file);
    freeTree(root);

    return 0;
}

