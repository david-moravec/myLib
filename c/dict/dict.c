#include <string.h>

#include "../list/list.h"
#include "dict.h"

LIST_FUNCS_DEFINE(Entry, struct Entry);

static uint32_t hashString(const char *key, int length) {
  uint32_t hash = 2166136261u;
  for (int i = 0; i < length; i++) {
    hash ^= (uint8_t)key[i];
    hash *= 16777619;
  }
  return hash;
}

static struct Entry *find_entry(struct list_Entry *entries, size_t capacity,
                                uint32_t hash) {
  if (capacity == 0) {
    return NULL;
  }

  int index = hash % capacity;

  for (;;) {
    struct Entry *entry = &entries->list.data[index];

    if (entry->hash == -1) {
      if (entry->value == NULL) {
        return entry;
      }
    } else if (entry->hash == hash) {
      return entry;
    }

    // wrap to beginning
    index = (index + 1) % capacity;
  }

  return NULL;
}

static void grow_dict(Dict *dict) {
  int old_capacity = dict->list.size;
  int new_cap = 2 * old_capacity;
  struct list_Entry entries;
  list_Entry_init(&entries, new_cap);

  for (int i = 0; i < new_cap; i++) {
    struct Entry *entry = &entries.list.data[i];
    entry->hash = -1;
    entry->key = NULL;
    entry->value = NULL;
  }

  // copy existing
  for (int i = 0; i < dict->list.size; i++) {
    struct Entry *entry = &dict->list.data[i];

    if (entry->hash == -1)
      continue;

    struct Entry *dest = find_entry(&entries, new_cap, entry->hash);
    dest->key = entry->key;
    dest->value = entry->value;
  }

  list_Entry_free(dict);
  dict->list = entries.list;
}

bool dict_set(Dict *dict, const char *key, void *value) {
  if (dict->list.size + 1 > MAX_LOAD_FACTOR * dict->list.size) {
    grow_dict(dict);
  }
  uint32_t hash = hashString(key, strlen(key));
  struct Entry *entry = find_entry(dict, dict->list.size, hash);

  bool newEntry = entry->hash == -1;

  if (newEntry && entry->key == NULL) {
    dict->list.count++;
  }

  entry->key = key;
  entry->hash = hash;
  entry->value = value;

  return newEntry;
}

bool dict_set_copy(Dict *dict, const char *key, void *value, size_t val_size) {
  void *copy = malloc(val_size);
  memcpy(copy, value, val_size);
  return dict_set(dict, key, value);
}

void dict_init(Dict *dict) { list_Entry_init(dict, 8); }
void dict_free(Dict *dict) { list_Entry_free(dict); }
