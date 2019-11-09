#include "AvlTree.h"
#include <stdio.h>
#include <stdlib.h>

#define SPACE_FACTOR 10
#define MAX(a, b) (((a)>(b))?(a):(b))


AvlTree *createAvlTree(getValFunc getVal) {
    AvlTree *pTree = (AvlTree *) malloc(sizeof(AvlTree));
    if (pTree != NULL) {
        pTree->root = NULL;
        pTree->getVal = getVal;

    } else printf("OutOfMemory!\n");
    return pTree;
}

TreeNode *containsHelperAdd(AvlTree* pTree,TreeNode *node, TreeNode *parent, void* obj) {
    if (node == NULL) return parent;
    if (pTree->getVal(node->obj) == pTree->getVal(obj)) return node;
    if (pTree->getVal(node->obj) < pTree->getVal(obj)) return containsHelperAdd(pTree,node->right, node, obj);
    return containsHelperAdd(pTree,node->left, node, obj);
}

TreeNode *containsHelperDelete(AvlTree *pTree, TreeNode *node, void *obj) {
    if (node == NULL) return NULL;
    if (pTree->getVal(node->obj) == pTree->getVal(obj)) return node;
    if (pTree->getVal(node->obj) < pTree->getVal(obj)) return containsHelperDelete(pTree, node->right, obj);
    return containsHelperDelete(pTree, node->left, obj);
}

bool contains(AvlTree *pTree, void *obj) {
    return containsHelperDelete(pTree, pTree->root, obj) != NULL;
}

void heightUpdater(TreeNode *pNode) {
    while (pNode != NULL) {
        if (pNode->left == NULL && pNode->right == NULL) {
            pNode->height = 0;
        } else if (pNode->left == NULL) pNode->height = pNode->right->height + 1;
        else if (pNode->right == NULL) pNode->height = pNode->left->height + 1;
        else pNode->height = MAX(pNode->left->height, pNode->right->height) + 1;
        pNode = pNode->parent;
    }
}

void balanceUpdater(TreeNode *pNode) {
    while (pNode != NULL) {
        int left = (pNode->left == NULL) ? -1 : pNode->left->height,
                right = (pNode->right == NULL) ? -1 : pNode->right->height;
        pNode->balance = left - right;
        pNode = pNode->parent;
    }
}

void rotateLeft(AvlTree *pTree, TreeNode *pivot) {
    TreeNode *father = pivot->parent; // maybe null
    TreeNode *son = pivot->right;

    pivot->right = son->left;
    if (son->left != NULL) son->left->parent = pivot;

    son->left = pivot;
    pivot->parent = son;

    if (father == NULL) {
        son->parent = NULL;
        pTree->root = son;
    } else if (father->left == pivot) {
        son->parent = father;
        father->left = son;
    } else {
        son->parent = father;
        father->right = son;
    }
    heightUpdater(pivot);
    balanceUpdater(pivot);
}

void rotateRight(AvlTree *pTree, TreeNode *pivot) {
    TreeNode *father = pivot->parent; // maybe null
    TreeNode *son = pivot->left;

    pivot->left = son->right;
    if (son->right != NULL) son->right->parent = pivot;

    son->right = pivot;
    pivot->parent = son;

    if (father == NULL) {
        son->parent = NULL;
        pTree->root = son;
    } else if (father->right == pivot) {
        son->parent = father;
        father->right = son;
    } else {
        son->parent = father;
        father->left = son;
    }
    heightUpdater(pivot);
    balanceUpdater(pivot);

}

void violationChecker(AvlTree *pTree, TreeNode *pNode) {
    while (pNode != NULL) {
        if (pNode->balance == 2) { // left heavy
            if (pNode->left->balance == -1) rotateLeft(pTree, pNode->left);
            rotateRight(pTree, pNode);
        }
        if (pNode->balance == -2) { //right heavy
            if (pNode->right->balance == 1) rotateRight(pTree, pNode->right);
            rotateLeft(pTree, pNode);
        }
        pNode = pNode->parent;
    }
}


bool add(AvlTree *pTree, void *obj) {
    TreeNode *res = containsHelperAdd(pTree,pTree->root, NULL, obj);
    if (res != NULL && res->obj == obj) return false;
    TreeNode *newObj = (TreeNode *) malloc(sizeof(TreeNode));
    if (newObj != NULL) {
        newObj->obj = obj;
        newObj->balance = 0;
        newObj->height = 0;
        newObj->left = NULL;
        newObj->right = NULL;
        newObj->parent = res;

        if (res == NULL) {
            pTree->root = newObj;
        } else {
            if (res->obj > obj) res->left = newObj;
            else res->right = newObj;
            heightUpdater(newObj);
            balanceUpdater(newObj);
            violationChecker(pTree, newObj);
        }
        return true;
    } else {
        printf("OutOfMemory!\n");
        return false;
    }
}

TreeNode *successor(TreeNode *node) {
    TreeNode *cur = node->right;
    while (cur->left != NULL) cur = cur->left;
    return cur;
}

bool delete(AvlTree *pTree, void *obj) {
    TreeNode *toDel = containsHelperDelete(pTree,pTree->root, obj);
    if (toDel == NULL) return false;

    if (toDel->left == NULL && toDel->right == NULL) {
        TreeNode *parent = toDel->parent;
        if (parent == NULL) pTree->root = NULL;
        else if (parent->left == toDel) parent->left = NULL;
        else parent->right = NULL;
        heightUpdater(parent);
        balanceUpdater(parent);
        violationChecker(pTree, parent);
    } else if (toDel->left == NULL || toDel->right == NULL) {
        TreeNode *son = (toDel->left == NULL) ? toDel->right : toDel->left;
        TreeNode *parent = toDel->parent;
        if (parent == NULL) {
            pTree->root = son;
            son->parent = NULL;
        } else if (parent->left == toDel) parent->left = son;
        else parent->right = son;
        son->parent = parent;
        heightUpdater(son);
        balanceUpdater(son);
        violationChecker(pTree, son);
    } else {
        TreeNode *parent = toDel->parent;
        TreeNode *left = toDel->left;
        TreeNode *right = toDel->right;
        TreeNode *suc = successor(toDel);
        TreeNode *sucParent = suc->parent; // suc has one right son at most
        TreeNode *sucSon = suc->right;

        if (sucParent->left == suc) sucParent->left = sucSon;
        else sucParent->right = sucSon;
        if (sucSon != NULL) sucSon->parent = sucParent;

        if (parent == NULL) {
            pTree->root = suc;
            suc->parent = NULL;
        } else if (parent->left == toDel) parent->left = suc;
        else parent->right = suc;

        suc->parent = parent;
        suc->left = left;
        left->parent = suc;
        if (right != suc) {
            suc->right = right;
            right->parent = suc;
        } else suc->right = NULL;
        heightUpdater(suc);
        balanceUpdater(suc);
        violationChecker(pTree, suc);
    }
    free(toDel);
    return true;
}

int sizeHelper(TreeNode *node) {
    if (node == NULL) return 0;
    return sizeHelper(node->left) + sizeHelper(node->right) + 1;
}

int size(AvlTree *tree) {
    return sizeHelper(tree->root);
}

bool isEmptyAvlTree(AvlTree *tree) {
    return tree->root == NULL;
}

void printAvlTreeHelper(AvlTree* pTree,TreeNode *pNode, int space) {
    if (pNode == NULL) return;
    space += SPACE_FACTOR;
    printAvlTreeHelper(pTree,pNode->right, space);
    printf("\n");
    for (int i = 0; i < space; ++i) {
        printf(" ");
    }
    printf("%d\n", pTree->getVal(pNode->obj));
    printAvlTreeHelper(pTree,pNode->left, space);
}

void printAvlTree(AvlTree *pTree) {
    printAvlTreeHelper(pTree, pTree->root, 0);
}

void freeTreeNode(TreeNode *node) {
    if (node == NULL)
        return;
    TreeNode *toDel = node;
    freeTreeNode(node->left);
    freeTreeNode(node->right);
    free(toDel);
}

void freeAvlTree(AvlTree *tree) {
    freeTreeNode(tree->root);
    free(tree);
}


