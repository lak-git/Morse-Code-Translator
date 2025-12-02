#include <ctype.h>
#include "morse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


BTreeNode* morse_tree_init(void)
{
    BTreeNode* root = malloc(sizeof(BTreeNode));
    if (!root) { return NULL; }
    root->alnum_character = '\0';
    root->left = NULL;
    root->right = NULL;
    return root;
}

void morse_tree_delete(BTreeNode* root)
{
    if (!root) return;
    morse_tree_delete(root->left);
    morse_tree_delete(root->right);
    free(root);
}

// Insert an alphabetic character into the morse code tree
void morse_tree_insert(BTreeNode* root, const char* morse_code, char alnum_character)
{
    BTreeNode* current = root;
    for (size_t i = 0; i < strlen(morse_code); i++)
    {
        char ch = morse_code[i];
        if (ch == '.')
        {
            if (!current->left)
            {
                current->left = malloc(sizeof(BTreeNode));
                current->left->alnum_character = '\0';
                current->left->left = NULL;
                current->left->right = NULL;
            }
            current = current->left;
        } else if (ch == '-')
        {
            if (!current->right)
            {
                current->right = malloc(sizeof(BTreeNode));
                current->right->alnum_character = '\0';
                current->right->left = NULL;
                current->right->right = NULL;
            }
            current = current->right;
        }
    }
    current->alnum_character = alnum_character;
}

//Print morse code dictionary
void morse_tree_print(BTreeNode* root)
{
    if (!root) return;

    stack(Node) node_stack;
    stack_init(Node, &node_stack);

    char* empty = malloc(sizeof(char));
    if (!empty)
    {
        fprintf(stderr, "Memory allocation failed\n");
        stack_delete(Node, &node_stack);
        return;
    }
    empty[0] = '\0';
    Node initial = { .node = root, .morse_code = empty };

    bool not_succesfully_added = !stack_push(Node, &node_stack, initial);
    if (not_succesfully_added)
    {
        fprintf(stderr, "Stack push failed.\n");
        free(initial.morse_code);
        stack_delete(Node, &node_stack);
        return;
    }

    while (!stack_empty(Node, &node_stack))
    {
        Node current = stack_peek(Node, &node_stack);
        stack_pop(Node, &node_stack);

        BTreeNode* node = current.node;
        char* morse_code = current.morse_code;

        bool not_empty_character = node->alnum_character != '\0';
        if (not_empty_character)
        {
            printf("Character: %c, Morse Code: %s\n", node->alnum_character, morse_code);
        }

        // Push right child (dash) first so left is processed first
        if (node->right)
        {
            size_t len = strlen(morse_code);
            char* right_str = malloc(len + 2); // +1 for '-' and +1 for '\0'
            if (!right_str) free(morse_code); continue;
            MEMORY_COPY(right_str, morse_code, len);
            right_str[len] = '-';
            right_str[len + 1] = '\0';

            Node reverse_node = { .node = node->right, .morse_code = right_str };
            bool unsuccessful_push = !stack_push(Node, &node_stack, reverse_node);
            if (unsuccessful_push)
            {
                fprintf(stderr, "Stack push failed.\n");
                free(right_str);
            }
        }
        if (node->left)
        {
            size_t len = strlen(morse_code);
            char* left_str = malloc(len + 2); // +1 for '.' and +1 for '\0'
            if (!left_str) { free(morse_code); continue; }
            MEMORY_COPY(left_str, morse_code, len);
            left_str[len] = '.';
            left_str[len + 1] = '\0';

            Node dot_node = { .node = node->left, .morse_code = left_str };
            bool unsuccessful_push = !stack_push(Node, &node_stack, dot_node);
            if (unsuccessful_push)
            {
                fprintf(stderr, "Stack push failed.\n");
                free(left_str);
            }
        }
        free(morse_code);
    }
    stack_delete(Node, &node_stack);
}

bool is_valid_morse_message(const char* message)
{
    for (size_t i = 0; i < strlen(message); i++)
    {
        char ch = message[i];
        if (ch != '.' && ch != '-' && ch != ' ' && ch != '/') { return false; }
    }
    return true;
}

char decode_letter(BTreeNode* root, const char* morse_code)
{
    if (!root || !morse_code) { return '\0'; }
    BTreeNode* current = root;
    for (size_t i = 0; i < strlen(morse_code); i++)
    {
        char ch = morse_code[i];
        if (ch == '.')
        {
            if (!current->left) { return '\0'; }
            current = current->left;
        } else if (ch == '-')
        {
            if (!current->right) { return '\0'; }
            current = current->right;
        }
    }
    return current->alnum_character;
}

char* encode_letter(BTreeNode* root, const char alnum_character)
{
    if (!root || !isalnum(alnum_character)) { return NULL; }
    bool not_uppercase = !isupper(alnum_character);
    char ch = not_uppercase ? toupper(alnum_character) : alnum_character;

    queue(Node) node_queue;
    queue_init(Node, &node_queue);

    char* empty = malloc(sizeof(char));
    if (!empty)
    {
        fprintf(stderr, "Memory allocation failed\n");
        queue_delete(Node, &node_queue);
        return NULL;
    }
    empty[0] = '\0';
    Node initial = { .node = root, .morse_code = empty };

    bool unsuccessful_enqueue = !queue_enque(Node, &node_queue, initial);
    if (unsuccessful_enqueue)
    {
        fprintf(stderr, "Queue enqueue failed.\n");
        free(empty);
        queue_delete(Node, &node_queue);
        return NULL;
    }

    while (!queue_empty(Node, &node_queue))
    {
        Node current = queue_peek(Node, &node_queue);
        queue_deque(Node, &node_queue);

        BTreeNode* node = current.node;
        char* morse_code = current.morse_code;

        if (node->alnum_character == ch)
        {
            size_t len = strlen(morse_code);
            char* result = malloc(len + 1);
            if (!result)
            {
                free(morse_code);
                queue_delete(Node, &node_queue);
                return NULL;
            }
            MEMORY_COPY(result, morse_code, len);
            result[len] = '\0';
            free(morse_code);
            queue_delete(Node, &node_queue);
            return result;
        }
        if (node->left)
        {
            size_t len = strlen(morse_code);
            char* left_str = malloc(len + 2); // +1 for '.' +1 for '\0'
            if (left_str)
            {
                MEMORY_COPY(left_str, morse_code, len);
                left_str[len] = '.';
                left_str[len + 1] = '\0';
                Node left_node = { .node = node->left, .morse_code = left_str };
                bool unsuccessful_enqueue = !queue_enque(Node, &node_queue, left_node);
                if (unsuccessful_enqueue)
                {
                    fprintf(stderr, "Queue enqueue failed.\n");
                    free(left_str);
                }
            }
        }
        if (node->right)
        {
            size_t len = strlen(morse_code);
            char* right_str = malloc(len + 2); // +1 for '-' +1 for '\0'
            if (right_str)
            {
                MEMORY_COPY(right_str, morse_code, len);
                right_str[len] = '-';
                right_str[len + 1] = '\0';
                Node right_node = { .node = node->right, .morse_code = right_str };
                bool unsuccessful_enqueue = !queue_enque(Node, &node_queue, right_node);
                if (unsuccessful_enqueue)
                {
                    fprintf(stderr, "Queue enqueue failed.\n");
                    free(right_str);
                }
            }
        }
        free(morse_code);
    }
    queue_delete(Node, &node_queue);
    return NULL;
}

char* reverse_string(const char* string)
{
    size_t len = strlen(string);
    char* reversed = malloc(len + 1);
    if (!reversed) { return NULL; }
    for (size_t i = 0; i < len; i++) { reversed[i] = string[len - 1 - i]; }
    reversed[len] = '\0';
    return reversed;
}

// Morse Code to Alphabet
char* morse_decode(BTreeNode* root, const char* morse_message)
{
    if (!root || !morse_message) { return NULL; }
    size_t cap = 256;
    size_t len = 0;
    char* output = malloc(cap);
    if (!output)
    {
        fprintf(stderr, "Memory allocation failed\n"); 
        return NULL;
    }
    output[0] = '\0';

    const char* ptr = morse_message;
    bool first_word = true;
    while (*ptr)
    {
        const char* segment_end = ptr;
        while (*segment_end && *segment_end != '/') segment_end++;

        const char* segment_start = ptr;
        while (segment_start < segment_end && *segment_start == ' ') segment_start++;

        const char *segment_trim_end = segment_end;
        while (segment_trim_end > segment_start && segment_trim_end[-1] == ' ') segment_trim_end--;

        if (!first_word) {
            if (len + 1 >= cap) {
                cap *= 2;
                char* tmp = realloc(output, cap);
                if (!tmp) { free(output); return NULL; }
                output = tmp;
            }
            output[len++] = ' ';
            output[len] = '\0';
        }
        first_word = false;

        const char* word_ptr = segment_start;
        while (word_ptr < segment_trim_end) 
        {
            while (word_ptr < segment_trim_end && *word_ptr == ' ') word_ptr++;
            if (word_ptr >= segment_trim_end) break;
            const char* segment_ptr = word_ptr;
            while (segment_ptr < segment_trim_end && *segment_ptr != ' ') segment_ptr++;

            size_t token_len = (size_t)(segment_ptr - word_ptr);
            char* token = malloc(token_len + 1);
            if (!token) { free(output); return NULL; }
            MEMORY_COPY(token, word_ptr, token_len);
            token[token_len] = '\0';

            char decoded = decode_letter(root, token);
            free(token);

            if (decoded == '\0') decoded = '?'; /* avoid embedding NUL in output */

            if (len + 1 >= cap) {
                cap *= 2;
                char *tmp = realloc(output, cap);
                if (!tmp) { free(output); return NULL; }
                output = tmp;
            }
            output[len++] = decoded;
            output[len] = '\0';
            word_ptr = segment_ptr;
        }
        ptr = segment_end;
        if (*ptr == '/') ptr++;
    }
    if (len > 0 && output[len - 1] == ' ') output[len - 1] = '\0';
    return output;
}

// Alphabet to Morse Code
char* morse_encode(BTreeNode* root, const char* text_message)
{
    if (!root || !text_message) { return NULL; }

    size_t cap = 256;
    size_t len = 0;
    char* output = malloc(cap);
    if (!output)
    {
        fprintf(stderr, "Memory allocation failed\n"); 
        return NULL;
    }
    output[0] = '\0';

    for (size_t i = 0; i < strlen(text_message); i++)
    {
        char ch = text_message[i];
        if (ch == ' ')
        {
            if (len + 2 >= cap) {
                while (len + 2 >= cap) cap *= 2;
                char *tmp = realloc(output, cap);
                if (!tmp) { free(output); return NULL; }
                output = tmp;
            }
            output[len++] = '/';
            output[len++] = ' ';
            output[len] = '\0';
            continue;
        }
        if (!isalnum(ch)) { continue; } // ignore non-alphabetic characters; you could append '?' instead

        char uppercase_ch = toupper(ch);
        char* morse_code = encode_letter(root, uppercase_ch);
        if (!morse_code) {
            if (len + 1 >= cap) {
                cap *= 2;
                char* tmp = realloc(output, cap);
                if (!tmp) { free(output); return NULL; }
                output = tmp;
            }
            output[len++] = '?';
            output[len] = '\0';
            continue;
        }

        size_t morse_code_len = strlen(morse_code);
        if (len + morse_code_len + 1 >= cap) { // +1 for trailing space or terminator
            while (len + morse_code_len + 1 >= cap) cap *= 2;
            char *tmp = realloc(output, cap);
            if (!tmp) { free(morse_code); free(output); return NULL; }
            output = tmp;
        }

        MEMORY_COPY(output + len, morse_code, morse_code_len);
        len += morse_code_len;
        output[len++] = ' ';  // space between letters
        output[len] = '\0';

        free(morse_code);
    }
    // Remove trailing space if exists
    if (len > 0 && output[len - 1] == ' ') output[len - 1] = '\0';
    return output;
}
