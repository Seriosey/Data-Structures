#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "I11-Tree.h"
#include <malloc.h>
#define _CRT_SECURE_NO_WARNINGS


node* Node() {
    node* n = (node*)malloc(sizeof(node));
    n->parent = NULL;
    n->length = 1;
    n->children[0] = NULL;
    n->children[1] = NULL;
    n->children[2] = NULL;
    n->children[3] = NULL;
    return n;
}

node* root = NULL;


int find(node* v, int x) {
    if (v == NULL)
        return 0;
    for (int i = 0; i < v->length; i++)
        if (v->keys[i] == x) return 1;
    return 0;
}

node* add(node* d, int x) {
    node* n = (node*)malloc(sizeof(node));
    n->length = 1;
    n->keys[0] = x;
    d = root;

    if (d == NULL) {
        root = n;
        return root;
    }
    node* a = search(d, x);
    if (a->keys[0] == x) return root;
    if (a->parent == NULL) {
        node* new_root = Node();
        new_root->length = 2;
        new_root->children[0] = a;
        new_root->children[1] = n;
        a->parent = new_root;
        n->parent = new_root;
        sort_son(new_root);
        root = new_root;
    }
    else {
        node* p = a->parent;
        p->children[p->length] = n;
        n->parent = p;
        p->length++;
        sort_son(p);
        update_keys(n);
        split_parent(p);
    }
    update_keys(n);

    return root;
}


node_list* list_add(node_list* l, node* v) {
	node_list* n = (node_list*)malloc(sizeof(node_list));
	l->next = n;
	n->next = NULL;
	n->prev = l;
	n->val = v;
	return n;
}


node* search(node* v, int x) {
    if (!v) return NULL;

    if (find(v, x)) return v;
    else if (x < v->keys[0]) return search(v->children[0], x);
    else if ((v->length == 2) && (x < v->keys[1]) || (v->length == 1)) return search(v->children[1], x);
    else if (v->length == 2) return search(v->children[2], x);
}


int cmpfu(const void* a, const void* b) {
    node** val1 = (node**)a;
    node** val2 = (node**)b;

    return (*val1)->keys[0] - (*val2)->keys[0];
}

void sort_son(node* v) {
    if (v == NULL) {
        return;
    }
    node* arr[4];
    for (int i = 0; i < v->length; i++) {
        arr[i] = v->children[i];
    }
    qsort(arr, v->length, sizeof(node*), cmpfu);

    for (int i = 0; i < v->length; i++) {
        v->children[i] = arr[i];
    }
}


int subtree_max(node* v) {
    node* d = v;
    while (d->length != 1) {
        d = d->children[d->length - 1];
    }
    return d->keys[0];
}


void update_keys(node* v) {
    if (v == NULL) {
        return;
    }

    node* p = v->parent;
    while (p != NULL) {
        for (int i = 0; i < p->length - 1; i++) {
            p->keys[i] = subtree_max(p->children[i]);
        }
        p = p->parent;
    }
}


void split_parent(node* d) {
    if (d == NULL) return;
    if (d->length > 3) {
        node* b = Node();
        b->parent = d->parent;
        b->children[0] = d->children[2];
        b->children[1] = d->children[3];
        b->keys[0] = d->keys[2];
        b->length = 2;
        d->children[2]->parent = b;
        d->children[3]->parent = b;
        d->length = 2;
        d->children[2] = NULL;
        d->children[3] = NULL;

        node* p = d->parent;
        if (p != NULL) {
            p->children[p->length] = b;
            b->parent = p;
            p->length++;
            sort_son(p);
            update_keys(b);
            split_parent(p);
        }
        else {
            node* new_root = Node();
            new_root->children[0] = d;
            new_root->children[1] = b;
            new_root->keys[0] = d->keys[1];
            new_root->length = 2;
            d->parent = new_root;
            b->parent = new_root;
            root = new_root;
        }
    }
}


node* find_brother(node* v) {
    node* p = v->parent;
    if (p == NULL) {
        return NULL;
    }
    if (p->length == 1) {
        return NULL;
    }

    int pos;
    for (int i = 0; i < p->length; i++) {
        if (v == p->children[i]) {
            pos = i;
            break;
        }
    }

    if (pos == 0 || pos == 2) {
        return p->children[1];
    }
    else {
        return p->children[0];
    }
}

void delete_in_node(node* v) {
    node* p = v->parent;
    int pos = 4;

    for (int i = 0; i < p->length; i++) {
        if (v == p->children[i]) {
            pos = i;
            free(p->children[i]);
        }
        if (i > pos) {
            p->children[i - 1] = p->children[i];
        }
    }
    p->length--;
    update_keys(p->children[0]);
}

node* del(node* v, int x) {
    node* d = search(v, x);
    if (d == NULL) {
        return NULL;
    }

    if (d->keys[0] != x) return root;

    node* p = d->parent;

    if (p == NULL) {
        free(v);
        root = NULL;
    }
    else {
        if (p->length > 2) {
            delete_in_node(d);
            return root;
        }
        if (p->length == 2) {
            node* np = find_brother(p);
            if (np == NULL) {
                delete_in_node(d);
                p->children[0]->parent = NULL;
                root = p->children[0];
            }
            else {
                node* b = find_brother(d);
                np->children[np->length] = b;
                b->parent = np;
                np->length++;
                p->length = 1;
                p->keys[0] = x;
                sort_son(np);
                del(v, x);
                update_keys(b);
                split_parent(np);
                update_keys(b);
            }
        }
    }
    return root;
}

int get_height(node* v) {
    int h = 0;
    while (v->length != 1) {
        h++;
        v = v->children[0];
    }
    return h;
}


node* merge(node* l, node* r) {
    if (l == NULL) {
        return r;
    }
    if (r == NULL) {
        return l;
    }
    int h_right = get_height(r);
    int h_left = get_height(l);
    if (h_left == h_right) {
        node* new_root = (node*)malloc(sizeof(node));
        new_root->length = 2;
        new_root->children[0] = l;
        l->parent = new_root;
        new_root->children[1] = r;
        r->parent = new_root;
        update_keys(r);
        return new_root;
    }

    node* v;
    if (h_right > h_left) {
        v = r;
        while (abs(h_right - get_height(v)) > 1) {
            v = v->children[0];
        }
        r->children[r->length] = l;
        r->length++;
        l->parent = r;
        sort_son(r);
        update_keys(l);
        node* new_root = r;
        split_parent(r);
        return new_root;
    }
    else {
        v = l;
        while (abs(h_left - get_height(v)) > 1) {
            v = v->children[0];
        }
        l->children[l->length] = r;
        l->length++;
        r->parent = l;
        update_keys(r);
        node* new_root = l;
        split_parent(l);
        return new_root;
    }

}


split_retval split(node* d, int key) {
    node_list* l_list = (node_list*)malloc(sizeof(node_list));
    node_list* l_ptr = l_list;
    node_list* r_list = (node_list*)malloc(sizeof(node_list));
    node_list* r_ptr = r_list;

    while (d->length != 1) {
        if (d->length == 2) {
            if (key <= d->keys[0]) {
                r_ptr = list_add(r_ptr, d->children[1]);
                d = d->children[0];
            }
            else {
                l_ptr = list_add(l_ptr, d->children[0]);
                d = d->children[1];
            }
        }
        else if (key <= d->keys[0]) {
            r_ptr = list_add(r_ptr, d->children[1]);
            r_ptr = list_add(r_ptr, d->children[2]);
            d = d->children[0];
        }
        else if (key <= d->keys[1]) {
            l_ptr = list_add(l_ptr, d->children[0]);
            r_ptr = list_add(r_ptr, d->children[2]);
            d = d->children[1];
        }
        else {
            l_ptr = list_add(l_ptr, d->children[0]);
            l_ptr = list_add(l_ptr, d->children[1]);
            d = d->children[2];
        }
    }
    l_ptr = list_add(l_ptr, d);

    node* l = NULL;
    node* r = NULL;
    l_ptr = l_list->next;
    while (l_ptr != NULL) {
        l = merge(l, l_ptr->val);
        l_ptr = l_ptr->next;
    }
    while (r_ptr != r_list) {
        r = merge(r, r_ptr->val);
        r_ptr = r_ptr->prev;
    }

    split_retval ret;
    ret.left = l;
    ret.right = r;
    return ret;
}
