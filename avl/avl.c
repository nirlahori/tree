#include "avl.h"
#include <stdlib.h>

static void rotate_left(avl_tree* tree, avl_node* head){
    avl_node* newhead = head->right;
    avl_node* oldleft = newhead->left;

    newhead->left = head;
    newhead->parent = head->parent;

    if(!head->parent){
        // Root case
        tree->root = newhead;
    }
    else{
        if(head == head->parent->left){
            head->parent->left = newhead;
        }
        else{
            head->parent->right = newhead;
        }
    }

    head->parent = newhead;
    head->right = oldleft;
    if(oldleft){
        oldleft->parent = head;
    }
}


static void rotate_right(avl_tree* tree, avl_node* head){
    avl_node* newhead = head->left;
    avl_node* oldright = newhead->right;

    newhead->right = head;
    newhead->parent = head->parent;

    if(!head->parent){
        // Root case
        tree->root = newhead;
    }
    else{
        if(head == head->parent->left){
            head->parent->left = newhead;
        }
        else{
            head->parent->right = newhead;
        }
    }

    head->parent = newhead;
    head->left = oldright;
    if(oldright){
        oldright->parent = head;
    }
}

int node_height(avl_node* node){

    int lh = 0, rh = 0;
    if(node->left){
        lh = node_height(node->left);
    }
    if(node->right){
        rh = node_height(node->right);
    }
    if(lh > rh){
        return lh + 1;
    }
    else{
        return rh + 1;
    }
}

static void update_node_height(avl_node* *node){

    int h = node_height(*node);
    (*node)->height = h;
}

static void balance(avl_tree* tree, avl_node* node){

    int lh = 0, rh = 0;
    if(node->left){
        lh = node_height(node->left);
    }
    if(node->right){
        rh = node_height(node->right);
    }

    if(lh > rh){
        avl_node* left_tree = node->left;
        if(left_tree->right){
            rh = node_height(left_tree->right);
        }
        else{
            rh = 0;
        }
        if(rh > left_tree->left->height){
            rotate_left(tree, left_tree);
            update_node_height(&node->left->left);
            update_node_height(&node->left);
        }
        rotate_right(tree, node);
    }
    else{
        avl_node* right_tree = node->right;
        if(right_tree->left){
            lh = node_height(right_tree->left);
        }
        else {
            lh = 0;
        }
        if(lh > right_tree->right->height){
            rotate_right(tree, right_tree);
            update_node_height(&node->right->right);
            update_node_height(&node->right);
        }
        rotate_left(tree, node);
    }
}

static int subtree_height(avl_node* n, int count){

    static int max = 0;
    static int save = 0;

    if(!n){
        max = save;
        return 0;
    }

    if(!n->left && !n->right){
        if(count > max){
            max = save;
            return count;
        }
    }
    else if(!n->left){
        return subtree_height(n->right, count + 1);
    }
    else if(!n->right){
        return subtree_height(n->left, count + 1);
    }
    else{
        int left_height = subtree_height(n->left, count + 1);
        int right_height = subtree_height(n->right, count + 1);
        return (left_height > right_height) ? left_height : right_height;
    }
    max = save;
    return 0;
}

int height(avl_tree* tree){
    return node_height(tree->root);
}

static void update_heights(avl_node* *node){

    avl_node* curr = *node;
    int h = 0;
    while(curr){
        h = node_height(curr);
        curr->height = h;
        curr = curr->parent;
    }
}

static avl_node* find_unbalanced_node(avl_node* node){

    avl_node* curr = node->parent;
    int lh = 0, rh = 0;
    while(curr){
        if(curr->left){
            lh = node_height(curr->left);
        }
        if(curr->right){
            rh = node_height(curr->right);
        }
        if(lh > rh && lh - rh >= 2){
            return curr;
        }
        else if(rh > lh && rh - lh >= 2){
            return curr;
        }
        curr = curr->parent;
    }
    return curr;
}




static void avl_insert(avl_tree* tree, avl_node* node){

    avl_node* curr = tree->root;
    avl_node* par = NULL;
    while(curr){
        par = curr;
        if(node->key < curr->key){
            curr = curr->left;
        }
        else{
            curr = curr->right;
        }
    }
    node->parent = par;
    if(!par){
        tree->root = node;
    }
    else if(node->key < par->key){
        par->left = node;
    }
    else{
        par->right = node;
    }

    update_heights(&node);
    avl_node* faulty_node = find_unbalanced_node(node);
    if(faulty_node){
        balance(tree, faulty_node);
        update_node_height(&faulty_node);
    }
}

static void avl_add_recursive(avl_node* curr, avl_node* *par, avl_node* node){

    if(curr){
        if(node->key < curr->key){
            avl_add_recursive(curr->left, &curr, node);
        }
        else{
            avl_add_recursive(curr->right, &curr, node);
        }
    }
    else{
        if(node->key < (*par)->key){
            (*par)->left = node;
            node->parent = *par;
        }
        else{
            (*par)->right = node;
            node->parent = *par;
        }
    }
}

static void avl_insert_recursive(avl_tree* tree, avl_node* node){

    if(!tree->root){
        tree->root = node;
        tree->root->height = 1;
    }
    else{
        node->height = 0;
        avl_add_recursive(tree->root, NULL, node);
    }
    update_heights(&node);
    avl_node* faulty_node = find_unbalanced_node(node);
    if(faulty_node){
        balance(tree, faulty_node);
        update_node_height(&faulty_node);
    }
}


void insert(avl_tree* tree, avl_node* node){

    avl_insert_recursive(tree, node);
    update_node_height(&tree->root);
//    avl_insert(tree, node);
}

avl_node* avl_minimum(avl_node* node){

    avl_node* curr = node;
    while(curr->left){
        curr = curr->left;
    }
    return curr;
}

avl_node *avl_successor(avl_node* node){

    if(!node){
        return NULL;
    }

    if(node->right){
        return avl_minimum(node->right);
    }

    avl_node* curr = node;
    avl_node* par = curr->parent;
    while(par && curr == par->right){
        curr = par;
        par = par->parent;
    }
    return par;
}
