// Copyright 2022 Radulescu Matei
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "list.h"

// This function creates an empty list of a given type of data.
list *create_list(unsigned int data_size)
{
    list *new_list = malloc(sizeof(list));
    if (new_list == NULL)
        exit(-1);
    new_list->head = NULL;
    new_list->data_size = data_size;
    new_list->size = 0;
    return new_list;
}

// This function returns the node you ask for from the given list.
node *get_node(list *list, int n)
{
    node *aux = list->head;
    for (int i = 0; i < n; i++)
        aux = aux->next;

    return aux;
}

// This function adds a node to the given list with the given data in it.
void add_node_list(list *list, int n, const void *data)
{
    node *new_node = malloc(sizeof(node));
    if (new_node == NULL)
        exit(-1);
    new_node->data = malloc(list->data_size);
    if (new_node->data == NULL)
        exit(-1);
    memcpy(new_node->data, data, list->data_size);
    list->size++;
    if (list->head == NULL)
    {
        new_node->next = new_node;
        new_node->prev = new_node;
        list->head = new_node;
        return;
    }
    if (n == 0)
    {
        new_node->next = list->head;
        new_node->prev = list->head->prev;
        list->head->prev = new_node;
        list->head = new_node;
        return;
    }

    node *previous = list->head;
    node *after = list->head;

    if (n >= list->size - 1)
    {
        if (list->head->prev != NULL)
            previous = list->head->prev;
        new_node->next = list->head;
        new_node->prev = previous;
        previous->next = new_node;
        list->head->prev = new_node;
        return;
    }

    for (int i = 0; i < (n - 1); i++)
        previous = previous->next;
    after = previous->next;

    new_node->next = after;
    after->prev = new_node;
    previous->next = new_node;
    new_node->prev = previous;
}

// This function removes a node from a given list
node *remove_node_list(list *list, int n)
{
    if (n >= list->size - 1)
        n = list->size - 1;
    node *previous = list->head;
    node *after;
    node *removed;
    list->size--;
    if (list->size == 0)
    {
        list->head = NULL;
        return previous;
    }
    if (n == 0)
    {
        list->head->next->prev = list->head->prev;
        list->head = list->head->next;
        list->head->prev->next = list->head;
        return previous;
    }

    for (int i = 0; i < n - 1; i++)
        previous = previous->next;
    removed = previous->next;
    after = removed->next;
    after->prev = previous;
    previous->next = after;

    return removed;
}

// This function frees a given list, i made another for the list which
// contains other lists in it.
void free_list(list **pp_list)
{
    node *removed;
    while ((*pp_list)->size)
    {
        removed = remove_node_list((*pp_list), 0);
        if ((*pp_list)->size == 0)
            (*pp_list)->head = NULL;

        free(removed->data);
        removed->data = NULL;
        free(removed);
        removed = NULL;
    }
    free((*pp_list));
    (*pp_list) = NULL;
}
