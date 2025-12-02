#ifndef MORSE_H
#define MORSE_H

#include "stack.h"
#include <stddef.h>


typedef struct Node
{
    char* alpha_character; // Alphabetic character or \0
    struct Node* left;
    struct Node* right;
} Node;

typedef struct StackNode
{
    Node* node;
    char* morse_code;
} StackNode;

#define STACKNODE_STACK_INIT_SIZE 16
#define STACKNODE_STACK_GROWTH_FACTOR 2
DEFINE_STACK(StackNode, size_t, STACKNODE_STACK_INIT_SIZE);


#endif MORSE_H