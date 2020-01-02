#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "driver.h"

int main(void) {
    int x = 0, y = 0;
    int input_index = 0;
    Board_Coordinates coords_mapping;
    Sudoku_Board start_board;
    Domains board_domains;
    
    initialize_squares();

    init_empty_board(&start_board);
    for (; input_index < ROWS_LEN; input_index++) {
        scanf("%s", start_board.rows[input_index]);
    }

    initialize_domains(&start_board, &board_domains, &coords_mapping);
    initialize_arcs();
    return VALID;
}

/* hash_code function is simply since only needing to account for keys
   A1...I9. */
unsigned int hash_code(char *str) {
    unsigned int c = (int) str[0] - 64;
  
    c += (c * str[1]);
    return c - 50;
}
/* Given a tile name (A1...I9) ad desired new value for the board,
   this function uses the coordinate mapping to retrieve the appropiate board (x, y) values
   to set the new value in to. */
void set_tile_value(Board_Coordinates *coord_mapping, char *tile, Sudoku_Board *board, char *new_value) {
    Coordinate *coords;
    int hash;

    hash = hash_code(tile);
    coords = coord_mapping -> coords[hash];
    board -> rows[coords -> row][coords -> col] = *new_value;
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

void print_board(Sudoku_Board *board) {
    int x = 0, y = 0;
    for (; x < ROWS_LEN; x++) {
        printf("|");
        for (; y < COL_LEN; y++) {
            printf("%c|", board -> rows[x][y]);
        }
        printf("\n");        
        y = 0;
    }
    printf("\n");
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

/* ====================START INIT SECTION =============================*/
/* ====================START INIT SECTION =============================*/
/* ====================START INIT SECTION =============================*/

/* load row is used to add the tile symbols in the squares board. 
   after this function is done the squares variable contains, see SQUARES in driver.h 
   for what the result will look like.  */
void load_row(char *squares_row[][9], int inner_index, char *letters, char *nums){
    int squares_index = 0, letters_index = 0, nums_index = 0, hash;
    char *str;

    for(; letters_index < 3; letters_index++) {

        for (; nums_index < 3; nums_index++) {
            str = malloc(3 * sizeof(char));
            str[0] = letters[letters_index];
            str[1] = nums[nums_index];
            str[2] = '\0';

            /* set the tile symbol for SQUARES */
            squares_row[0][squares_index] = str;

            squares_index++;
        }
        nums_index = 0;
    }
}
/* this function uses load row passing in the required combinations of
   strings used. this is used for the arc rule of 1-9 occuring once each in 
   each of the 9 sub squares in the sudoku board. */
void initialize_squares(){
    char *letters[] = {"ABC", "DEF", "GHI"};
    char *nums[] = {"123", "456", "789"};
    int letters_index = 0, nums_index = 0, letters_len = 3, nums_len = 3,
    squares_index = 0;
    char *current_letter, *current_num;
    
    for(;letters_index < letters_len; letters_index++) {
        current_letter = letters[letters_index];
        
        for(;nums_index < nums_len; nums_index++){
            current_num = nums[nums_index];
            load_row(&SQUARES[squares_index], squares_index, current_letter, current_num);
            squares_index++;
        }
        nums_index = 0;
    }
}
/* initializing the domains takes the current state of the sudoku board with its values
   and states what the possible values for the final solution can be. Using only the values
   and no arc rules this is simply 1, 2, 3,..., 9 when the space is empty or a size 1 list of 
   the value if the value is not empty. 
   This function also has the side effect of setting the coordinate hashmap values.
*/
void initialize_domains(Sudoku_Board *start_board, Domains *board_domains, Board_Coordinates *coords_mapping){
    Coordinate *coord;
    Node *domain_list;
    char start_value;
    int x = 0, y = 0, hash;
    char *space = malloc(3 * sizeof(char));

    for(;x < ROWS_LEN; x++){
        for(;y < COL_LEN; y++) {

            /* build space string*/
            space[0] = ROWS[x];
            space[1] = domains[y];
            space[2] = '\0';

            /* insert the tile into the board coordinates mapping */
            hash = hash_code(space);
            coord = malloc(sizeof(Coordinate));
            coord -> row = x;
            coord -> col = y;
            coords_mapping -> coords[hash] = coord;
    
            start_value = start_board -> rows[x][y];

            if (start_value != '0') {
                domain_list = malloc(sizeof(Node));
                domain_list -> value = start_value;
                domain_list -> next = NULL;
            } else {
                domain_list = linked_list_from_str(domains);
            }
            printf("tile: %s domain: ", space);
            print_domain_list(domain_list);
            printf("\n");
            board_domains -> values[hash] = domain_list;
        }
        y = 0;
    }   
}

/* initialize an empty board which consists of 9 rows and 9 columns 
   all containing the empty space indicator 0 */
void init_empty_board(Sudoku_Board *empty_board) {
    int index = 0, col_index = 0;
    empty_board -> rows = malloc(ROWS_LEN * sizeof(char *));

    for (; index < ROWS_LEN; index++) {
        empty_board -> rows[index] = malloc((1 + COL_LEN) * sizeof(char));
        for (; col_index < COL_LEN; col_index++) {
            empty_board -> rows[index][col_index] = EMPTY_SPACE;
        }
        empty_board -> rows[index][col_index] = '\0';
        col_index = 0;
    }
}

void initialize_arcs() {
    int x = 0, y = 0;
    char *space = malloc(3 * sizeof(char));

    for(;x < ROWS_LEN; x++){
        for(;y < COL_LEN; y++) {
            
            /* build space string*/
            space[0] = ROWS[x];
            space[1] = domains[y];
            space[2] = '\0';

            
        }
        y = 0;
    }    
}
/* ======================END INIT SECTION =============================*/
/* ======================END INIT SECTION =============================*/
/* ======================END INIT SECTION =============================*/

    // for (; x < ROWS_LEN; x++) {
    //     printf("|");
    //     for (; y < COL_LEN; y++) {
    //         printf("%c%c|", SQUARES[x][y][0], SQUARES[x][y][1]);
    //     }
    //     printf("\n");        
    //     y = 0;
    // }