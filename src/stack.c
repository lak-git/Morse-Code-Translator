#include "morse.h"
#include <stdbool.h>
#include <stdlib.h>


bool valid_StackNode (StackNode s)
{
    (void)s;
    return true;
}

GENERATE_STACK(StackNode, size_t, STACKNODE_STACK_INIT_SIZE, STACKNODE_STACK_GROWTH_FACTOR, valid_StackNode, malloc, realloc, free);