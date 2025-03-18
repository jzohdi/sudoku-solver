
#if !defined HASHMAPSH
#define HASHMAPSH

#define MAX_SIZE 473

typedef struct node
{
    char value;
    struct node *next;
} Node;

typedef struct
{
    int size;
    Node *values[MAX_SIZE];
} Domains;

/* the Squares_row will hold a mapping
of each square (A1, A2 ... I9) row number to cache.
This is done, since otherwise the last step of initialize_arcs
would have to do an n^2 search for each tile in order to find the
correct location inside the SQUARES rule. */
typedef struct
{
    int row[MAX_SIZE];
} Squares_Row;

typedef struct arc_ele
{
    char *value;
    struct arc_ele *next;
} Arc_List;

typedef struct
{
    Arc_List *values[MAX_SIZE];
} Arcs;

typedef struct
{
    char *space;
    Node *domain_list;
} Space_List_Pair;

int domain_list_len(Node *domain_list);
void print_domain_list(Node *head);
void print_arc_list(Arc_List *head);
void print_domains(Domains *board_domains, const char *rows, int row_len, const char *cols, int col_len);
int is_consistent(Domains *board_domains, const char *rows, int row_len, const char *cols, int col_len);
unsigned int hash_code(char *str);
Node *linked_list_from_str(const char *str);
char **get_unsolved_domain_keys(Domains *board_domains, const char *rows, int row_len, const char *cols, int col_len);
Space_List_Pair *get_min_list(Domains *board_domains, char **space_options);
Node *deep_copy_list(Node *prev_list);
Arc_List *append_arc_list(Arc_List *prev_list, char *value);
int board_is_solved(Domains *board_domains, const char *rows, int row_len, const char *cols, int col_len);
void print_solved_domains(Domains *board_domains, const char *rows, int row_len, const char *cols, int col_len);
void print_unsolved_domains(Domains *board_domains, const char *rows, int row_len, const char *cols, int col_len);
Node *remove_value_from_domain_list(Node *list, char val);
Domains *deep_copy_domains(Domains *board_domains, const char *rows, int row_len, const char *cols, int col_len);

void free_arc_list(Arc_List *head);
void free_keys(char **keys);
void free_domain_keys(Domains *board_domains, const char *rows, int row_len, const char *cols, int col_len);
void free_domain_list(Node *head);

#endif
