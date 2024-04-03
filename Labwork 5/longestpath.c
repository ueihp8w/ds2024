#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_PATH_LENGTH 256

typedef struct {
  char path[MAX_PATH_LENGTH];
  size_t length; 
} PathComponent;

typedef struct {
  PathComponent *entries[1024]; 
  bool occupied[1024];
  size_t size;
} HashTable;

size_t hash(const char *key) {
  size_t hash_value = 0;
  for (int i = 0; key[i] != '\0'; i++) {
    hash_value = (hash_value * 31 + key[i]) % 1024;
  }
  return hash_value;
}

void init_hash_table(HashTable *table) {
  for (int i = 0; i < 1024; i++) {
    table->entries[i] = NULL;
    table->occupied[i] = false;
  }
  table->size = 0;
}

void insert_hash_table(HashTable *table, const char *parent_path,
                       const char *component) {
  size_t key_index = hash(parent_path);
  int i = 0;

  while (table->occupied[key_index] && (table->entries[key_index] == NULL ||
                                         strcmp(table->entries[key_index]->path, parent_path) != 0)) {
    key_index = (key_index + 1) % 1024;
    i++;
    if (i >= 1024) { 
      fprintf(stderr, "Hash table overflow\n");
      return;
    }
  }

  PathComponent *new_entry = malloc(sizeof(PathComponent));
  if (new_entry == NULL) {
    fprintf(stderr, "Memory allocation error\n");
    return;
  }

  snprintf(new_entry->path, MAX_PATH_LENGTH, "%s/%s", parent_path, component);
  new_entry->length = strlen(new_entry->path);
  table->entries[key_index] = new_entry;
  table->occupied[key_index] = true;
  table->size++;
}

PathComponent *find_hash_table(HashTable *table, const char *parent_path) {
  size_t key_index = hash(parent_path);
  int i = 0;

  while (table->occupied[key_index] && (table->entries[key_index] == NULL ||
                                         strcmp(table->entries[key_index]->path, parent_path) != 0)) {
    key_index = (key_index + 1) % 1024;
    i++;
    if (i >= 1024) {
      return NULL; 
    }
  }

  return table->entries[key_index];
}

void split_path(const char *path, char components[][MAX_PATH_LENGTH], int *num_components) {
  int i = 0, j = 0, curr_component = 0;
  while (path[i] != '\0') {
    if (path[i] == '/') {
      components[curr_component][j] = '\0';
      curr_component++;
      j = 0;
    } else {
      components[curr_component][j++] = path[i];
    }
    i++;
  }
  components[curr_component][j] = '\0'; 
  *num_components = curr_component + 1;
}

void map(const char *line, HashTable *table) {
  char *path = strdup(line);
  char components[MAX_PATH_LENGTH][MAX_PATH_LENGTH];
  int num_components = 0;
  split_path(path, components, &num_components);

  for (int i = 0; i < num_components; i++) {
    char parent_path[MAX_PATH_LENGTH];
    if (i == 0) {
      snprintf(parent_path, MAX_PATH_LENGTH, "/");
    } else {
      snprintf(parent_path, MAX_PATH_LENGTH, "%s", components[i - 1]);
    }
    insert_hash_table(table, parent_path, components[i]);
  }

  free(path);
}

void reduce(HashTable *table, const char *parent_path) {
  PathComponent *entry = find_hash_table(table, parent_path);
  if (entry == NULL) {
    return; 
  }

  size_t max_length = 0;
  PathComponent *longest_path = NULL;

  for (int i = 0; i < table->size; i++) {
    if (table->entries[i] != NULL &&
        strncmp(table->entries[i]->path, parent_path, strlen(parent_path)) == 0) {
      if (table->entries[i]->length > max_length) {
        max_length = table->entries[i]->length;
        longest_path = table->entries[i];
      }
    }
  }

  if (longest_path != NULL) {
    printf("Longest path under %s: %s\n", parent_path, longest_path->path);
  }
}

void free_hash_table(HashTable *table) {
  for (int i = 0; i < 1024; i++) {
    if (table->occupied[i]) {
      free(table->entries[i]);  
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: %s <input_file1> [<input_file2> ...]\n", argv[0]);
    return 1;
  }

  HashTable table;
  init_hash_table(&table);

  for (int i = 1; i < argc; i++) {
    FILE *file = fopen(argv[i], "r");
    if (!file) {
      fprintf(stderr, "Error opening file: %s\n", argv[i]);
      continue;
    }

    char line[MAX_PATH_LENGTH];
    while (fgets(line, sizeof(line), file)) {
      line[strcspn(line, "\n")] = '\0';

      char components[MAX_PATH_LENGTH][MAX_PATH_LENGTH];
      int num_components;
      split_path(line, components, &num_components);

      for (int j = 1; j < num_components; j++) {
        char parent_path[MAX_PATH_LENGTH];
        if (j == 1) {
          strcpy(parent_path, "/"); 
        } else {
          snprintf(parent_path, MAX_PATH_LENGTH, "%s", components[j - 1]);
        }
        map(components[j], &table);
      }
    }

    fclose(file);
  }

  for (int i = 0; i < table.size; i++) {
    if (table.entries[i] != NULL) {
      reduce(&table, table.entries[i]->path);
    }
  }

  free_hash_table(&table);
  return 0;
}
