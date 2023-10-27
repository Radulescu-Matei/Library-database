// Copyright 2022 Radulescu Matei
#ifndef LIST_H_
#define LIST_H_

typedef struct node
{
    void *data;
    struct node *prev, *next;
} node;

typedef struct list
{
    struct node *head;
    unsigned int data_size;
    int size;
} list;

list *create_list(unsigned int data_size);

node *get_node(list *list, int n);

void add_node_list(list *list, int n, const void *data);

node *remove_node_list(list *list, int n);

void free_list(list **pp_list);

#endif  //  LIST_H_
