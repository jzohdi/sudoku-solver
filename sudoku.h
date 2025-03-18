#if !defined DRIVERH
#define DRIVERH

#include "hashmaps.h"

#define VALID 0
#define INVALID 1
#define ROWS_LEN 9
#define COL_LEN 9
#define DOMAIN_LEN 9
#define SQUARE_NUM 9
#define EMPTY_SPACE '0'
#define NO_MODE 0
#define TRMNL_MODE 1
#define FILE_MODE 2
#define LINE_MODE 3
#define HELP_MODE 4
#define AC3_ 1
#define AC3_ONLY 2

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
const char COLUMNS[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '\0'};

/* SQUARES will hold the tile symbol names the groups of tiles in each
   of the 9 sub-squares make an arc. the groups that can only hold a
   single instance of 1 - 9 in such:
   |A1|A2|A3|B1|B2|B3|C1|C2|C3|, |A4|A5|A6|B4|B5|B6|C4|C5|C6|, |A7|A8|A9|B7|B8|B9|C7|C8|C9|,
   |D1|D2|D3|E1|E2|E3|F1|F2|F3|, |D4|D5|D6|E4|E5|E6|F4|F5|F6|, |D7|D8|D9|E7|E8|E9|F7|F8|F9|,
   |G1|G2|G3|H1|H2|H3|I1|I2|I3|, |G4|G5|G6|H4|H5|H6|I4|I5|I6|, |G7|G8|G9|H7|H8|H9|I7|I8|I9|  */
char *SQUARES[9][9];

typedef struct
{
    int ac3;
    int input_mode;
    int write_out;
    char *file_name;
} Command_Line_Args;

typedef struct board
{
    Command_Line_Args *cmd;
    char rows[9][9];
} Sudoku_Board;

void print_commands(void);
void print_board(char rows[9][9]);
void set_args(Sudoku_Board *board, char *argv[], int argc);
int str_contains_char(char s[], char c);
int is_number(char s[]);
int is_digit(char c);
int str_len(char s[]);
int str_equal(char s1[], char s2[]);
void load_row(char *squares_row[][9], int inner_index, char *letters, char *nums, Squares_Row *sq_row);
void initialize_squares(Squares_Row *sq_row);
void initialize_domains(Sudoku_Board *start_board, Domains *board_domains, Squares_Row *sq_row);
void init_empty_board(Sudoku_Board *empty_board);
void initialize_arcs(Arcs *arc_rules, Squares_Row *sq_row);
Arc_List *append_arc_list(Arc_List *new_list, char *value);
void AC3(Domains *board_domains, Arcs *arc_rules);
Domains *backtracking_search(Domains *board_domains, Arcs *arc_rules);
Domains *get_new_domains(Domains *board_domains, char *space, char new_val, Arcs *arc_rules);
int revise_domains(Domains *board_domains, char *tile1, char *tile2);

/* functions to free memory. */
void free_squares(char *squares[9][9]);
void free_arcs(Arcs *arc_rules);

#endif
