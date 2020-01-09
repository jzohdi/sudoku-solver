
#if !defined HASHMAPSH
#define HASHMAPSH

#define MAX_SIZE 473

typedef struct node {
    char value;
    struct node *next;
} Node;

typedef struct {
    int size;
    Node *values[MAX_SIZE];
} Domains;

typedef struct {
    int squares_row;
    int row;
    int col;
} Coordinate;

/* the board coordinates will hold a mapping
of each square (A1, A2 ... I9) to its x,y coordinate
on the sudoku board */
typedef struct {
    Coordinate *coords[MAX_SIZE];
} Board_Coordinates;

typedef struct arc_ele {
    char *value;
    struct arc_ele *next;
} Arc_List;

typedef struct {
    Arc_List *values[MAX_SIZE];
} Arcs;

typedef struct {
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
Node *remove_value_from_domain_list(Node *list, char val);

#endif