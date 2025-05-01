#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * treeMap = (TreeMap *)malloc(sizeof(TreeMap));
    if (treeMap == NULL) return NULL;
    treeMap->root = NULL;
    treeMap->current = NULL;
    treeMap->lower_than = lower_than;
    return treeMap;
    //return NULL;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if (tree == NULL) return;

    TreeNode * newNode = createTreeNode(key, value);
    if (newNode == NULL) return;

    if (tree->root == NULL) {
        tree->root = newNode;
        tree->current = newNode;
        return;
    }

    TreeNode * current = tree->root;
    TreeNode * parent = NULL;

    while (current != NULL) {
        parent = current;

        // Si la clave ya existe, no insertamos
        if (!tree->lower_than(key, current->pair->key) &&
            !tree->lower_than(current->pair->key, key)) {
            free(newNode->pair);
            free(newNode);
            return;
        }

        if (tree->lower_than(key, current->pair->key)) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    newNode->parent = parent;
    if (tree->lower_than(key, parent->pair->key)) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }

    tree->current = newNode;
}


TreeNode * minimum(TreeNode * x){
    if (x == NULL) return NULL;
    while (x->left != NULL) {
        x = x->left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if (tree == NULL || node == NULL) return;

    if (node->left == NULL && node->right == NULL) {
        if (node->parent != NULL) {
            if (node->parent->left == node) {
                node->parent->left = NULL; 
            } else {
                node->parent->right = NULL; 
            }
        } else {
            tree->root = NULL;
        }
        free(node->pair); 
        free(node); 
    }
    else if (node->left == NULL || node->right == NULL) {
        TreeNode* child = (node->left != NULL) ? node->left : node->right;

        if (node->parent != NULL) {
            if (node->parent->left == node) {
                node->parent->left = child; 
            } else {
                node->parent->right = child; 
            }
        } else {
            tree->root = child;
        }
        child->parent = node->parent; 
        free(node->pair); 
        free(node); 
    }
    else {
        TreeNode* minNode = minimum(node->right);
        node->pair->key = minNode->pair->key;
        node->pair->value = minNode->pair->value;
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}


Pair * searchTreeMap(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode * current = tree->root;
    while (current != NULL) {
        if (tree->lower_than(key, current->pair->key)) {
            current = current->left;
        } else if (tree->lower_than(current->pair->key, key)) {
            current = current->right;
        } else {
            tree->current = current;
            return current->pair;
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->root == NULL) return NULL;

    TreeNode * current = tree->root;
    while (current->left != NULL) {
        current = current->left;
    }
    tree->current = current;
    return current->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    return NULL;
}
