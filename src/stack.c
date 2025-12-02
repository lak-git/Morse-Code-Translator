#include "morse.h"
#include <stdbool.h>
#include <stdlib.h>


bool valid_StackNode (Node sn)
{
    (void)sn;
    return true;
}

GENERATE_STACK(Node, size_t, NODE_STACK_INIT_SIZE, NODE_STACK_GROWTH_FACTOR, valid_StackNode, malloc, realloc, free);