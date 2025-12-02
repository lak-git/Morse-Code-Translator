#ifndef MORSE_H
#define MORSE_H

#include "stack.h"
#include <stddef.h>
#include "queue.h"


typedef struct BTreeNode
{
    char alnum_character; // Alphabetic character or \0
    struct BTreeNode* left; // dot (.)
    struct BTreeNode* right; // dash (-)
} BTreeNode;

typedef struct Node
{
    BTreeNode* node;
    char* morse_code;
} Node;

#define NODE_STACK_INIT_SIZE 16
#define NODE_STACK_GROWTH_FACTOR 2
DEFINE_STACK(Node, size_t, NODE_STACK_INIT_SIZE)

#define NODE_QUEUE_INIT_SIZE 16
#define NODE_QUEUE_GROWTH_FACTOR 2
DEFINE_QUEUE(Node, size_t, NODE_QUEUE_INIT_SIZE)


BTreeNode* morse_tree_init(void);
void morse_tree_delete(BTreeNode* root);
void morse_tree_insert(BTreeNode* root, const char* morse_code, char alpha_character);
void morse_tree_print(BTreeNode* root);
bool is_valid_morse_message(const char* message);
char* reverse_string(const char* string);
char decode_letter(BTreeNode* root, const char* morse_code);
char* encode_letter(BTreeNode* root, const char alnum_character);
char* morse_decode(BTreeNode* root, const char* morse_message);
char* morse_encode(BTreeNode* root, const char* text_message);


#endif // MORSE_H
