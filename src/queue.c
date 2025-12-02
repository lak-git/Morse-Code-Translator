#include "morse.h"
#include <stdbool.h>
#include <stdlib.h>


bool valid_QueueNode (Node qn)
{
    (void)qn;
    return true;
}

GENERATE_QUEUE(Node, size_t, NODE_QUEUE_INIT_SIZE, NODE_QUEUE_GROWTH_FACTOR, valid_QueueNode, malloc, realloc, free);