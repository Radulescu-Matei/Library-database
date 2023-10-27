// Copyright 2022 Radulescu Matei
#ifndef BOOK_H_
#define BOOK_H_

#include "hashmap.h"

// I made a struct that contains a book's
// total rating, it's number of purchases.
// wether it's borrowed or not, a hastable
// containing the definitions within the book
// and also the number of days it's been
// borrowed for.
typedef struct book_data
{
    int t_rating;
    int purchases;
    int borrowed;
    int days_b;
    hashtable_t *content;
} book_data;

// I made a structs that has a book's name, it's
// rating and it's number of purchases. I use this
// for printing the top books.
typedef struct sort_book
{
    char *book_name;
    float rating;
    int purchases;
} sort_book;

void add_book(hashtable_t *library);

void get_book(hashtable_t *library);

void rmv_book(hashtable_t *library);

void add_def(hashtable_t *library);

void get_def(hashtable_t *library);

void rmv_def(hashtable_t *library);

void top_book(hashtable_t *library);

#endif  //  BOOK_H_
