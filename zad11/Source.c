#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 11

typedef struct CityTree {
    char name[50];
    int population;
    struct CityTree* left;
    struct CityTree* right;
} CityTree;

typedef struct StateHash {
    char name[50];
    CityTree* root;
    struct StateHash* next;
} StateHash;

int hash_function(char* name);
CityTree* create_city(char* name, int population);
CityTree* insert_city_tree(CityTree* root, CityTree* newNode);
CityTree* read_cities_from_file(char* filename);
void print_city_tree(CityTree* root);
void search_city_tree(CityTree* root, int min_population);

StateHash* insert_state_sorted(StateHash* head, StateHash* newNode);
void read_states_to_hash(char* filename, StateHash* table[]);
void print_hash_table(StateHash* table[]);
StateHash* find_state(StateHash* table[], char* name);

int main() {
    StateHash* hashTable[TABLE_SIZE] = { NULL };
    char state_name[50];
    int min_population;

    read_states_to_hash("drzave.txt", hashTable);

    printf("\n--- HASH TABLICA DRZAVA ---\n");
    print_hash_table(hashTable);

    printf("\nUnesite ime drzave: ");
    scanf("%49s", state_name);

    printf("Unesite minimalan broj stanovnika: ");
    scanf("%d", &min_population);

    StateHash* found = find_state(hashTable, state_name);
    if (!found) {
        printf("Drzava ne postoji!\n");
    }
    else {
        printf("Gradovi s vise od %d stanovnika:\n", min_population);
        search_city_tree(found->root, min_population);
    }

    return 0;
}

int hash_function(char* name) {
    int sum = 0;
    for (int i = 0; i < 5 && name[i] != '\0'; i++)
        sum += (int)name[i];
    return sum % TABLE_SIZE;
}

CityTree* create_city(char* name, int population) {
    CityTree* node = malloc(sizeof(CityTree));
    if (!node) return NULL;

    strcpy(node->name, name);
    node->population = population;
    node->left = node->right = NULL;
    return node;
}

CityTree* insert_city_tree(CityTree* root, CityTree* newNode) {
    if (!root) return newNode;

    if (newNode->population < root->population)
        root->left = insert_city_tree(root->left, newNode);
    else if (newNode->population > root->population)
        root->right = insert_city_tree(root->right, newNode);
    else {
        if (strcmp(newNode->name, root->name) < 0)
            root->left = insert_city_tree(root->left, newNode);
        else
            root->right = insert_city_tree(root->right, newNode);
    }
    return root;
}

CityTree* read_cities_from_file(char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return NULL;

    CityTree* root = NULL;
    char city[50];
    int population;

    while (fscanf(file, " %49[^,], %d", city, &population) == 2) {
        CityTree* newCity = create_city(city, population);
        root = insert_city_tree(root, newCity);
    }

    fclose(file);
    return root;
}

void print_city_tree(CityTree* root) {
    if (!root) return;
    print_city_tree(root->left);
    printf("  %s (%d)\n", root->name, root->population);
    print_city_tree(root->right);
}

void search_city_tree(CityTree* root, int min_population) {
    if (!root) return;

    if (root->population > min_population) {
        search_city_tree(root->left, min_population);
        printf("  %s (%d)\n", root->name, root->population);
        search_city_tree(root->right, min_population);
    }
    else {
        search_city_tree(root->right, min_population);
    }
}

StateHash* insert_state_sorted(StateHash* head, StateHash* newNode) {
    if (!head || strcmp(newNode->name, head->name) < 0) {
        newNode->next = head;
        return newNode;
    }

    StateHash* curr = head;
    while (curr->next && strcmp(newNode->name, curr->next->name) > 0)
        curr = curr->next;

    newNode->next = curr->next;
    curr->next = newNode;
    return head;
}

void read_states_to_hash(char* filename, StateHash* table[]) {
    FILE* file = fopen(filename, "r");
    if (!file) return;

    char state[50], cityFile[50];

    while (fscanf(file, "%49s %49s", state, cityFile) == 2) {
        StateHash* newState = malloc(sizeof(StateHash));
        strcpy(newState->name, state);
        newState->root = read_cities_from_file(cityFile);
        newState->next = NULL;

        int key = hash_function(state);
        table[key] = insert_state_sorted(table[key], newState);
    }

    fclose(file);
}

void print_hash_table(StateHash* table[]) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        printf("\n[%d]\n", i);
        StateHash* s = table[i];
        while (s) {
            printf("%s\n", s->name);
            print_city_tree(s->root);
            s = s->next;
        }
    }
}

StateHash* find_state(StateHash* table[], char* name) {
    int key = hash_function(name);
    StateHash* s = table[key];

    while (s) {
        if (strcmp(s->name, name) == 0)
            return s;
        s = s->next;
    }
    return NULL;
}




