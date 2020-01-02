
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

#endif