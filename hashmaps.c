#include "hashmaps.h"
#include <stdlib.h>
#include <stdio.h>

int domain_list_len(Node *domain_list) {
    int size = 0;

    while (domain_list != NULL) {
        size++;
        domain_list = domain_list -> next;
    }
    return size;
}

int board_is_solved(Domains *board_domains, const char *rows, int row_len, const char *cols, int col_len) {
    int x = 0, y = 0, is_solved = 1, hash;
    char space[2];
    Node *domain_list;

    for (; x < row_len; x++) {
        for (; y < col_len; y++) {

            space[0] = rows[x];
            space[1] = cols[y];

            hash = hash_code(space);

            domain_list = board_domains -> values[hash];

            if (domain_list_len(domain_list) > 1) {
                return 0;
            }
        }
        y = 0;
    }
    return is_solved;
}
void print_unsolved_domains(Domains *board_domains, const char *rows, int row_len, const char *cols, int col_len) {
    int x = 0, y = 0, hash;
    char value;
    char space[2];

    for (; x < row_len; x++) {
        printf("|");
        for (; y < col_len; y++) {

            space[0] = rows[x];
            space[1] = cols[y];

            hash = hash_code(space);

            if (domain_list_len(board_domains -> values[hash]) == 1) {
                value = board_domains -> values[hash] -> value;
                printf("%c|", value);
            } else {
                printf("0|");
            }
        }
        printf("\n");
        y = 0;
    }
}

void print_solved_domains(Domains *board_domains, const char *rows, int row_len, const char*cols, int col_len) {
    int x = 0, y = 0, hash;
    char value;
    char space[2];

    for (; x < row_len; x++) {
        printf("|");
        for (; y < col_len; y++) {

            space[0] = rows[x];
            space[1] = cols[y];

            hash = hash_code(space);

            value = board_domains -> values[hash] -> value;
            printf("%c|", value);
        }
        printf("\n");
        y = 0;
    }
}

void print_domain_list(Node *head) {

    while (head != NULL ) {
        printf("%c", head -> value);
        if ( head -> next != NULL ) {
            printf(", ");
        }
        head = head -> next;
    }
}

void print_arc_list(Arc_List *head) {
    while( head != NULL ) {
        printf("%s", head -> value);
        if ( head -> next != NULL ) {
            printf(", ");
        }
        head = head -> next;
    }
}

void print_domains(Domains *board_domains, const char *rows, int row_len, const char *cols, int col_len) {
    char space[3];
    int x = 0, y = 0, hash;

    space[2] = '\0';

    for(;x < row_len; x++){
        for(;y < col_len; y++) {

            space[0] = rows[x];
            space[1] = cols[y];

            /* insert the tile into the board coordinates mapping */
            hash = hash_code(space);
            printf("%s: ", space);
            print_domain_list(board_domains -> values[hash]);
            printf("\n");

        }
        y = 0;
    }
}

/* check if the domain is consistent or not, the domain is consistent
   as long as every key hash a domain length of at least 1. if a key
   has a domain of 0, then there is no possible value fo this space. */
int is_consistent(Domains *board_domains, const char *rows, int row_len, const char *cols, int col_len) {
    int x = 0, y = 0, hash;
    char space[2];

    for (; x < row_len; x++) {
        for(; y < col_len; y++) {

            space[0] = rows[x];
            space[1] = cols[y];

            hash = hash_code(space);

            /* check if the size of the list is 0. */
            if(board_domains -> values[hash] == NULL) {
                return 0;
            }
        }
        y = 0;
    }

    return 1;
}
/* hash_code function is simply since only needing to account for keys
   A1...I9. */
unsigned int hash_code(char *str) {
    unsigned int c = (int) str[0] - 64;

    c += (c * str[1]);
    return c - 50;
}

/* takes a string and creates a Linked List */
Node *linked_list_from_str(const char *str) {
    int index = 1;
    Node *curr, *prev, *head = malloc(sizeof(Node));
    head -> value = str[0];
    head -> next = NULL;
    prev = head;

    while( str[index] != '\0' ) {
        curr = malloc(sizeof(Node));
        curr -> value = str[index];
        curr -> next = NULL;
        prev -> next = curr;
        prev = prev -> next;
        index++;
    }
    return head;
}

/* get_unsolved_domain_keys returns a char** array of strings which are the unsolved
   keys of the board_domains. a key is unsolved if the domain list is more than one option. */
char **get_unsolved_domain_keys(Domains *board_domains, const char *rows, int row_len, const char *cols, int col_len) {
    int x = 0, y = 0, hash;
    char *space;
    int size = 0;
    char **key_list = NULL;

    for (; x < row_len; x++) {
        for (; y < col_len; y++) {

            space = malloc(3 * sizeof(char));
            space[0] = rows[x];
            space[1] = cols[y];
            space[2] = '\0';

            hash = hash_code(space);

            if (domain_list_len(board_domains -> values[hash]) > 1) {
                if (key_list == NULL) {
                    key_list = malloc(2 * sizeof(char *));
                    key_list[size] = space;
                    key_list[size + 1] = NULL;
                    size++;
                } else {
                    key_list = realloc(key_list, (size + 2)* sizeof(char *));
                    key_list[size] = space;
                    key_list[size + 1] = NULL;
                    size++;
                }

            } else {
                free(space);
                space = NULL;
            }

        }
        y = 0;
    }

    if (size == 0) {
        return NULL;
    }
    return key_list;
}

/* given a char** array of strings (space_options) for which keys to select from,
   get_min_list returns both the space string as well as the shortest domain_list
   found. If multiple domain_lists exist with the smallest length, the function will
   return the first one. Since we know these are unsolved keys, we know the shortest
   length will be 2, so we can have a small optimization by exiting once we find a list of size 2.*/
Space_List_Pair *get_min_list(Domains *board_domains, char **space_options) {
    int index = 0, hash, min_len = 10, curr_len;
    Space_List_Pair *pair = malloc(sizeof(Space_List_Pair));

    if (pair == NULL) {
        return NULL;
    }

    while(space_options[index] != NULL) {
        hash = hash_code(space_options[index]);
        curr_len = domain_list_len(board_domains -> values[hash]);

        if (curr_len < min_len) {
            min_len = curr_len;
            pair -> space = space_options[index];
            pair -> domain_list = board_domains -> values[hash];

            if (curr_len == 2) {
                return pair;
            }
        }
        index++;
    }
    return pair;
}

Domains *deep_copy_domains(Domains *board_domains, const char *rows, int row_len, const char *cols, int col_len) {
    int x = 0, y = 0, hash;
    char domain_key[3];
    Domains *new_domains = malloc(sizeof(Domains));

    domain_key[2] = '\0';

    for (; x < row_len; x++) {
        for (; y < col_len; y++) {

            domain_key[0] = rows[x];
            domain_key[1] = cols[y];

            hash = hash_code(domain_key);
            new_domains -> values[hash] = deep_copy_list(board_domains -> values[hash]);
        }
        y = 0;
    }
    return new_domains;
}

/* deep copy is necessary while making a new copy of the board domains.
   Each node must be made a deep copy of since if two separate copies
   of the board domains contained the same instance of Node's, their
   next member would not be retained across all copies. */
Node *deep_copy_list(Node *prev_list) {
    Node *temp = NULL;
    Node *new_curr = NULL;
    Node *new_head = malloc(sizeof(Node));
    new_head -> value = prev_list -> value;
    new_head -> next = NULL;

    temp = new_head;
    prev_list = prev_list -> next;


    while (prev_list != NULL) {
        new_curr = malloc(sizeof(Node));
        new_curr -> value = prev_list -> value;
        new_curr -> next = NULL;
        temp -> next = new_curr;
        temp = temp -> next;
        prev_list = prev_list -> next;
    }

    return new_head;
}

/* for the arc rule list, order doesn't matter so appending is
   done by adding a node to the front of the linked list.*/
Arc_List *append_arc_list(Arc_List *prev_list, char *value) {
    Arc_List *new_node = malloc(sizeof(Arc_List));
    new_node -> value = value;

    if (prev_list == NULL) {
        new_node -> next = NULL;
        return new_node;
    }
    new_node -> next = prev_list;
    return new_node;
}

/* remove the desired char from the Node linked list and return
   the new list. */
Node *remove_value_from_domain_list(Node *list, char val) {
    Node *prev, *curr = list;

    /* if val is the list's head value, then return the second node
      (or NULL if list was size 1) */
    if (curr -> value == val) {
        list = list -> next;
        curr -> next = NULL;
        free(curr);
        return list;
    }

    prev = curr;

    /* else find the correct node to remove and set the previous node's
       next pointer to node to remove's next. */
    while (curr != NULL) {

        if (curr -> value == val) {
            prev -> next = curr -> next;
            free(curr);
            return list;
        }
        prev = curr;
        curr = curr -> next;
    }

    return list;
}
/* free the Arc_List linked list for one value in Arcs hashmap*/
void free_arc_list(Arc_List *head) {
    Arc_List *temp = head;
    while (head != NULL) {
        head = head -> next;
        free(temp -> value);
        free(temp);
        temp = head;
    }
}

void free_keys(char ** keys) {
    int index = 0;
    while(keys[index] != NULL) {
        free(keys[index]);
        keys[index] = NULL;
        index++;
    }
}

void free_domain_keys(Domains *board_domains, const char *rows, int row_len, const char *cols, int col_len) {
    int x = 0, y = 0, hash;
    char space[2];
    Node *key_head;

    for (; x < row_len; x++) {
        for (; y < col_len; y++) {
            space[0] = rows[x];
            space[1] = cols[y];

            hash = hash_code(space);

            key_head = board_domains -> values[hash];

            free_domain_list(key_head);
            key_head = NULL;
            board_domains -> values[hash] = NULL;
        }
        y = 0;
    }
}
void free_domain_list(Node *head) {
    Node *temp = head;
    while(head != NULL) {
        head = head -> next;
        free(temp);
        temp = head;

    }
}