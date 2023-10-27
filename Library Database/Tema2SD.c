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

// I used a function that reads one word at a time in order to read the
// commands. I started each of the two hashtables with the size of 1 in
// order to use toresize them later for efficiency. I also used throughout
// the programdefensive programming were needed.

// I used hash and compare function for strings as all my keys are strings.
// I tried to make a header for each separate part.
int main(void)
{
    hashtable_t *library = NULL, *users = NULL;
    library = ht_create(1, hash_function_string, compare_function_strings);
    users = ht_create(1, hash_function_string, compare_function_strings);
    char *command;
    int endline;

    endline = read_word(&command);

    // I read commands untill i find the exit command and in the loop
    // i check for all possible commands.
    while (strcmp(command, "EXIT") != 0)
    {
        if (!strcmp(command, "ADD_BOOK"))
        {
            add_book(library);
        }
        else if (!strcmp(command, "GET_BOOK"))
        {
            get_book(library);
        }
        else if (!strcmp(command, "RMV_BOOK"))
        {
            rmv_book(library);
        }
        else if (!strcmp(command, "ADD_DEF"))
        {
            add_def(library);
        }
        else if (!strcmp(command, "GET_DEF"))
        {
            get_def(library);
        }
        else if (!strcmp(command, "RMV_DEF"))
        {
            rmv_def(library);
        }
        else if (!strcmp(command, "ADD_USER"))
        {
            add_user(users);
        }
        else if (!strcmp(command, "BORROW"))
        {
            borrow(users, library);
        }
        else if (!strcmp(command, "RETURN"))
        {
            return_book(users, library);
        }
        else if (!strcmp(command, "LOST"))
        {
            lost(users, library);
        }
        else
        {
            printf("Invalid command. Please try again.\n");
            // I read untill the line has ended in this error case as all
            // commands are 1 line.
            while (endline == 1)
            {
                free(command);
                endline = read_word(&command);
            }
        }
        if (strcmp(command, "EXIT"))
        {
            free(command);
            endline = read_word(&command);
        }
    }
    free(command);
    // I use my functions witch show the final top
    top_book(library);
    top_user(users);

    // I free the hashtables
    free_library(library);
    free_users(users);
}
