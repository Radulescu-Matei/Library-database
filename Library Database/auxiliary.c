// Copyright 2022 Radulescu Matei
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hashmap.h"
#include "book.h"
#include "user.h"

void conv_number(char *word, int *nr)
{
    int number = 0;
    if (word[0] == '-')
    {
        return;
    }

    for (int i = 0; i < (int)strlen(word); i++)
        if (!isdigit(word[i]))
            return;

    number = atoi(word);

    (*nr) = number;
    return;
}

int read_word(char **word)
{
    char letter, *word2 = malloc(0);
    if (word2 == NULL)
        exit(-1);
    int size = 0;

    scanf("%c", &letter);

    while (letter == ' ' || letter == '\n')
        scanf("%c", &letter);

    while (letter != ' ' && letter != '\n')
    {
        size++;
        word2 = realloc(word2, (size + 1) * sizeof(char));
        word2[size - 1] = letter;
        scanf("%c", &letter);

        if (size == 4 && word2[0] == 'E' && word2[1] == 'X'
            && word2[2] == 'I' && word2[3] == 'T')
            break;
    }
    (*word) = word2;
    (*word)[size] = '\0';

    if (letter == '\n')
        return 0;

    return 1;
};

void read_book_name(char **book)
{
    char letter, *word2 = malloc(0);
    if (word2 == NULL)
        exit(-1);
    int size = 0;
    scanf("%c", &letter);
    while (letter == ' ' || letter == '\n')
        scanf("%c", &letter);

    scanf("%c", &letter);

    while (letter != '"')
    {
        size++;
        word2 = realloc(word2, (size + 1) * sizeof(char));
        word2[size - 1] = letter;
        scanf("%c", &letter);
    }
    scanf("%c", &letter);
    (*book) = word2;
    (*book)[size] = '\0';
}

void free_library(hashtable_t *library)
{
    for (int i = 0; i < (int)library->hmax; i++)
    {
        node *it = library->buckets[i]->head;
        for (int j = 0; j < (int)library->buckets[i]->size; j++)
        {
            info *data = (info *)it->data;
            book_data *aux = (book_data *)data->value;
            ht_free(aux->content);
            it = it->next;
        }
    }

    ht_free(library);
}

void free_users(hashtable_t *users)
{
    for (int i = 0; i < (int)users->hmax; i++)
    {
        node *it = users->buckets[i]->head;
        for (int j = 0; j < (int)users->buckets[i]->size; j++)
        {
            info *data = (info *)it->data;
            user_data *aux = (user_data *)data->value;
            free(aux->b_book);
            aux->b_book = NULL;
            it = it->next;
        }
    }

    ht_free(users);
}
