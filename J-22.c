#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<malloc.h>
#define NOTSTATED 2
#define DELETED 0
#define TRUE 1

typedef struct {
	char* str;
	int state;
} node;

node* table;

const int size = 524288;

void InitHashTable(void);
int HashFunc(const char* str, int step);
int Search(const char* str);
void Insert(const char* str);
void Delete(const char* str);
void DestroyHashTable(void);

void InitHashTable(void) {
	table = (node*)malloc(size * sizeof(node));
	if (table == NULL)
		return;
	for (int i = 0; i < size; i++) {
		table[i].str = NULL;
		table[i].state = NOTSTATED;
	}
}

int HashFunc(const char* str, int step) {
	int c0 = 31;
	double c1 = 0.5;
	double c2 = 0.5;
	int h1 = 0;
	for (int i = 0; str[i] != '\0'; i++)
	{
		h1 = c0 * h1 + str[i];
	}
	h1 = h1 % size;

	int h2 = (int)(h1 + c1 * step + c2 * step * step) % size;

	return h2;
}

int Search(const char* str) {
	int h = HashFunc(str, 0);
	int i = 0;
	while (table[h].str != NULL && i < size) {
		if (table[h].state!=DELETED && strcmp(table[h].str, str) == 0) {
			return 1;
		}
		i++;
		h = HashFunc(str, i);
	}
	return 0;
}

void Insert(const char* str) {
	int i = 0;
	int h = HashFunc(str, 0);
	int first_deleted = -1;
	while (table[h].str != NULL && i < size) {
		if (strcmp(table[h].str, str) == 0 && table[h].state !=DELETED) {
			return;
		}
		if (table[h].state == DELETED && first_deleted == -1) {
			first_deleted = h;
		}
		i++;
		h = HashFunc(str, i);
	}

	if (first_deleted != -1) {
		table[first_deleted].str = (char*)malloc((strlen(str) + 1) * sizeof(char));
		strcpy(table[first_deleted].str, str);
		table[first_deleted].state = NOTSTATED;
	}
	else {
		table[h].str = (char*)malloc((strlen(str) + 1) * sizeof(char));
		strcpy(table[h].str, str);
	}
}

void Delete(const char* str) {
	int h = HashFunc(str, 0);
	int i = 0;
	while (table[h].str != NULL && i < size) {
		if (strcmp(table[h].str, str) == 0 && table[h].state !=DELETED) {
			table[h].state = DELETED;
			return;
		}
		i++;
		h = HashFunc(str, i);
	}
}

void DestroyHashTable(void) {
	if (table == NULL)
		return;
	for (int i = 0; i < size; i++)
		free(table[i].str);
	free(table);
	table = NULL;
}

int main()
{
	InitHashTable();
	char command;
	char str[10000];
	while (fscanf(stdin, "%c", &command)!=EOF) {
		switch (command) {
		case 'a':
			scanf("%s", &str);
			Insert(str);
			break;
		case 'r':
			scanf("%s", &str);
			Delete(str);
			break;
		case 'f':
			scanf("%s", &str);
			if (Search(str)) {
				puts("yes");
			}
			else {
				puts("no");
			}
			break;
		}
	}
	DestroyHashTable();
}