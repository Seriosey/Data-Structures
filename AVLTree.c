#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AVLTree.h"
#define _CRT_SECURE_NO_WARNINGS


node* new_node(int data) {
	node* new_node = (node*)malloc(sizeof(node));
	new_node->data = data;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->height = 1;
	return new_node;
}

node* insertel(node* p, int data) {
	if (!p) return new_node(data);
	if (p->data > data) p->left = insertel(p->left, data);
	else p->right = insertel(p->right, data);
	return balance(p);
}

node* deletel(node* p, int data) {
	if (!p) return NULL;
	if (p->data > data) p->left = deletel(p->left, data);
	else if (p->data < data) p->right = deletel(p->right, data);
	else {
		node* l = p->left;
		node* r = p->right;
		if (!r) return l;
		node* min = findmin(r);
		min->right = deletemin(r);
		min->left = l;
		return balance(min);
	}
	return balance(p);
}

int findel(node* x, int data) {
	node* root = x;
	while (x != NULL) {
		if (x->data > data) x = x->left;
		else if (x->data == data) return 1;
		else x = x->right;
	}
	x = root;
	return 0;
}

unsigned char height(node* p) {
	return p ? p->height : 0;
}

int bfactor(node* p) {
	return height(p->right) - height(p->left);
}

void fixheight(node* p) {
	if (p->left && p->right) {
		unsigned char hl = height(p->left);
		unsigned char hr = height(p->right);
		p->height = (hl > hr ? hl : hr) + 1;
	}
}

node* rotateright(node* p) {
	node* l = p->left;
	p->left = l->right;
	l->right = p;
	fixheight(p);
	fixheight(l);
	return l;
}

node* rotateleft(node* p) {
	node* r = p->right;
	p->right = r->left;
	r->left = p;
	fixheight(p);
	fixheight(r);
	return r;
}

node* balance(node* p) {
	fixheight(p);
	if (bfactor(p) == 2)
	{
		if (bfactor(p->right) < 0) p->right = rotateright(p->right);
		return rotateleft(p);
	}
	if (bfactor(p) == -2)
	{
		if (bfactor(p->left) > 0) p->left = rotateleft(p->left);
		return rotateright(p);
	}
	return p;
}


node* findmin(node* p) {
	return p->left ? findmin(p->left) : p;
}

node* deletemin(node* p) {
	if (p->left == NULL) return p->right;
	p->left = deletemin(p->left);
	return balance(p);
}
