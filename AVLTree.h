#pragma once

typedef struct node {
	int data;
	int height;
	struct node* left;
	struct node* right;

} node;

int findel(node* root, int data);

unsigned char height(node* p);
int bfactor(node* p);
void fixheight(node* p);
node* rotateright(node* p);
node* rotateleft(node* p);
node* balance(node* p);

node* new_node(int data);
node* insertel(node* p, int data);

node* findmin(node* p);
node* deletemin(node* p);
node * deletel(node * p, int data);
