// Copyright 2022 Radulescu Matei
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "list.h"
#include "hashmap.h"
#include "user.h"
#include "book.h"
#include "auxiliary.h"

// This function adds a user to the users hashtable.
void add_user(hashtable_t *users)
{
    char *user_name;
    read_word(&user_name);

    // I check wether the user is already registered.
    if (ht_has_key(users, user_name))
    {
        printf("User is already registered.\n");
        free(user_name);
        return;
    }

    // I build my user_data struct with the default values.
    user_data *data = malloc(sizeof(user_data));
    if (data == NULL)
        exit(-1);

    data->points = 100;
    data->borrowed = 0;
    data->b_book = NULL;

    // I add the struct to the hastbale with ht_put.
    ht_put(users, user_name, strlen(user_name) + 1, data, sizeof(user_data));

    free(user_name);
    free(data);
}

// This function tells the system when somebody borrows a book.
void borrow(hashtable_t *users, hashtable_t *library)
{
    char *user_name, *book_name;
    int days;

    read_word(&user_name);
    read_book_name(&book_name);
    scanf("%d", &days);

    // I check wether the user is registered
    if (!ht_has_key(users, user_name))
    {
        printf("You are not registered yet.\n");
        free(user_name);
        free(book_name);
        return;
    }

    user_data *data = (user_data *)ht_get(users, user_name);

    // I check wether the user is banned.
    if (data->points < 0)
    {
        printf("You are banned from this library.\n");
        free(user_name);
        free(book_name);
        return;
    }

    // I check wethere user already has a borrowed book.
    if (data->borrowed == 1)
    {
        printf("You have already borrowed a book.\n");
        free(user_name);
        free(book_name);
        return;
    }

    // I check wether the book exists.
    if (ht_has_key(library, book_name) == 0)
    {
        printf("The book is not in the library.\n");
        free(user_name);
        free(book_name);
        return;
    }

    book_data *book = (book_data *)ht_get(library, book_name);

    // I check wether the book is already borrowed.
    if (book->borrowed == 1)
    {
        printf("The book is borrowed.\n");
        free(user_name);
        free(book_name);
        return;
    }

    // I change both borrowed values to 1 and add the books name
    // to the users struct.
    data->borrowed = 1;
    book->borrowed = 1;
    book->days_b = days;
    data->b_book = malloc(strlen(book_name) + 1);
    if (data->b_book == NULL)
        exit(-1);
    memmove(data->b_book, book_name, strlen(book_name) + 1);

    free(user_name);
    free(book_name);
}

// This books shows when a user returns a book adn what rating.
// he gives it.
void return_book(hashtable_t *users, hashtable_t *library)
{
    char *user_name, *book_name;
    int days, rating;

    read_word(&user_name);
    read_book_name(&book_name);
    scanf("%d ", &days);
    scanf("%d", &rating);

    user_data *data = (user_data *)ht_get(users, user_name);

    // I check wether the user has benn banned from the library.
    if (data->points < 0)
    {
        printf("You are banned from this library.\n");
        free(user_name);
        free(book_name);
        return;
    }

    // I check wether the user hasnt borrowed a book or he has borrowed
    // another one.
    if (data->borrowed == 0 || strcmp(book_name, data->b_book) != 0)
    {
        printf("You didn't borrow this book.\n");
        free(user_name);
        free(book_name);
        return;
    }

    // I return the borrowed balues to 0 and the book's name
    // to NULL.
    book_data *book = (book_data *)ht_get(library, book_name);

    book->borrowed = 0;
    data->borrowed = 0;
    free(data->b_book);
    data->b_book = NULL;

    book->t_rating = book->t_rating + rating;
    book->purchases = book->purchases + 1;

    // I than calculate the new points of the user based
    // on the number of days he borrowed the book for
    // and when he actually returned it.
    if (book->days_b < days)
    {
        data->points = data->points - 2 * (days - book->days_b);
    }
    else
    {
        data->points = data->points + book->days_b - days;
    }

    // I announce that the user is now banned if his points have fallen
    // below 0.
    if (data->points < 0)
        printf("The user %s has been banned from this library.\n", user_name);

    free(user_name);
    free(book_name);
}

void lost(hashtable_t *users, hashtable_t *library)
{
    char *user_name, *book_name;

    read_word(&user_name);
    read_book_name(&book_name);

    // I check wether the user is registered.
    if (!ht_has_key(users, user_name))
    {
        printf("You are not registered yet.\n");
        free(user_name);
        free(book_name);
        return;
    }

    user_data *data = (user_data *)ht_get(users, user_name);

    // I check wether the user is banned in the library.
    if (data->points < 0)
    {
        printf("You are banned from this library.\n");
        free(user_name);
        free(book_name);
        return;
    }

    // I decrease the user's point by 50 as he has lost a book.
    data->points = data->points - 50;
    data->borrowed = 0;
    free(data->b_book);
    data->b_book = NULL;

    // If the user's points reach 0 I announce that he is banned.
    if (data->points < 0)
    {
        printf("The user %s has been banned from this library.\n", user_name);
    }

    // I remove the book as it's been lost.
    book_data *book = (book_data *)ht_get(library, book_name);
    hashtable_t *aux = book->content;
    ht_free(aux);
    ht_remove_entry(library, book_name);

    free(user_name);
    free(book_name);
}

// This function prints the top users in the library.
void top_user(hashtable_t *users)
{
    sort_user *sort = malloc(users->size * sizeof(sort_user));
    if (sort == NULL)
        exit(-1);

    int nr = 0;

    for (int i = 0; i < (int)users->hmax; i++)
    {
        node *it = users->buckets[i]->head;
        for (int j = 0; j < users->buckets[i]->size; j++)
        {
            // I save the user's name and their points
            // in the struct i made.
            info *data = (info *)it->data;
            sort[nr].user_name = malloc(data->key_size);
            if (sort[nr].user_name == NULL)
                exit(-1);
            memmove(sort[nr].user_name, data->key, data->key_size);
            user_data *aux = (user_data *)data->value;
            sort[nr].points = aux->points;
            nr++;
            it = it->next;
        }
    }

    // I use a bubble sort to sort the "vector" of structs
    // based on points and if they have equal points i do it
    // lexicografically.
    for (int i = 0; i < nr - 1; i++)
    {
        for (int j = 0; j < nr - i - 1; j++)
        {
            if (sort[j].points < sort[j + 1].points)
            {
                sort_user aux;
                aux = sort[j];
                sort[j] = sort[j + 1];
                sort[j + 1] = aux;
            }

            if (sort[j].points == sort[j + 1].points)
            {
                if (strcmp(sort[j].user_name, sort[j + 1].user_name) > 0)
                {
                    sort_user aux;
                    aux = sort[j];
                    sort[j] = sort[j + 1];
                    sort[j + 1] = aux;
                }
            }
        }
    }

    // I used the "vector" which i already sorted t print
    // the top users without printing the banned one's.
    printf("Users ranking:\n");
    for (int i = 0; i < nr; i++)
    {
        if (sort[i].points >= 0)
            printf("%d. Name:%s Points:%d\n", (i + 1),
                   sort[i].user_name, sort[i].points);

        free(sort[i].user_name);
    }

    free(sort);
}
