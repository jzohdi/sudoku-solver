#include "hashmaps.h"

#if !defined DRIVERH
#define DRIVERH

#define VALID 1
#define ROWS_LEN 9
#define COL_LEN 9
#define DOMAIN_LEN 9
#define SQUARE_NUM 9
#define EMPTY_SPACE '0'

/* Iterating over rows/domains (both char) give the board layout:
   |A1|A2|A3|A4|A5|A6|A7|A8|A9|
   |B1|B2|B3|B4|B5|B6|B7|B8|B9|
   |C1|C2|C3|C4|C5|C6|C7|C8|C9|
   |D1|D2|D3|D4|D5|D6|D7|D8|D9|
   |E1|E2|E3|E4|E5|E6|E7|E8|E9|
   |F1|F2|F3|F4|F5|F6|F7|F8|F9|
   |G1|G2|G3|G4|G5|G6|G7|G8|G9|
   |H1|H2|H3|H4|H5|H6|H7|H8|H9|
   |I1|I2|I3|I4|I5|I6|I7|I8|I9|  */
const char ROWS[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', '\0'};
const int COLUMNS[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
const char domains[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '\0'};

/* SQUARES will hold the tile symbol names the groups of tiles in each
   of the 9 sub-squares make an arc. the groups that can only hold a 
   single instance of 1 - 9 in such:
   |A1|A2|A3|B1|B2|B3|C1|C2|C3|, |A4|A5|A6|B4|B5|B6|C4|C5|C6|, |A7|A8|A9|B7|B8|B9|C7|C8|C9|,
   |D1|D2|D3|E1|E2|E3|F1|F2|F3|, |D4|D5|D6|E4|E5|E6|F4|F5|F6|, |D7|D8|D9|E7|E8|E9|F7|F8|F9|,
   |G1|G2|G3|H1|H2|H3|I1|I2|I3|, |G4|G5|G6|H4|H5|H6|I4|I5|I6|, |G7|G8|G9|H7|H8|H9|I7|I8|I9|  */
char *SQUARES[9][9];

typedef struct board {
    char**rows;
} Sudoku_Board;

void load_row(char *squares_row[][9], int inner_index, char *letters, char *nums, Board_Coordinates *coords_mapping);
void initialize_squares(Board_Coordinates *coords_mapping);
void initialize_domains(Sudoku_Board *start_board, Domains *board_domains, Board_Coordinates *coords_mapping);
void init_empty_board(Sudoku_Board *empty_board);
void initialize_arcs(Arcs *arc_rules, Board_Coordinates *coords_mapping);
Arc_List *append_arc_list(Arc_List *new_list, char *value);
void AC3(Domains *board_domains, Arcs *arc_rules);
int revise_domains(Domains *board_domains, char *tile1, char *tile2);
Node * remove_value_from_domain_list(Node *list, char val);

void print_arc_list(Arc_List *head);
void print_domain_list(Node *head);
void print_board(Sudoku_Board *board);
void print_domains(Domains *board_domains);

#endif