#define _GNU_SOURCE
#include "morse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>


static const char* MORSE_CODE_SEQUENCE[] = {
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",
    ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",
    "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", ".----",
    "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----.", "-----"
};

static const char ALPHABET[] = {
    'A','B','C','D','E','F','G','H','I','J','K','L','M',
    'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
    '0','1','2','3','4','5','6','7','8','9'
};

static const size_t ALPHABET_SIZE = sizeof(ALPHABET) / sizeof(ALPHABET[0]);

void populate_morse_tree(BTreeNode* root);
char* read_file(const char* filename);


int main(int argc, char const *argv[])
{
    BTreeNode* root = morse_tree_init();
    if (!root)
    {
        fprintf(stderr, "Memory error\n");
        return 1; 
    }
    populate_morse_tree(root);

    printf("Morse Code Dictionary:\n");
    morse_tree_print(root);

    char* input_message = NULL;
    int mode = -1; // 1: Morse->Alnum, 2: Alnum->Morse */

    if (argc > 1) {
        input_message = read_file(argv[1]);
        if (!input_message) {
            fprintf(stderr, "Failed to read file: %s\n", argv[1]);
            morse_tree_delete(root);
            return 1;
        }
        mode = 1;
    } else {
        /* interactive prompt for mode */
        printf("\nChoose mode:\n1) Morse code -> Alphabetical\n2) Alphabetical -> Morse code\nEnter 1 or 2: ");
        if (scanf("%d%*c", &mode) != 1) {
            fprintf(stderr, "Invalid input\n");
            morse_tree_delete(root);
            return 1;
        }
        if (mode == 1) {
            printf("Enter Morse message (letters separated by spaces, words by /):\n");
            size_t cap = 0;
            ssize_t linelen = getline(&input_message, &cap, stdin);
            if (linelen <= 0) input_message = NULL;
            if (input_message) {
                /* trim newline */
                size_t n = strlen(input_message);
                while (n > 0 && (input_message[n-1] == '\n' || input_message[n-1] == '\r')) {
                    input_message[--n] = '\0';
                }
            }
        } else if (mode == 2) {
            printf("Enter alphabetical text to convert (letters & spaces):\n");
            size_t cap = 0;
            ssize_t linelen = getline(&input_message, &cap, stdin);
            if (linelen <= 0) input_message = NULL;
            if (input_message) {
                size_t n = strlen(input_message);
                while (n > 0 && (input_message[n-1] == '\n' || input_message[n-1] == '\r')) {
                    input_message[--n] = '\0';
                }
            }
        } else {
            fprintf(stderr, "Unknown mode\n");
            morse_tree_delete(root);
            return 1;
        }
    }

    if (mode == 1) {
        if (!input_message) {
            fprintf(stderr, "No Morse message provided\n");
            morse_tree_delete(root);
            return 1;
        }
        if (!is_valid_morse_message(input_message)) {
            fprintf(stderr, "Error: The Morse code message contains invalid characters.\n");
            free(input_message);
            morse_tree_delete(root);
            return 1;
        }

        printf("\nOriginal Morse Code: %s\n", input_message);

        char* reverse_str = reverse_string(input_message);
        if (!reverse_str)
        { 
            fprintf(stderr, "Memory error\n");
            free(input_message);
            morse_tree_delete(root);
            return 1;
        }
        printf("Reversed Morse Code: %s\n", reverse_str);

        char* decoded = morse_decode(root, reverse_str);
        printf("Decoded Message: %s\n", decoded ? decoded : "(null)\n");

        free(decoded);
        free(reverse_str);
        free(input_message);
    } else if (mode == 2) {
        // Alphabetical -> Morse */
        if (!input_message) {
            fprintf(stderr, "No text provided\n");
            morse_tree_delete(root);
            return 1;
        }
        char* morse = morse_encode(root, input_message);
        if (!morse) {
            fprintf(stderr, "Conversion failed\n");
            free(input_message);
            morse_tree_delete(root);
            return 1;
        }
        printf("\nAlphabetical input: %s\n", input_message);
        printf("Converted Morse: %s\n", morse);

        free(morse);
        free(input_message);
    }
    morse_tree_delete(root);
    return 0;
}


void populate_morse_tree(BTreeNode* root)
{
    for (size_t i = 0; i < ALPHABET_SIZE; i++)
    {
        morse_tree_insert(root, MORSE_CODE_SEQUENCE[i], ALPHABET[i]);
    }
}

char* read_file(const char* filename)
{
    FILE* file = fopen(filename, "rb");
    if (!file) return NULL;
    if (fseek(file, 0, SEEK_END) != 0) { fclose(file); return NULL; }

    long file_size = ftell(file);
    if (file_size < 0) { fclose(file); return NULL; }
    rewind(file);

    char* buffer = malloc((size_t)file_size + 1);
    if (!buffer) { fclose(file); return NULL; }
    size_t got = fread(buffer, 1, (size_t)file_size, file);
    fclose(file);
    buffer[got] = '\0';
    /* trim trailing newlines */
    while (got > 0 && (buffer[got-1] == '\n' || buffer[got-1] == '\r')) { buffer[--got] = '\0'; }
    return buffer;
}
