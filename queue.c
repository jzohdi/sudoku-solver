#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

Tile_Pair *create_pair(char *v1, char *v2)
{
    Tile_Pair *new_pair = malloc(sizeof(Tile_Pair));
    if (new_pair == NULL)
    {
        return NULL;
    }
    /* make deep copies of each string. */
    new_pair->values[0] = malloc(3 * sizeof(char));

    if (new_pair->values[0] == NULL)
    {
        return NULL;
    }
    new_pair->values[0][0] = v1[0];
    new_pair->values[0][1] = v1[1];
    new_pair->values[0][2] = v1[2];

    new_pair->values[1] = malloc(3 * sizeof(char));

    if (new_pair->values[1] == NULL)
    {
        free(new_pair->values[0]);
        return NULL;
    }
    new_pair->values[1][0] = v2[0];
    new_pair->values[1][1] = v2[1];
    new_pair->values[1][2] = v2[2];

    new_pair->next = NULL;
    new_pair->prev = NULL;

    return new_pair;
}

/* adding to the queue consists of creating a new Tile_Pair
   and appending it to the last position of the queue. the new Tile_pair
   will have the current tail as its previous, its' next will be NULL and
   the tail will now be the new tail. */
int append_queue(Queue *queue, char *v1, char *v2)
{

    Tile_Pair *new_pair = create_pair(v1, v2);

    if (new_pair == NULL)
    {
        return 0;
    }

    queue->size++;

    /* handle case where queue was empty.*/
    if (queue->tail == NULL && queue->head == NULL)
    {
        queue->tail = new_pair;
        queue->head = new_pair;
        return 1;
    }

    new_pair->prev = queue->tail;
    queue->tail->next = new_pair;
    queue->tail = new_pair;

    return 1;
}

Tile_Pair *shift_queue(Queue *queue)
{
    Tile_Pair *return_pair;

    if (queue_is_empty(queue))
    {
        return NULL;
    }

    if (queue->size == 1)
    {
        return_pair = queue->head;
        queue->head = NULL;
        queue->tail = NULL;
    }
    else
    {
        return_pair = queue->head;
        queue->head = queue->head->next;
        queue->head->prev = NULL;
        return_pair->next = NULL;
    }

    queue->size--;

    return return_pair;
}

void init_queue(Queue *queue)
{
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
}

int queue_is_empty(Queue *queue)
{
    return ((queue->head == NULL) && (queue->tail == NULL));
}

int queue_size(Queue *queue)
{
    int size = 0;
    Tile_Pair *head = queue->head;
    while (head != NULL)
    {
        size++;
        head = head->next;
    }
    return size;
}
void print_queue(Queue *queue)
{
    Tile_Pair *curr;

    if (queue == NULL)
    {
        return;
    }
    curr = queue->head;

    if (curr == NULL)
    {
        printf("[]");
        return;
    }

    printf("[");

    while (curr != NULL)
    {
        printf("(%s, %s)", curr->values[0], curr->values[1]);
        if (curr->next != NULL)
        {
            printf(", ");
        }
        curr = curr->next;
    }

    printf("]\n");
}
