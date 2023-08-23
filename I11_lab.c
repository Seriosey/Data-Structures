#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct tree_t {
	struct tree_t* parent;
	struct tree_t* left;
	struct tree_t* mid;
	struct tree_t* right;
	int height;
	int key1, key2;
	bool full;
} tree_t;

tree_t* InitNode(tree_t* brother, int key, tree_t* pointer);
tree_t* SearchNode(tree_t* root, int key);
tree_t* SplitParent(tree_t* son, int key);
tree_t* Insert(tree_t* root, int key, tree_t* pointer);
bool FindNode(tree_t* tree, int key);
void TreeDestroy(tree_t* root);
tree_t* Remove(tree_t* root, int key);
void PrintTree(tree_t* t, int offset);
tree_t* SplitTree(tree_t* tree, int key, tree_t** pLeft, tree_t** pRight);
tree_t* MergeTree(tree_t* left, tree_t* right);

tree_t* InitNode(tree_t* brother, int key, tree_t* pointer) {
	tree_t* res = (tree_t*)malloc(sizeof(tree_t));
	if (res) {
		res->full = false;
		res->key1 = key;
		res->left = pointer;
		if (brother) {
			if ((res->height = brother->height))
				res->left->parent = res;
			res->parent = brother->parent;
			res->right = brother->right;
			brother->right = res;
		}
		else {
			res->height = 0;
			res->parent = NULL;
			res->right = NULL;
		}
	}
	return res;
}

tree_t* SearchNode(tree_t* tree, int key) {
	if (!tree)
		return NULL;
	tree_t* cur = tree;
	while (cur->height) {
		if (key <= cur->key1)
			cur = cur->left;
		else if (cur->full && key > cur->key2)
			cur = cur->right;
		else
			cur = cur->mid;
	}
	return cur;
}

bool FindNode(tree_t* tree, int key) {
	if (!tree)
		return false;
	tree_t* leaf = SearchNode(tree, key);
	if (key == leaf->key1 || (leaf->full && key == leaf->key2))
		return true;
	return false;
}

tree_t* Root(tree_t* node) {
	tree_t* cur = node;
	while (cur->parent)
		cur = cur->parent;
	return cur;
}

void LeafInsert(tree_t* node, int key, tree_t* pointer) {
	if (key > node->key1) {
		node->key2 = key;
		node->mid = pointer;
	}
	else {
		node->key2 = node->key1;
		node->mid = node->left;
		node->key1 = key;
		node->left = pointer;
	}
}

tree_t* Insert(tree_t* tree, int key, tree_t* pointer) {
	if (!tree)
		return InitNode(NULL, key, pointer);
	tree_t* leaf = SearchNode(tree, key);
	if (key == leaf->key1 || (leaf->full && key == leaf->key2))
		return tree;
	if (!leaf->full) {
		LeafInsert(leaf, key, pointer);
		leaf->full = true;
		return tree;
	}
	else {
		if (key < leaf->key1) {
			tree_t* next = InitNode(leaf, leaf->key2, leaf->mid);
			if (!next) {
				TreeDestroy(tree);
				return NULL;
			}
			LeafInsert(leaf, key, pointer);
			return SplitParent(leaf, leaf->key2);
		}
		else if (key < leaf->key2) {
			tree_t* next = InitNode(leaf, leaf->key2, leaf->mid);
			if (!next) {
				TreeDestroy(tree);
				return NULL;
			}
			LeafInsert(leaf, key, pointer);
			return SplitParent(leaf, key);
		}
		else {
			tree_t* next = InitNode(leaf, key, pointer);
			if (!next) {
				TreeDestroy(tree);
				return NULL;
			}
			return SplitParent(leaf, leaf->key2);
		}
	}
}

tree_t* SplitParent(tree_t* son, int key) {
	tree_t* node = son->parent;
	if (!node) {
		node = InitNode(NULL, key, son);
		if (!node) {
			TreeDestroy(son->right);
			TreeDestroy(son);
			return NULL;
		}
		node->height = son->height + 1;
		node->mid = son->right;
		son->right = NULL;
		son->parent = node;
		node->mid->parent = node;
		return node;
	}
	if (!node->full) {
		if (key > node->key1) {
			node->key2 = key;
			node->right = son->right;
			node->right->parent = node;
			son->right = NULL;
		}
		else {
			node->key2 = node->key1;
			node->right = node->mid;
			node->key1 = key;
			node->mid = son->right;
			node->mid->parent = node;
			son->right = NULL;
		}
		node->full = true;
	}
	else {
		if (key < node->key1) {
			tree_t* next = InitNode(node, node->key2, node->mid);
			if (!next) {
				TreeDestroy(son->right);
				TreeDestroy(Root(node));
				return NULL;
			}
			node->full = false;
			node->key2 = 0;
			next->mid = next->right;
			next->right = NULL;
			next->mid->parent = next;
			int old_key = node->key1;
			node->key1 = key;
			node->mid = son->right;
			son->right = NULL;
			return SplitParent(node, old_key);
		}
		else if (key < node->key2) {
			tree_t* next = InitNode(node, node->key2, son->right);
			if (!next) {
				TreeDestroy(son->right);
				TreeDestroy(Root(node));
				return NULL;
			}
			node->full = false;
			node->key2 = 0;
			next->mid = next->right;
			next->mid->parent = next;
			next->right = NULL;
			son->right = NULL;
			return SplitParent(node, key);
		}
		else {
			tree_t* next = InitNode(node, key, node->right);
			if (!next) {
				TreeDestroy(son->right);
				TreeDestroy(Root(node));
				return NULL;
			}
			node->full = false;
			int old_key = node->key2;
			node->key2 = 0;
			next->mid = son->right;
			next->right = NULL;
			next->mid->parent = next;
			son->right = NULL;
			return SplitParent(node, old_key);
		}
	}
	return Root(node);
}

void TreeDestroy(tree_t* root) {
	if (root) {
		if (root->height) {
			TreeDestroy(root->left);
			TreeDestroy(root->mid);
			if (root->full)
				TreeDestroy(root->right);
		}
		free(root);
	}
}

void PrintTree(tree_t* tree, int offset) {
	if (!tree)
		return;
	if (!tree->height) {
		if (offset > 0)
			for (int i = 0; i < offset; i++)
				printf(" ");
		printf("[%2d", tree->key1);
		if (tree->full)
			printf(", %2d", tree->key2);
		printf("]\n");
	}
	else {
		PrintTree(tree->left, offset + 2);
		if (!tree->full) {
			if (offset > 0)
				for (int i = 0; i < offset; i++)
					printf(" ");
			printf("%2d\n", tree->key1);
			PrintTree(tree->mid, offset + 2);
		}
		else {
			for (int i = 0; i < offset; i++)
				printf(" ");
			printf("%2d\n", tree->key1);
			PrintTree(tree->mid, offset + 2);
			for (int i = 0; i < offset; i++)
				printf(" ");
			printf("%2d\n", tree->key2);
			PrintTree(tree->right, offset + 2);
		}
	}
}

tree_t* MergeTree(tree_t* left, tree_t* right) {
	if (!left)
		return right;
	if (!right)
		return left;
	if (left->height == right->height) {
		tree_t* leftParent = left->parent;
		tree_t* rightParent = right->parent;
		int lmax;
		tree_t* cur = left;
		while (cur->height)
			cur = (!cur->full) ? cur->mid : cur->right;
		lmax = (!cur->full) ? cur->key1 : cur->key2;
		if (leftParent) {
			right->parent = leftParent;
			if (!leftParent->full) {
				leftParent->key2 = lmax;
				leftParent->right = right;
				leftParent->full = true;
				return Root(leftParent);
			}
			else {
				tree_t* next = InitNode(leftParent, lmax, left);
				if (!next)
					return NULL;
				next->mid = right;
				right->parent = next;
				next->right = NULL;
				leftParent->full = false;
				return SplitParent(leftParent, leftParent->key2);
			}
		}
		else if (rightParent) {
			left->parent = rightParent;
			if (!rightParent->full) {
				rightParent->key2 = rightParent->key1;
				rightParent->right = rightParent->mid;
				rightParent->mid = rightParent->left;

				rightParent->key1 = lmax;
				rightParent->left = left;
				rightParent->full = true;
				return Root(rightParent);
			}
			else {
				tree_t* next = InitNode(rightParent, rightParent->key2, rightParent->mid);
				if (!next)
					return NULL;
				rightParent->full = false;
				next->mid = next->right;
				next->mid->parent = next;
				next->right = NULL;
				int old_key = rightParent->key1;
				rightParent->mid = rightParent->left;
				rightParent->left = left;
				rightParent->key1 = lmax;
				return SplitParent(rightParent, old_key);
			}
		}
		else {
			tree_t* tree = InitNode(NULL, lmax, left);
			if (!tree)
				return NULL;
			tree->height = left->height + 1;
			tree->mid = right;
			left->parent = tree;
			right->parent = tree;
			return tree;
		}
	}
	if (left->height < right->height)
		return MergeTree(left, right->left);
	else
		return MergeTree((!left->full) ? left->mid : left->right, right);
}

tree_t* Extract(tree_t** ptr) {
	tree_t* res = *ptr;
	*ptr = NULL;
	res->parent = NULL;
	return res;
}

tree_t* SplitTree(tree_t* tree, int key, tree_t** pLeft, tree_t** pRight) {
	if (tree) {
		if (tree->height) {
			tree_t* left = Extract(&tree->left);
			tree_t* mid = Extract(&tree->mid);
			tree_t* right = NULL;
			if (tree->full)
				right = Extract(&tree->right);
			if (key <= tree->key1) {
				if (right)
					*pRight = MergeTree(right, *pRight);
				*pRight = MergeTree(mid, *pRight);
				free(tree);
				return SplitTree(left, key, pLeft, pRight);
			}
			*pLeft = MergeTree(*pLeft, left);
			if (tree->full && key > tree->key2) {
				*pLeft = MergeTree(*pLeft, mid);
				free(tree);
				return SplitTree(right, key, pLeft, pRight);
			}
			if (right)
				*pRight = MergeTree(right, *pRight);
			free(tree);
			return SplitTree(mid, key, pLeft, pRight);
		}
		if (key < tree->key1) {
			*pRight = Insert(*pRight, tree->key1, tree->left);
			if (tree->full)
				*pRight = Insert(*pRight, tree->key2, tree->mid);
			free(tree);
			return NULL;
		}
		if (key == tree->key1) {
			if (tree->full)
				*pRight = Insert(*pRight, tree->key2, tree->mid);
			tree->parent = NULL;
			tree->key2 = 0;
			tree->mid = NULL;
			return tree;
		}
		*pLeft = Insert(*pLeft, tree->key1, tree->left);
		if (tree->full && key < tree->key2) {
			*pRight = Insert(*pRight, tree->key2, tree->mid);
			free(tree);
			return NULL;
		}
		if (tree->full && key == tree->key2) {
			tree->parent = NULL;
			tree->key1 = key;
			tree->left = tree->mid;
			tree->key2 = 0;
			tree->mid = NULL;
			tree->full = false;
			return tree;
		}
		if (tree->full)
			*pLeft = Insert(*pLeft, tree->key2, tree->mid);
		free(tree);
		return NULL;
	}
	return NULL;
}

tree_t* Remove(tree_t* tree, int key) {
	tree_t* left = NULL;
	tree_t* right = NULL;
	if (!FindNode(tree, key))
		return tree;
	tree_t* m = SplitTree(tree, key, &left, &right);
	free(m);
	return MergeTree(left, right);
}

int Solution(FILE* streamIn, FILE* streamOut) {
	char lineBuffer[16] = " ";
	char action;
	int number;
	tree_t* t = NULL;
	while (fgets(lineBuffer, 16, streamIn)) {
		sscanf(lineBuffer, "%c%i", &action, &number);
		switch (action) {
		case 'a':
			t = Insert(t, number, NULL);
			break;
		case 'r':
			t = Remove(t, number);
			break;
		case 'f':
			if (FindNode(t, number))
				fprintf(streamOut, "yes\n");
			else
				fprintf(streamOut, "no\n");
			break;
		default:
			TreeDestroy(t);
			return 0;
		}
	}
	TreeDestroy(t);
	return 0;
}

int main(void) {
	Solution(stdin, stdout);
	return 0;
}