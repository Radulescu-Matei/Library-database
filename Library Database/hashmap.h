// Copyright 2022 Radulescu Matei
#ifndef HASHMAP_H_
#define HASHMAP_H_

#include "list.h"

typedef struct hashtable_t
{
	list **buckets;
	unsigned int size;
	unsigned int hmax;
	unsigned int (*hash_function)(void *);
	int (*compare_function)(void *, void *);
} hashtable_t;

typedef struct info info;
struct info
{
	void *key;
	void *value;
	int key_size;
	int value_size;
};

int compare_function_strings(void *a, void *b);

unsigned int
hash_function_string(void *a);

hashtable_t *
ht_create(unsigned int hmax, unsigned int (*hash_function)(void *),
		  int (*compare_function)(void *, void *));

int ht_has_key(hashtable_t *ht, void *key);

void *
ht_get(hashtable_t *ht, void *key);

void ht_free(hashtable_t *ht);

void ht_put(hashtable_t *ht, void *key, unsigned int key_size,
			void *value, unsigned int value_size);

void ht_remove_entry(hashtable_t *ht, void *key);

#endif  //  HASHMAP_H_
