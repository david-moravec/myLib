#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "dict.h"
#include "list.h"

typedef struct {
  uint64_t a;
  int b;
  void *c;
} TestingStruct;

LIST_DEFINE(test, TestingStruct);

typedef struct list_test listTest;

bool testingStructEquals(TestingStruct a, TestingStruct b) {
  return a.a == b.a && a.b && b.b && a.c == b.c;
}

void test_list_init() {
  struct list list;
  list_init(&list, sizeof(int), 8);

  assert(list.size == 8);
  assert(list.el_size == sizeof(int));
}

void test_list_grow() {
  struct list list;
  list_init(&list, sizeof(int), 8);

  assert(list.size == 8);
  assert(list.el_size == sizeof(int));
  list_grow(&list);
  assert(list.size == 16);
}

void test_list_append() {
  listTest list;
  list_test_init(&list, 8);
  assert(list.list.size == 8);

  TestingStruct data;
  data.a = 15000;
  data.b = 4;
  void *test_pointer = (void *)malloc(sizeof(char));
  data.c = test_pointer;
  size_t index;

  for (int i = 0; i < 1024; i++) {
    memcpy(list_test_append(&list, &index), &data, sizeof(TestingStruct));
  }
  assert(list.list.count == 1024);
  assert(index == 1023);

  TestingStruct obtained = *list_test_get(&list, 1000);
  assert(testingStructEquals(obtained, data));
}

void test_list_append_copy() {
  struct list list;
  list_init(&list, sizeof(uint64_t), 8);

  uint64_t data = 18;
  size_t index;

  list_append_copy(&list, &data, &index);
  assert(list.size == 8);
  assert(index == 0);

  uint64_t d = *(uint64_t *)list_get(&list, index);
  assert(d == data);
}

typedef struct {
  int field1;
  const char *field2;
  void *pointer;
} TestStruct;

DEFINE_DICT(test_struct, TestStruct);

void test_dict_init() {
  Dict dict;
  dict_test_struct_init(&dict);
}

void test_dict_set_get() {
  Dict dict;
  dict_test_struct_init(&dict);

  TestStruct data;
  data.field1 = 25;
  data.field2 = "ahoj";
  data.pointer = malloc(1024 * sizeof(char));

  assert(dict.list.count == 0);
  dict_test_struct_set(&dict, "testKey1", &data);
  assert(dict.list.count == 1);

  TestStruct *data_obtained;
  assert(!dict_test_struct_get(&dict, "testKey2", &data_obtained));
  assert(dict_test_struct_get(&dict, "testKey1", &data_obtained));
  assert(data_obtained == &data);
}

int main() {
  test_list_init();
  test_list_grow();
  test_list_append();
  test_list_append_copy();

  test_dict_init();
  test_dict_set_get();
  return 0;
}
