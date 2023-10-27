// Copyright 2022 Radulescu Matei
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "list.h"
#include "auxiliary.h"
#include "hashmap.h"
#include "book.h"

// This function adds a book to the "library", in terms of coding it adds
// in a hash_table my book_data struct which contains another hash_table
// and a few other details about the book.
void add_book(hashtable_t *library)
{
    char *book = NULL, *def = NULL, *val = NULL;
    char *word;

    // I read the book's name with the special function i made just for that.
    read_book_name(&book);
    read_word(&word);

    // I verify if the book already exists and if it does i delete it
    // and than add it again as stated in the update operation
    if (ht_has_key(library, book))
    {
        book_data *book_aux = (book_data *)ht_get(library, book);
        hashtable_t *aux = book_aux->content;
        ht_free(aux);
        ht_remove_entry(library, book);
    }

    // Because i read everything in strings i had some problem with this reading
    // this number in particular so i made a function that converts a string
    // to a number.
    int nr;
    conv_number(word, &nr);
    book_data *data = malloc(sizeof(book_data));
    if (data == NULL)
        exit(-1);

    // I build an empty struct at first
    data->purchases = 0;
    data->t_rating = 0;
    data->borrowed = 0;
    data->content = ht_create(1, library->hash_function,
                              library->compare_function);

    for (int i = 0; i < nr; i++)
    {
        // I than read all of the definitions and add them to the hashtable
        // that i have in the struct
        read_word(&def);
        read_word(&val);

        // I update the definition in case i get two with the same name.
        if (ht_has_key(data->content, def))
        {
            ht_remove_entry(data->content, def);
        }
        ht_put(data->content, def, strlen(def) + 1, val, strlen(val) + 1);

        free(def);
        free(val);
        def = NULL;
        val = NULL;
    }

    // I put the struct in the library hashtable and free the remaining
    // pointers.
    ht_put(library, book, strlen(book) + 1, data, sizeof(book_data));
    free(data);
    free(book);
    free(def);
    free(word);
}

// This function returns the rating and number of purchases of a book.
void get_book(hashtable_t *library)
{
    char *name;
    read_book_name(&name);
    // I verify wether the book is in the library.
    if (ht_has_key(library, name) == 0)
    {
        printf("The book is not in the library.\n");
        free(name);
        return;
    }

    // I than use ht_get to fetch my struct.
    book_data *book = (book_data *)ht_get(library, name);

    float rating = 0;
    // I calculate the rating as stated in the starting document and than
    // i print the name and values of the book.
    printf("Name:%s ", name);
    if (book->purchases != 0)
    {
        rating = ((double)book->t_rating / book->purchases);
    }
    printf("Rating:%.3f ", rating);
    printf("Purchases:%d\n", book->purchases);

    free(name);
}

// This function removes a book from my library
void rmv_book(hashtable_t *library)
{
    char *name;
    read_book_name(&name);
    // I check if the book is in the library.
    if (!ht_has_key(library, name))
    {
        printf("The book is not in the library.\n");
        free(name);
        return;
    }

    // I fetch my struct so i can free the hashtable in it
    // with the ht_free function that simply remove
    // the book with the ht_remove_entry function.
    book_data *book = (book_data *)ht_get(library, name);
    hashtable_t *aux = book->content;
    ht_free(aux);
    ht_remove_entry(library, name);
    free(name);
}

// This function adds a definition to one of the books
void add_def(hashtable_t *library)
{
    char *name, *key, *val;
    read_book_name(&name);
    read_word(&key);
    read_word(&val);

    // I check wether the given book is in the library.
    if (ht_has_key(library, name) == 0)
    {
        printf("The book is not in the library.\n");
        free(name);
        free(key);
        free(val);
        return;
    }

    // I fetch the book for further verifications.
    book_data *book = (book_data *)ht_get(library, name);
    hashtable_t *aux = book->content;

    // I verify wether the definition already exists so i can just update it.
    if (ht_has_key(aux, key))
    {
        ht_remove_entry(aux, key);
    }

    // I use the put function to add the definition to the required book.
    ht_put(aux, key, strlen(key) + 1, val, strlen(val) + 1);

    free(name);
    free(key);
    free(val);
}

// This function returns a given definition from a given book.
void get_def(hashtable_t *library)
{
    char *name, *key;
    read_book_name(&name);
    read_word(&key);

    // I verify wether the book is in the library.
    if (ht_has_key(library, name) == 0)
    {
        printf("The book is not in the library.\n");
        free(name);
        free(key);
        return;
    }

    // I fetch my struct.
    book_data *book = (book_data *)ht_get(library, name);
    hashtable_t *aux = book->content;

    // I verify wether the definition is in the given book.
    if (ht_has_key(aux, key) == 0)
    {
        printf("The definition is not in the book.\n");
        free(name);
        free(key);
        return;
    }

    // I cast the definition as a char * and print it.
    char *val = ht_get(aux, key);
    printf("%s\n", val);

    free(name);
    free(key);
}

// This function removes a definition from a given book.
void rmv_def(hashtable_t *library)
{
    char *name, *key;
    read_book_name(&name);
    read_word(&key);

    // I check wether the book is in the library.
    if (!ht_has_key(library, name))
    {
        printf("The book is not in the library.\n");
        free(name);
        free(key);
        return;
    }

    // I fetch my book.
    book_data *book = (book_data *)ht_get(library, name);
    hashtable_t *aux = book->content;

    // I check were the definition is in the book.
    if (ht_has_key(aux, key) == 0)
    {
        printf("The definition is not in the book.\n");
        free(name);
        free(key);
        return;
    }

    // I use the ht_remove_entry fucntion to remove the definition.
    ht_remove_entry(aux, key);
    free(name);
    free(key);
}

// This function prints the top_books in the library.
void top_book(hashtable_t *library)
{
    // I made a struct in order to help me print the required data
    // and i made a pointer in which i score all the structs that
    // i need to print.
    sort_book *sort = malloc(library->size * sizeof(sort_book));
    if (sort == NULL)
        exit(-1);

    int nr = 0;

    for (int i = 0; i < (int)library->hmax; i++)
    {
        node *it = library->buckets[i]->head;
        for (int j = 0; j < (int)library->buckets[i]->size; j++)
        {
            // For each element i add the books_name and the number of
            // in the struct and than i calculate the rating and add it
            // as well.
            info *data = (info *)it->data;
            sort[nr].book_name = data->key;
            book_data *aux = (book_data *)data->value;
            sort[nr].purchases = aux->purchases;
            float rating = 0;
            if (aux->purchases != 0)
                rating = (float)((float)aux->t_rating / (float)aux->purchases);

            sort[nr].rating = rating;
            nr++;
            it = it->next;
        }
    }

    // I use a bubble sort to sort my "vector" of structs
    // based on the rating, in case their equal i explained
    // what i do below.
    for (int i = 0; i < (int)library->size; i++)
    {
        for (int j = 0; j < (int)library->size - i - 1; j++)
        {
            if (sort[j].rating < sort[j + 1].rating)
            {
                sort_book aux;
                memmove(&aux, &sort[j], sizeof(sort_book));
                memmove(&sort[j], &sort[j + 1], sizeof(sort_book));
                memmove(&sort[j + 1], &aux, sizeof(sort_book));
            }

            if (sort[j].rating == sort[j + 1].rating)
            {
                // In this case i check which book has the most
                // purchases.
                if (sort[j].purchases < sort[j + 1].purchases)
                {
                    sort_book aux;
                    memmove(&aux, &sort[j], sizeof(sort_book));
                    memmove(&sort[j], &sort[j + 1], sizeof(sort_book));
                    memmove(&sort[j + 1], &aux, sizeof(sort_book));
                }
                else if (sort[j].purchases == sort[j + 1].purchases)
                {
                    // If both values are equal i compare lexicografic values
                    // using strcmp.
                    if (strcmp(sort[j].book_name, sort[j + 1].book_name) > 0)
                    {
                        sort_book aux;
                        memmove(&aux, &sort[j], sizeof(sort_book));
                        memmove(&sort[j], &sort[j + 1], sizeof(sort_book));
                        memmove(&sort[j + 1], &aux, sizeof(sort_book));
                    }
                }
            }
        }
    }

    // I than print each struct as i already have put them in order.
    printf("Books ranking:\n");
    for (int i = 0; i < (int)library->size; i++)
    {
        sort_book aux;
        memmove(&aux, &sort[i], sizeof(sort_book));
        printf("%d. Name:%s Rating:%.3f Purchases:%d\n",
               (i + 1), aux.book_name, aux.rating, aux.purchases);
    }

    free(sort);
}
