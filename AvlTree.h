#ifndef AVLTREE_AVLTREE_H
#define AVLTREE_AVLTREE_H

#include <stdbool.h>
typedef int (*getValFunc)(void* obj);
typedef struct TreeNode {
    void* obj;
    int height;
    int balance;
    struct TreeNode *parent;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;
typedef struct AvlTree {
    TreeNode *root;
    getValFunc getVal;
} AvlTree;

AvlTree *createAvlTree(getValFunc getVal);

bool add(AvlTree *pTree, void* obj);

bool delete(AvlTree *pTree, void* obj);

bool contains(AvlTree *pTree, void* obj);

int size(AvlTree *tree);

bool isEmptyAvlTree(AvlTree *tree);

void printAvlTree(AvlTree *pTree);

void freeAvlTree(AvlTree* tree);

#endif //AVLTREE_AVLTREE_H