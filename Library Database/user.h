// Copyright 2022 Radulescu Matei
#ifndef USER_H_
#define USER_H_

// I made a struct that contains a user's points
// a value that says wether he borrowe a book or not
// and the name of the book he borrowed.
typedef struct user_data
{
    int borrowed;
    int points;
    char *b_book;
} user_data;

// I made a struct that has a user's name and his points
// i use this for printing the top users.
typedef struct sort_user
{
    char *user_name;
    int points;
} sort_user;

void add_user(hashtable_t *users);

void borrow(hashtable_t *users, hashtable_t *library);

void return_book(hashtable_t *users, hashtable_t *library);

void lost(hashtable_t *users, hashtable_t *library);

void top_user(hashtable_t *users);

#endif  //  USER_H_
