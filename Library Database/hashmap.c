// Copyright 2022 Radulescu Matei
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "list.h"
#include "hashmap.h"
#include "auxiliary.h"

// A function that compares two string keys.
int compare_function_strings(void *a, void *b)
{
	char *str_a = (char *)a;
	char *str_b = (char *)b;

	return strcmp(str_a, str_b);
}

// A hash function for strings.
unsigned int
hash_function_string(void *a)
{
	/*
	 * Credits: http://www.cse.yorku.ca/~oz/hash.html
	 */
	unsigned char *puchar_a = (unsigned char *)a;
	unsigned int hash = 5381;
	int c;

	while ((c = *puchar_a++))
		hash = ((hash << 5u) + hash) + c; /* hash * 33 + c */

	return hash;
}

// A function that crates a hashtable of given max size and
// hash/compare functions.
hashtable_t *
ht_create(unsigned int hmax, unsigned int (*hash_function)(void *),
		  int (*compare_function)(void *, void *))
{
	hashtable_t *hash;
	hash = malloc(sizeof(*hash));
	if (hash == NULL)
		exit(-1);

	hash->hmax = hmax;
	hash->buckets = malloc(hmax * sizeof(list *));
	if (hash->buckets == NULL)
		exit(-1);

	hash->size = 0;

	for (int i = 0; i < (int)hmax; i++)
	{
		hash->buckets[i] = create_list(sizeof(info));
	}

	hash->hash_function = hash_function;
	hash->compare_function = compare_function;

	return hash;
}

// A function that checks wether a key already exists in a hasthable.
int ht_has_key(hashtable_t *ht, void *key)
{
	int index = ht->hash_function(key);
	index = index % ht->hmax;

	list *l = ht->buckets[index];

	node *aux = l->head;
	for (int i = 0; i < l->size; i++)
	{
		info *inside_data = (info *)aux->data;
		int value = ht->compare_function(inside_data->key, key);

		if (value == 0)
			return 1;

		aux = aux->next;
		inside_data = aux->data;
	}
	return 0;
}

// A function that fetches and element from a hashtable.
void *
ht_get(hashtable_t *ht, void *key)
{
	int index = ht->hash_function(key);
	index = index % ht->hmax;

	list *l = ht->buckets[index];
	node *aux = l->head;

	while (aux)
	{
		info *inside_data = (info *)aux->data;
		int value = ht->compare_function(inside_data->key, key);

		if (value == 0)
			return inside_data->value;

		aux = aux->next;
		inside_data = aux->data;
	}
	return NULL;
}

// A function that puts an element in hashtable.
void ht_put(hashtable_t *ht, void *key, unsigned int key_size,
			void *value, unsigned int v_s)
{
	ht->size++;
	int index = ht->hash_function(key);
	index = index % ht->hmax;
	info inside_data;
	inside_data.key = malloc(key_size);
	if (inside_data.key == NULL)
		exit(-1);
	inside_data.value = malloc(v_s);
	if (inside_data.value == NULL)
		exit(-1);
	memcpy(inside_data.value, value, v_s);
	memcpy(inside_data.key, key, key_size);
	inside_data.key_size = key_size;
	inside_data.value_size = v_s;

	add_node_list(ht->buckets[index], ht->buckets[index]->size, &inside_data);

	// Here i implemented the resize function. The number 1 is the load_factor
	// which can be swapped for anyother.
	if (((double)(ht->size) / ht->hmax) > 1)
	{
		// I made a point of the info structs were i will save
		// all my keys and values before i empty the hashtable.
		// I also saved their sizes in this struct for moving them.
		info *copy = malloc(ht->size * sizeof(info));
		if (copy == NULL)
			exit(-1);
		int nr = 0;
		for (int i = 0; i < (int)ht->hmax; i++)
		{
			node *it = ht->buckets[i]->head;
			for (int j = 0; j < (int)ht->buckets[i]->size; j++)
			{
				// I build my copy "vector" so i can build
				// the hashtbale with it's new size.
				info *data = (info *)it->data;
				copy[nr].key = malloc(data->key_size);
				if (copy[nr].key == NULL)
					exit(-1);
				copy[nr].value = malloc(data->value_size);
				if (copy[nr].value == NULL)
					exit(-1);
				memcpy(copy[nr].key, data->key, data->key_size);
				memcpy(copy[nr].value, data->value, data->value_size);
				copy[nr].key_size = data->key_size;
				copy[nr].value_size = data->value_size;

				it = it->next;
				nr++;
			}

			// I empty the buckets of the hastable.
			for (int j = 0; j < (int)ht->buckets[i]->size;)
			{
				node *aux;
				info *inside_data;

				aux = remove_node_list(ht->buckets[i], 0);
				inside_data = (info *)aux->data;
				free(inside_data->key);
				free(inside_data->value);
				free(aux->data);
				free(aux);
			}

			free_list(&ht->buckets[i]);
		}

		// I double the size and alloc the new size for the buckets
		// and other sizes.
		int new_max = ht->hmax * 2;
		free(ht->buckets);
		ht->size = 0;
		ht->hmax = new_max;
		ht->buckets = malloc(ht->hmax * sizeof(list *));
		if (ht->buckets == NULL)
			exit(-1);
		for (int i = 0; i < (int)ht->hmax; i++)
			ht->buckets[i] = create_list(sizeof(info));

		// I place the elements saved in copy in the newly alloced hashtable.
		for (int i = 0; i < nr; i++)
		{
			ht_put(ht, copy[i].key, copy[i].key_size, copy[i].value, copy[i].value_size);
			free(copy[i].key);
			free(copy[i].value);
		}

		free(copy);
	}
}

// This function removes an entry from a hashtable.
void ht_remove_entry(hashtable_t *ht, void *key)
{
	int index = ht->hash_function(key);
	index = index % ht->hmax;

	list *l = ht->buckets[index];
	node *aux = l->head;

	int nr = 0;

	while (aux)
	{
		info *inside_data = (info *)aux->data;
		int value = ht->compare_function(inside_data->key, key);

		if (value == 0)
		{
			aux = remove_node_list(ht->buckets[index], nr);
			inside_data = (info *)aux->data;
			free(inside_data->key);
			free(inside_data->value);
			free(aux->data);
			free(aux);
			ht->size--;

			return;
		}

		nr++;
		aux = aux->next;
		inside_data = aux->data;
	}
}

// This function frees a hashtable.
void ht_free(hashtable_t *ht)
{
	if (ht == NULL)
		return;

	for (int i = 0; i < (int)ht->hmax; i++)
	{
		for (int j = 0; j < (int)ht->buckets[i]->size;)
		{
			list *l = ht->buckets[i];
			node *aux = l->head;
			info *inside_data = (info *)aux->data;

			aux = remove_node_list(ht->buckets[i], 0);
			inside_data = (info *)aux->data;
			free(inside_data->key);
			free(inside_data->value);
			free(aux->data);
			free(aux);
		}

		free_list(&ht->buckets[i]);
	}

	free(ht->buckets);
	free(ht);
}
