#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#define T 2
#define _CRT_SECURE_NO_WARNINGS

typedef struct node {
    struct node* parent;
    struct node* children[2 * T ];
    int length;
    int keys[2*T - 1];
} node;


typedef struct node_list {
    struct node_list* next;
    struct node_list* prev;
    node* val;
}node_list;

typedef struct split_retval {
    node* left;
    node* right;
} split_retval;

node_list* list_add(node_list* l, node* v);

node* Node();



node* search(node* v, int  data);

int cmpfu(const void* a, const void* b);

void sort_son(node* v);

int subtree_max(node* v);

void update_keys(node* v);

void split_parent(node* d);

node* add(node* d, int x);

node* find_brother(node* v);

void delete_in_node(node* v);

node* del(node* v, int x);

int get_height(node* v);

node* merge(node* l, node* r);

node* v;

split_retval split(node* d, int key);

