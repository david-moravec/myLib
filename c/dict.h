#ifndef MY_DICT_H
#define MY_DICT_H

#include <stdbool.h>
#include <stdint.h>

#include "list.h"

#define MAX_LOAD_FACTOR 0.8

struct Entry {
  uint32_t hash;
  const char *key;
  void *value;
};

LIST_TYPE_DEFINE(Entry, struct Entry);

typedef struct list_Entry Dict;

void dict_init(Dict *dict);
void dict_free(Dict *dict);

bool dict_get(Dict *dict, const char *key, void *value);
bool dict_get_copy(Dict *dict, const char *key, void *value, size_t val_size);
bool dict_set(Dict *dict, const char *key, void *value);
bool dict_set_copy(Dict *dict, const char *key, void *value, size_t val_size);

#define DEFINE_DICT(name, ValueType)                                           \
  static inline void dict_##name##_init(struct list_Entry *dict) {             \
    dict_init(dict);                                                           \
  }                                                                            \
  static inline void dict_##name##_free(struct list_Entry *dict) {             \
    dict_free(dict);                                                           \
  }                                                                            \
                                                                               \
  static inline bool dict_##name##_get(struct list_Entry *dict,                \
                                       const char *key, ValueType **value) {   \
    return dict_get(dict, key, value);                                         \
  }                                                                            \
  static inline bool dict_##name##_get_copy(                                   \
      struct list_Entry *dict, const char *key, ValueType *value) {            \
    return dict_get_copy(dict, key, value, sizeof(ValueType));                 \
  }                                                                            \
  static inline bool dict_##name##_set(struct list_Entry *dict,                \
                                       const char *key, ValueType *value) {    \
    return dict_set(dict, key, value);                                         \
  }                                                                            \
  static inline bool dict_##name##_set_copy(                                   \
      struct list_Entry *dict, const char *key, ValueType *value) {            \
    return dict_set_copy(dict, key, value, sizeof(ValueType));                 \
  }

#endif
