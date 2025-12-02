#ifndef MORSE_H
#define MORSE_H

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


#endif MORSE_H