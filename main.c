#include "AvlTree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST_SIZE 10


typedef struct Cow {
    int id;
    char *name;
} Cow;

void swap(Cow **a, Cow **b) {
    Cow *temp = *a;
    *a = *b;
    *b = temp;
}

int getCowId(void *obj) {
    Cow *cow = (Cow *) obj;
    return cow->id;
}

int main() {
    Cow *arr[TEST_SIZE] = {NULL};
    for (int i = 0; i < TEST_SIZE; ++i) {
        Cow *pCow = (Cow *) malloc(sizeof(Cow));
        if (pCow != NULL) {
            pCow->id = i;
            pCow->name = (char *) malloc(sizeof(char) + 1);
            if (pCow->name != NULL) strcpy(pCow->name, "c");
            arr[i] = pCow;
        }
    }
    for (int j = 0; j < TEST_SIZE; ++j) {
        int r = rand() % (TEST_SIZE - j);
        swap(&arr[r], &arr[j]);
    }


    printf("stage0**************************************************************:\n");
    AvlTree *tree = createAvlTree(getCowId);
    printf("The size of the tree is: %d\n", size(tree));
    printAvlTree(tree);
    for (int k = 0; k < TEST_SIZE; ++k) {
        printf("stage%d**************************************************************:\n", k + 1);
        add(tree, arr[k]);
        printf("The size of the tree is: %d\n", size(tree));
        printAvlTree(tree);
        contains(tree, arr[k]) ? printf("contains %d\n", arr[k]->id) : printf("not contains %d\n", arr[k]->id);
    }
    for (int j = 0; j < TEST_SIZE; ++j) {
        int r = rand() % (TEST_SIZE - j);
        swap(&arr[r], &arr[j]);
    }
    for (int k = 0; k < TEST_SIZE; ++k) {
        printf("stage%d**************************************************************:\n", k + 1);
        delete(tree, arr[k]);
        printf("The size of the tree is: %d\n", size(tree));
        printAvlTree(tree);
        contains(tree, arr[k]) ? printf("contains %d\n", arr[k]->id) : printf("not contains %d\n", arr[k]->id);
    }

    isEmptyAvlTree(tree) ? printf("The tree is empty!!!\n") : printf("The tree is not empty!!!\n");
    freeAvlTree(tree);
    for (int i = 0; i < TEST_SIZE; ++i) {
        if (arr[i] != NULL) {
            free(arr[i]->name);
            free(arr[i]);
        }
    }
    return 0;
}

