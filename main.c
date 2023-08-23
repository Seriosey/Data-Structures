#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "AVLTree.h"
#include <malloc.h>
#define BUFFER_SIZE 16

int main() {
	char command;
	int value;
	char buffer[BUFFER_SIZE] = { 0 };
	char letter;
	node* ROOT = NULL;
	while (fgets(buffer, BUFFER_SIZE, stdin)) {
		sscanf_s(buffer, "%c %i", &letter, &value);
		switch (letter) {
		case 'a':
			ROOT = insertel(ROOT, value);
			break;
		case 'r':
			ROOT = deletel(ROOT, value);
			break;
		case 'f':
			if (findel(ROOT, value))
				puts("yes");
			else
				puts("no");
			break;

		}
	}

	return 0;
}