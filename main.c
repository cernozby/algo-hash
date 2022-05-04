#include <stdio.h>
#include "stdlib.h"

typedef struct Node {
    int key;
    struct Node *next ;
} Node;

typedef struct List {
    struct Node *first ;
} List;

Node* node_new(int key) {
    Node *node = malloc(sizeof(Node));
    node->key = key;
    node->next = NULL;
    return node;
}

void add_end (List* list, int key) {
    Node *actualNode = list->first;
    Node *node = node_new(key);
    if(actualNode == NULL) {list->first = node; return;}

    while(actualNode->next != NULL) {
        actualNode = actualNode->next;
    }
    actualNode->next = node;

}

void print_list (List* list) {
    if(!list->first) {return;}
    Node *actualNode = list->first;
    printf("(");
    while(actualNode->next != NULL) {
        printf("%d, ", actualNode->key);
        actualNode = actualNode->next;
    }
    printf("%d)\n", actualNode->key);
}

void add_start (List* list, int key) {
    Node *node = node_new(key);
    node->next = list->first;
    list->first = node;
}

int length (List* list) {
    Node *actualNode = list->first;
    if(actualNode == NULL) {return 0;}
    int iter = 1;
    while (actualNode->next != NULL) {
        iter++;
        actualNode = actualNode->next;
    }

    return iter;
}

int search (List* list, int key) {
    Node *node = node_new(key);
    Node *actualNode = list->first;

    int index = 0;
    while (actualNode != NULL) {
        if(node->key == actualNode->key) {
            return index;
        }
        index++;
        actualNode = actualNode->next;
    }

    return -1;
}

void fillList(List *list) {

    for(int i = 2; i < 10; i++) {
        add_end(list, i);
    }
}

int remove_start (List* list) {
    if(list->first == NULL) {-1;}
    Node *firstItem = list->first;
    list->first = list->first->next;
    free(firstItem);
    return 0;
}

int remove_end (List* list) {
    if(list->first == NULL) {return -1;}
    if(list->first->next == NULL) {
        free(list->first);
        list->first = NULL;
        return 0;
    }
    Node *actualNode = list->first;

    while(actualNode->next->next != NULL) {
        actualNode = actualNode->next;
    }
    Node *removeItem = actualNode->next;
    actualNode->next = NULL;
    free(removeItem);
    return 0;
}

int remove_node (List* list, int key) {
    if(!list->first) {return -1;}
    Node *node = node_new(key);
    Node *actualNode = list->first;
    if(actualNode->key == node->key) {
        return remove_start(list);
    }
    //nenajde prvni a posledni
    while (actualNode->next->next != NULL) {
        if (node->key == actualNode->next->key) {
            Node* remove = actualNode->next;
            actualNode->next = actualNode->next->next;
            free(remove);
            return 0;
        }
        actualNode = actualNode->next;
    }

    if(actualNode->next->key == node->key) {
        return remove_end(list);
    }
    return -1;
}


typedef struct Chained_Hash_Table {
    List** table;
    int size;
} Chained_Hash_Table;


typedef struct Open_Addressing_Table {
    unsigned int* table;
    int size;
    unsigned int (*hash_func)(struct Open_Addressing_Table*, int , int);
} Open_Addressing_Table;

unsigned int my_hash_func(struct Open_Addressing_Table* o, int key, int index) {
    return (key % o->size + index) % o->size;
}

Open_Addressing_Table* init_open_addressing_table(int size, unsigned int (*h_f)(struct Open_Addressing_Table*, int , int)) {
    Open_Addressing_Table *o = malloc(sizeof (Open_Addressing_Table));
    o->size = size;
    o->table = malloc(size * sizeof (unsigned int));

    for (int i = 0; i < size; i++) {
        o->table[i] = 0;
    }

    o->hash_func = h_f;
}



Chained_Hash_Table* init_chained_hash_table (int size){
    Chained_Hash_Table* ch = malloc(sizeof (Chained_Hash_Table));
    ch->size = size;
    ch->table = malloc(size * sizeof(List*));
    for (int i = 0; i < size; i++) {
        ch->table[i] = (List*) malloc(sizeof(List));
        ch->table[i]->first = NULL;
    }

    return ch;
}

int get_chained_hash(int key, int size) {
    if(key < 0) {
        key = key * -1;
    }
    return key % size;
}

int add_chained_hash (Chained_Hash_Table* table, int key) {
    int index = get_chained_hash(key, table->size);

    //if key exist, didnt add it;
    if(search(table->table[index], key) == 0) {
        return -1;
    }

    add_end(table->table[index], key);
    return 0;
}

int search_chained_hash (Chained_Hash_Table* table, int key) {
    return search(table->table[get_chained_hash(key, table->size)], key);
}

int remove_chained_hash (Chained_Hash_Table* table, int key) {
    return remove_node(table->table[get_chained_hash(key, table->size)], key);
}

int search_oa_hash (Open_Addressing_Table* table, unsigned int key) {
    for(int i = 0; i < table->size; i++) {
        unsigned int index = table->hash_func(table, key, i);
        if (table->table[index] == key) {
            return key;
        }
    }
    return -1;
}

int add_oa_hash (Open_Addressing_Table* table, unsigned int key) {
    for(int i = 0; i < table->size; i++) {
        unsigned int index = table->hash_func(table, key, i);
        if (table->table[index] == 0) {
            table->table[index] = key;
            return 0;
        }

        if(table->table[index] == key) {
            return -1;
        }
    }

    return -1;
}

int remove_oa_hash (Open_Addressing_Table* table, unsigned int key) {
    for(int i = 0; i < table->size; i++) {
        unsigned int index = table->hash_func(table, key, i);
        if (table->table[index] == key) {
            table->table[index] = 0;
            return 0;
        }
    }
    return -1;
}



int main() {

    Chained_Hash_Table* ch = init_chained_hash_table(10);
    printf("Uspesne pridani 1: %d\n", add_chained_hash(ch, 1));
    printf("Uspesne pridani 2: %d\n", add_chained_hash(ch, 11));
    printf("Uspesne pridani 12: %d\n", add_chained_hash(ch, 12));
    printf("Neuspesne pridani 12: %d\n", add_chained_hash(ch, 12));
    printf("uspesne hledani 12: %d\n", search_chained_hash(ch, 12));
    printf("Neuspesne hledani 13: %d\n", search_chained_hash(ch, 13));
    printf("uspesne odbrani 12: %d\n", remove_chained_hash(ch, 12));
    printf("Neuspesne hledani 12: %d\n", search_chained_hash(ch, 12));
    printf("Neuspesne odbrani 12: %d\n", remove_chained_hash(ch, 12));

    printf("\n\n");


    Open_Addressing_Table* o = init_open_addressing_table(10, my_hash_func);
    printf("Uspesne pridani 1: %d\n", add_oa_hash(o, 1));
    printf("Uspesne pridani 11: %d\n", add_oa_hash(o, 11));
    printf("Uspesne pridani 12: %d\n", add_oa_hash(o, 12));
    printf("Neuspesne pridani 12: %d\n", add_oa_hash(o, 12));
    printf("uspesne hledani 12: %d\n", search_oa_hash(o, 12));
    printf("Neuspesne hledani 13: %d\n", search_oa_hash(o, 13));
    printf("uspesne odbrani 12: %d\n", remove_oa_hash(o, 12));
    printf("Neuspesne hledani 12: %d\n", search_oa_hash(o, 12));
    printf("Neuspesne odbrani 12: %d\n", remove_oa_hash(o, 12));



    printf("Hello, World!\n");
    return 0;
}
