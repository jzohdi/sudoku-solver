typedef struct tile_pair
{
    char *values[2];
    struct tile_pair *next;
    struct tile_pair *prev;
} Tile_Pair;

typedef struct
{
    int size;
    Tile_Pair *head;
    Tile_Pair *tail;
} Queue;

void init_queue(Queue *queue);
int append_queue(Queue *queue, char *v1, char *v2);
int queue_is_empty(Queue *queue);
Tile_Pair *create_pair(char *v1, char *v2);
Tile_Pair *shift_queue(Queue *queue);
int queue_size(Queue *queue);
void print_queue(Queue *queue);
