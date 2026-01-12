#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct CityTree {
    char name[50];
    int num_citizens;
    struct CityTree* left;
    struct CityTree* right;
}CityTree;

typedef struct StateList {
    char name[50];
    CityTree* root;
    struct StateList* next;
}StateList;

typedef struct CityList {
    char name[50];
    int num_citizens;
    struct CityList* next;
}CityList;

typedef struct StateTree {
    char name[50];
    CityList* head_list;
    struct StateTree* left;
    struct StateTree* right;
}StateTree;

StateList* read_from_file_list(char* filename);
StateList* create_state_list_node(char* name);
StateList* insert_state_sorted(StateList* head, StateList* newNode);
CityTree* create_city_node(char* name, int population);
CityTree* insert_city_tree(CityTree* root, CityTree* newNode);
CityTree* read_cities_from_file(char* filename);
void print_city_tree(CityTree* root);
void search_city_tree(CityTree* root, int min_population);
StateList* find_state(StateList* head, char* name);
StateTree* insert_state_tree(StateTree* root, StateTree* newNode);
CityList* insert_city_list(CityList* head, CityList* newNode);
StateTree* create_state_tree_node(char* name); 
CityList* create_city_list_node(char* name, int population);
CityList* read_cities_to_list(char* filename);
StateTree* read_from_file_tree(char* filename);
void print_state_tree(StateTree* root);
void search_city_list(CityList* head, int min_population);
StateTree* find_state_tree(StateTree* root, char* name);

int main() {
        char state_name[50];
        int min_population;

        StateList* state_list = read_from_file_list("drzave.txt");

        printf("\n-- SORTIRANA LISTA DRZAVA --\n");
        StateList* temp = state_list;
        while (temp) {
            printf("%s\n", temp->name);
            print_city_tree(temp->root);
            temp = temp->next;
        }

        printf("\n[PRETRAGA - DIO A]\n");
        printf("Unesite ime drzave: ");
        scanf("%49s", state_name);

        printf("Unesite minimalan broj stanovnika: ");
        scanf("%d", &min_population);

        StateList* found_list = find_state(state_list, state_name);
        if (!found_list) {
            printf("Drzava ne postoji!\n");
        }
        else {
            printf("Gradovi s vise od %d stanovnika:\n", min_population);
            search_city_tree(found_list->root, min_population);
        }

        StateTree* state_tree = read_from_file_tree("drzave.txt");

        printf("\n-- STABLO DRZAVA --\n");
        print_state_tree(state_tree);

        printf("\n[PRETRAGA - DIO B]\n");
        printf("Unesite ime drzave: ");
        scanf("%49s", state_name);

        printf("Unesite minimalan broj stanovnika: ");
        scanf("%d", &min_population);

        StateTree* found_tree = find_state_tree(state_tree, state_name);
        if (!found_tree) {
            printf("Drzava ne postoji!\n");
        }
        else {
            printf("Gradovi s vise od %d stanovnika:\n", min_population);
            search_city_list(found_tree->head_list, min_population);
        }

        return 0;
    }

StateList* read_from_file_list(char* filename) {
    StateList* head = NULL;
    char state_name[50];
    char city_filename[50];
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Ne mogu otvoriti %s\n", filename);
        return NULL;
    }
    while (fscanf(file, "%49s %49s", state_name, city_filename) == 2) {
        StateList* newState = create_state_list_node(state_name);
        if (newState == NULL) {
            printf("Greska alokacije za drzavu %s\n", state_name);
            fclose(file);
            return head; // prekid u slucaju greske
        }
        newState->root = read_cities_from_file(city_filename);
        head = insert_state_sorted(head, newState);
    }
    fclose(file);
    return head;
}

StateList* create_state_list_node(char* name) {
    StateList* newNode = malloc(sizeof(StateList));
    if (newNode == NULL) {
        printf("Greska pri alokaciji\n");
        return NULL;
    }
    strcpy(newNode->name, name);
    newNode->root = NULL;
    newNode->next = NULL;

    return newNode;
}

StateList* insert_state_sorted(StateList* head, StateList* newNode) {
    if (head == NULL || strcmp(newNode->name, head->name) < 0) {
        newNode->next = head;
        return newNode;
    }
    StateList* current = head;
    while (current->next != NULL && strcmp(newNode->name, current->next->name) > 0) {
        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;

    return head;
}

CityTree* create_city_node(char* name, int population) {
    CityTree* newNode = malloc(sizeof(CityTree));
    if (newNode == NULL) {
        printf("Alokacija nije uspjela\n");
        return NULL;
    }

    strcpy(newNode->name, name);
    newNode->num_citizens = population;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

CityTree* insert_city_tree(CityTree* root, CityTree* newNode) {
    if (root == NULL) // prvo provjeri root
        return newNode;

    if (newNode->num_citizens < root->num_citizens)
        root->left = insert_city_tree(root->left, newNode);
    else if (newNode->num_citizens > root->num_citizens)
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
    if (file == NULL) {
        printf("Ne mogu otvoriti %s\n", filename);
        return NULL;
    }
    CityTree* root = NULL;
    char city_name[50];
    int population;

    while (fscanf(file, " %49[^,], %d", city_name, &population) == 2) {
        CityTree* newCity = create_city_node(city_name, population);
        if (newCity == NULL) {
            printf("Greska alokacije za grad %s\n", city_name);
            fclose(file);
            return root;
        }
        root = insert_city_tree(root, newCity);
    }
    fclose(file);
    return root;
}

void print_city_tree(CityTree* root) {
    if (!root) return;
    print_city_tree(root->left);
    printf("  %s (%d)\n", root->name, root->num_citizens);
    print_city_tree(root->right);
}

void search_city_tree(CityTree* root, int min_population) {
    if (root == NULL)
        return;

    if (root->num_citizens > min_population) {
        search_city_tree(root->left, min_population);
        printf("   %s (%d)\n", root->name, root->num_citizens);
        search_city_tree(root->right, min_population);
    }
    else {
        search_city_tree(root->right, min_population);
    }
}

StateList* find_state(StateList* head, char* name) {
    while (head != NULL) {
        if (strcmp(head->name, name) == 0)
            return head;
        head = head->next;
    }
    return NULL;
}

StateTree* insert_state_tree(StateTree* root, StateTree* newNode) {
    if (root == NULL)
        return newNode;

    if (strcmp(newNode->name, root->name) < 0)
        root->left = insert_state_tree(root->left, newNode);
    else
        root->right = insert_state_tree(root->right, newNode);

    return root;
}

CityList* insert_city_list(CityList* head, CityList* newNode) {
    if (head == NULL ||
        newNode->num_citizens < head->num_citizens ||
        (newNode->num_citizens == head->num_citizens &&
            strcmp(newNode->name, head->name) < 0)) {

        newNode->next = head;
        return newNode;
    }

    CityList* curr = head;
    while (curr->next &&
        (curr->next->num_citizens < newNode->num_citizens ||
            (curr->next->num_citizens == newNode->num_citizens &&
                strcmp(curr->next->name, newNode->name) < 0))) {

        curr = curr->next;
    }

    newNode->next = curr->next;
    curr->next = newNode;
    return head;
}

StateTree* create_state_tree_node(char* name) {
    StateTree* newNode = malloc(sizeof(StateTree));
    if (!newNode) {
        printf("Greska alokacije za drzavu\n");
        return NULL;
    }
    strcpy(newNode->name, name);
    newNode->head_list = NULL;
    newNode->left = newNode->right = NULL;
    return newNode;
}

CityList* create_city_list_node(char* name, int population) {
    CityList* newNode = malloc(sizeof(CityList));
    if (!newNode) {
        printf("Greska alokacije za grad\n");
        return NULL;
    }
    strcpy(newNode->name, name);
    newNode->num_citizens = population;
    newNode->next = NULL;
    return newNode;
}

CityList* read_cities_to_list(char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Ne mogu otvoriti %s\n", filename);
        return NULL;
    }

    CityList* head = NULL;
    char city_name[50];
    int population;

    while (fscanf(file, " %49[^,], %d", city_name, &population) == 2) {
        CityList* newCity = create_city_list_node(city_name, population);
        head = insert_city_list(head, newCity);
    }

    fclose(file);
    return head;
}

StateTree* read_from_file_tree(char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Ne mogu otvoriti %s\n", filename);
        return NULL;
    }

    StateTree* root = NULL;
    char state_name[50], city_file[50];

    while (fscanf(file, "%49s %49s", state_name, city_file) == 2) {
        StateTree* newState = create_state_tree_node(state_name);
        newState->head_list = read_cities_to_list(city_file);
        root = insert_state_tree(root, newState);
    }

    fclose(file);
    return root;
}

void print_state_tree(StateTree* root) {
    if (!root) return;

    print_state_tree(root->left);
    printf("%s\n", root->name);

    CityList* c = root->head_list;
    while (c) {
        printf("  %s (%d)\n", c->name, c->num_citizens);
        c = c->next;
    }

    print_state_tree(root->right);
}

void search_city_list(CityList* head, int min_population) {
    while (head) {
        if (head->num_citizens > min_population)
            printf("  %s (%d)\n", head->name, head->num_citizens);
        head = head->next;
    }
}

StateTree* find_state_tree(StateTree* root, char* name) {
    if (!root) return NULL;

    int cmp = strcmp(name, root->name);
    if (cmp == 0) return root;
    if (cmp < 0) return find_state_tree(root->left, name);
    return find_state_tree(root->right, name);
}



