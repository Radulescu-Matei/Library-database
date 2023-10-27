// Copyright 2022 Radulescu Matei
#ifndef AUXILIARY_H_
#define AUXILIARY_H_

#include "hashmap.h"
#include "book.h"

void conv_number(char *word, int *nr);

int read_word(char **word);

void read_book_name(char **book);

void free_book(book_data *rmv);

void free_library(hashtable_t *library);

void free_users(hashtable_t *users);

void free_hash_element(list **pp_list);

#endif  //  AUXILIARY_H_
