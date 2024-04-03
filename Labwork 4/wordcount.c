#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 100
#define INITIAL_DATA_SIZE 100
#define MAX_WORD_SIZE 100

typedef struct {
    char word[MAX_WORD_LENGTH];
    int count;
} WordCount;

void map(const char *line, WordCount **intermediate_data, int *data_size);
void reduce(WordCount *intermediate_data, int size);
void resize_data(WordCount **intermediate_data, int *data_size, int new_size);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *input_file = fopen(argv[1], "r");
    if (!input_file) {
        printf("Error opening input file.\n");
        return 1;
    }

        WordCount *intermediate_data = malloc(INITIAL_DATA_SIZE * sizeof(WordCount));
    int data_size = 0;

    char line[256];
    while (fgets(line, sizeof(line), input_file)) {
        map(line, &intermediate_data, &data_size);
    }

    fclose(input_file);

    reduce(intermediate_data, data_size);

    free(intermediate_data);     return 0;
}

void map(const char *line, WordCount **intermediate_data, int *data_size) {
    char word[MAX_WORD_SIZE];
    char *current_word = word;

        for (int i = 0; line[i] != '\0'; i++) {
        char ch = line[i];

                if (!isalnum(ch)) {
            if (current_word > word) {
                *current_word = '\0'; 
                                int found = 0;
                for (int j = 0; j < *data_size; j++) {
                    if (strcmp((*intermediate_data)[j].word, word) == 0) {
                        (*intermediate_data)[j].count++;
                        found = 1;
                        break;
                    }
                }

                if (!found) {
					if (*data_size == INITIAL_DATA_SIZE) {                       	
						resize_data(intermediate_data, data_size, *data_size * 2);
                    }

                    strcpy((*intermediate_data)[*data_size].word, word);
                    (*intermediate_data)[*data_size].count = 1;
                    (*data_size)++;
                }

                current_word = word;             }
        } else {
            *current_word++ = tolower(ch);         }
    }

        if (current_word > word) {
        *current_word = '\0';
        int found = 0;
        for (int j = 0; j < *data_size; j++) {
            if (strcmp((*intermediate_data)[j].word, word) == 0) {
                (*intermediate_data)[j].count++;
                found = 1;
                break;
            }
        }
        if (!found) {
            if (*data_size == INITIAL_DATA_SIZE) {               	
				resize_data(intermediate_data, data_size, *data_size * 2);
            }

            strcpy((*intermediate_data)[*data_size].word, word);
            (*intermediate_data)[*data_size].count = 1;
            (*data_size)++;
        }
    }
}

void resize_data(WordCount **intermediate_data, int *data_size, int new_size) {
    *intermediate_data = realloc(*intermediate_data, new_size * sizeof(WordCount));
    *data_size = new_size;
}

void reduce(WordCount *intermediate_data, int size) {
        for (int i = 0; i < size - 1; i++) {
        for (int j = i + 1; j < size; j++) {
            if (strcmp(intermediate_data[i].word, intermediate_data[j].word) > 0) {
                WordCount temp = intermediate_data[i];
                intermediate_data[i] = intermediate_data[j];
                intermediate_data[j] = temp;
            }
        }
    }

        for (int i = 0; i < size; i++) {
        printf("%s: %d\n", intermediate_data[i].word, intermediate_data[i].count);
    }
}

