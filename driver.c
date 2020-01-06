#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "driver.h"
#include "queue.h"

int main(void) {
    int x = 0, y = 0;
    int input_index = 0;
    Board_Coordinates coords_mapping;
    Sudoku_Board start_board;
    Domains board_domains;
    Arcs arc_rules;
    
    initialize_squares(&coords_mapping);

    init_empty_board(&start_board);
    for (; input_index < ROWS_LEN; input_index++) {
        scanf("%s", start_board.rows[input_index]);
    }

    initialize_domains(&start_board, &board_domains, &coords_mapping);
    initialize_arcs(&arc_rules, &coords_mapping);

    AC3(&board_domains, &arc_rules);
    print_domains(&board_domains);
    return VALID;
}
/*=======================================================================*/ 
/*===================== START HELPER FUNCTIONS ==========================*/
/*=======================================================================*/

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

void print_domains(Domains *board_domains) {
    char *space = malloc(3 * sizeof(char));
    int x = 0, y = 0, hash;

    for(;x < ROWS_LEN; x++){
        for(;y < COL_LEN; y++) {

            space[0] = ROWS[x];
            space[1] = domains[y];
            space[2] = '\0';

            /* insert the tile into the board coordinates mapping */
            hash = hash_code(space);
            printf("%s: ", space);
            print_domain_list(board_domains -> values[hash]);
            printf("\n");

        }
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
/*=======================================================================*/ 
/*======================= END HELPER FUNCTIONS ==========================*/
/*=======================================================================*/


/* ====================START INIT SECTION =============================*/
/* ====================START INIT SECTION =============================*/
/* ====================START INIT SECTION =============================*/

/* load row is used to add the tile symbols in the squares board. 
   after this function is done the squares variable contains, see SQUARES in driver.h 
   for what the result will look like.  */
void load_row(char *squares_row[][9], int inner_index, char *letters, char *nums, Board_Coordinates *coords_mapping){
    int squares_index = 0, letters_index = 0, nums_index = 0, hash;
    Coordinate *coord;
    char *str;

    for(; letters_index < 3; letters_index++) {

        for (; nums_index < 3; nums_index++) {
            str = malloc(3 * sizeof(char));
            str[0] = letters[letters_index];
            str[1] = nums[nums_index];
            str[2] = '\0';

            hash = hash_code(str);
            coord = malloc(sizeof(Coordinate));
            coord -> squares_row = inner_index;
            coords_mapping -> coords[hash] = coord;
            
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
void initialize_squares(Board_Coordinates *coords_mapping){
    char *letters[] = {"ABC", "DEF", "GHI"};
    char *nums[] = {"123", "456", "789"};
    int letters_index = 0, nums_index = 0, letters_len = 3, nums_len = 3,
    squares_index = 0;
    char *current_letter, *current_num;
    
    for(;letters_index < letters_len; letters_index++) {
        current_letter = letters[letters_index];
        
        for(;nums_index < nums_len; nums_index++){
            current_num = nums[nums_index];
            load_row(&SQUARES[squares_index], squares_index, current_letter, current_num, coords_mapping);
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
            coords_mapping -> coords[hash] -> row = x;
            coords_mapping -> coords[hash] -> row = x;
            start_value = start_board -> rows[x][y];

            if (start_value != '0') {
                domain_list = malloc(sizeof(Node));
                domain_list -> value = start_value;
                domain_list -> next = NULL;
            } else {
                domain_list = linked_list_from_str(domains);
            }

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

/* arcs are the rules that apply to each tile for which other tiles cannot have the same
   digit. The rules will be held in the Arcs arc_rules hashmap. 
   for example arc rules for "E1":
    "E2", "E3", "E4", "E5", "E6", "E7*, "E8", "E9", 
    "A1", "B1", "C1", "D1", "F1", "G1", "H1", "I1",
    "E9", "E8", "E7", "E6", "E5", "E4", "E3", "E2".
    Meaning, if there is a 6 in the E1 tile, 6 cannot be in any of the listed tiles.
*/
void initialize_arcs(Arcs *arc_rules, Board_Coordinates *coords_mapping) {
    int x = 0, y = 0, hash, rule_index = 0, squares_row;
    char *space = malloc(3 * sizeof(char));
    char arc_char;
    Arc_List *new_list;
    char *new_value;

    for(;x < ROWS_LEN; x++){
        for(;y < COL_LEN; y++) {
            
            /* build space string*/
            space[0] = ROWS[x];
            space[1] = domains[y];
            space[2] = '\0';

            hash = hash_code(space);
            
            new_list = NULL;

            /* add all arc rules for being in the same row */
            for(; rule_index < COL_LEN; rule_index++) {
                arc_char = domains[rule_index];
                
                if (arc_char != space[1]) {
                    new_value = malloc(3 * sizeof(char));
                    new_value[0] = space[0];
                    new_value[1] = arc_char;
                    new_value[2] = '\0';

                    new_list = append_arc_list(new_list, new_value);    
                }
            }
            rule_index = 0;

            /* add all arc rule for being in the same column. */
            for(; rule_index < ROWS_LEN; rule_index++) {
                arc_char = ROWS[rule_index];

                if (arc_char != space[0]) {
                    new_value = malloc(3 * sizeof(char));
                    new_value[0] = arc_char;
                    new_value[1] = space[1];
                    new_value[2] = '\0';

                    new_list = append_arc_list(new_list, new_value);
                }
            } 
            rule_index = 0;

            /* add all rules for being in the same sub square. */
            squares_row = coords_mapping -> coords[hash] -> squares_row;
    
            for(; rule_index < SQUARE_NUM; rule_index++) {
                if(strcmp(SQUARES[squares_row][rule_index], space) != 0) {
                    new_list = append_arc_list(new_list, SQUARES[squares_row][rule_index]);
                }
            }
            rule_index = 0;

            arc_rules -> values[hash] = new_list;            
        }
        y = 0;
    }    
}

/* ======================END INIT SECTION =============================*/
/* ======================END INIT SECTION =============================*/
/* ======================END INIT SECTION =============================*/

/*=======================================================================*/ 
/*===================== START SOLVER FUNCTIONS ==========================*/
/*=======================================================================*/

void AC3(Domains *board_domains, Arcs *arc_rules) {
    char *space, *arc_tile;
    int x = 0, y = 0, hash;
    Queue queue;
    Arc_List *curr;
    Tile_Pair *pair;

    init_queue(&queue);

    /* build queueue with every combination of pairs of arc rules. */
    for(;x < ROWS_LEN; x++){
        for(;y < COL_LEN; y++) {

            /* build space string*/
            space = malloc(3 * sizeof(char));
            space[0] = ROWS[x];
            space[1] = domains[y];
            space[2] = '\0';

            hash = hash_code(space);

            curr = arc_rules -> values[hash];
            
            while (curr != NULL) {
                arc_tile = curr -> value;

                append_queue(&queue, space, arc_tile);

                curr = curr -> next;
            }
        }
        y = 0;
    }

    while(!queue_is_empty(&queue)) {

        pair = shift_queue(&queue);

        if (revise_domains(board_domains, pair -> values[0], pair -> values[1])) {
            
            hash = hash_code(pair -> values[0]);
            curr = arc_rules -> values[hash];

            while (curr != NULL) {
                append_queue(&queue, curr -> value, pair -> values[0]);
                curr = curr -> next;
            }
        }
    
    }

}

int revise_domains(Domains *board_domains, char *tile1, char *tile2) {
    int revised = 0, possible = 0, hash1;
    Node *head1, *curr1, *curr2;
    
    hash1 = hash_code(tile1);
    head1 = curr1 = board_domains -> values[hash1];
    
    while(curr1 != NULL) {

        possible = 0;
        curr2 = board_domains -> values[hash_code(tile2)];
        
        while(curr2 != NULL) {
            if (curr1 -> value != curr2 -> value) {
                possible = 1;
            } 
            curr2 = curr2 -> next;
        }
        
        if (!possible) {

            board_domains -> values[hash1] = remove_value_from_domain_list(head1, curr1 -> value);
            revised = 1;
        }
        
        curr1 = curr1 -> next;
    }

    return revised;
}

/* remove the desired char from the Node linked list and return
   the new list. */
Node * remove_value_from_domain_list(Node *list, char val) {
    Node *prev, *temp, *curr = list;
    
    if (curr -> value == val) {
        list = list -> next;
        curr -> next = NULL;
        free(curr);
        return list;
    }

    prev = curr;
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
/*=======================================================================*/ 
/*======================= END SOLVER FUNCTIONS ==========================*/
/*=======================================================================*/