
/*
 *  AVL_tree.cpp
 *  ENSF 694 Lab 5, exercise A
 *  Created by Mahmood Moussavi on 2024-05-22
 *  Completed by: Jack Shenfield
 *  Development Date: August 5th, 2025
 */


#include "AVL_tree.h"

AVLTree::AVLTree() : root(nullptr), cursor(nullptr){}

int AVLTree::height(const Node* N) {
    // Student must complete and if necessary change the return value of 
    // this function this function

    // return 0 if there are no children
    // return height if there are children
    return (N == nullptr) ? 0 : N->height;

}

int AVLTree::getBalance(Node* N) {
    // Student must complete and if necessary change the return value of 
    // this function this function


    if (N == nullptr) {
        return 0;
    }

    return(height(N->left) - height(N->right));
}


Node* AVLTree::rightRotate(Node* y) {
    // Student must complete and if necessary change the return value of 
    // this function this function

    // y is the unbalanced node. must pivot around y->right

    // y is the parent node
    // x is the pivot node
    // T2 is tree 2, the right subtree of x
    // these 3 nodes must be moved.

    // extract nodes
    Node* x = y->left;
    Node* T2 = x->right;

    // "rotate" parent node around
    x->right = y;
    y->left = T2;

    // if T2 exists, it's new parent is y.
    if (T2){
        T2->parent = y;
    }

    // x is the new parent
    x->parent = y->parent;
    // adjust y to be x's child
    y->parent = x;

    // re-calculate heights
    y->height = std::max(height(y->left), height(y->right)) + 1;
    x->height = std::max(height(x->left), height(x->right)) + 1;

    return x;
}

 Node* AVLTree::leftRotate(Node* x) {
     // Student must complete and if necessary change the return value of 
     // this function this function

     // see comments above. same logic just for left rotate.

    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    if (T2){
        T2->parent = x;
    }

    y->parent = x->parent;
    x->parent = y;

    x->height = std::max(height(x->left), height(x->right)) + 1;
    y->height = std::max(height(y->left), height(y->right)) + 1;

    return y;
 }

void AVLTree::insert(int key, Type value) {
    root = insert(root, key, value, nullptr);
}

// Recursive function
 Node* AVLTree::insert(Node* node, int key, Type value, Node* parent) {
    // Student must complete and if necessary change the return value of 
    // this function this function

    // base case
    if (node == nullptr) // insert where the current node points to nullptr
        return new Node(key, value, parent);


    if (key < node->data.key) // root node is less than current node key, recursively call
        node->left = insert(node->left, key, value, node);
    else if (key > node->data.key) // if it is more than current node key, recursively call back
        node->right = insert(node->right, key, value, node);
    else // key = node->data.key this is a duplicate, and we do not insert
        return node;


    node->height = 1 + std::max(height(node->left), height(node->right)); // update height at current node
    int balance = getBalance(node); // calculate balance where node is being inserted


    // Rotation may be required
    // LL
    if (balance > 1 && key < node->left->data.key){
        return rightRotate(node);
    }
    // RR
    if (balance < -1 && key > node->right->data.key){
        return leftRotate(node);
    }
    // LR
    if (balance > 1 && key > node->left->data.key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    // RL
    if (balance < -1 && key < node->right->data.key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
 }

// Recursive function
void AVLTree::inorder(const Node* root) {
    // Student must complete this function

    if (!root){ // IF DNE, return
        return;
    }

    // recursive call/print order for inorder
    inorder(root->left);
    std::cout << "(" << root->data.key << " " << root->data.value << ") ";
    inorder(root->right);

}

// Recursive function
void AVLTree::preorder(const Node* root) {
    // Student must complete this function

    if (!root){ // if DNE, return
        return;
    }

    // recursive call/print order for preorder
    std::cout << "(" << root->data.key << " " << root->data.value << ") ";
    preorder(root->left);
    preorder(root->right);

}

// Recursive function
void AVLTree::postorder(const Node* root) {
    // Student must complete this function

    // base case, if not root 
    if (!root){
        return;
    }

    // recursive call/print order for postorder
    postorder(root->left);
    postorder(root->right);
    std::cout << "(" << root->data.key << " " << root->data.value << ") ";

}

const Node* AVLTree::getRoot(){
    return root;
}

void AVLTree::find(int key) {
    go_to_root();
    if(root != nullptr)
        find(root, key);
    else
        std::cout << "It seems that tree is empty, and key not found." << std::endl;
}

// Recursive funtion
void AVLTree::find(Node* root, int key){
    // Student must complete this function

    if (!root) { // If root DNE, print root not found
        cursor = nullptr;
        std::cout << "Key " << key << " NOT found...\n";
        return;
    }

    if (key == root->data.key) { // if found, print key and value
        cursor = root;
        std::cout << "Key " << key << " found with value: " << root->data.value << "\n";
    }

    else if (key < root->data.key){ // if the key is less than current node, recursively call left (to lesser values)
        find(root->left, key);
    }
    else{ // if the key is greater than current node, recursively call right (to greater values).
        find(root->right, key);
    }


}

AVLTree::AVLTree(const AVLTree& other) : root(nullptr), cursor(nullptr) {
    root = copy(other.root, nullptr);
    cursor = root;
}

AVLTree::~AVLTree() {
    destroy(root);
}

AVLTree& AVLTree::operator=(const AVLTree& other) {
    if (this == &other) return *this;
    destroy(root);
    root = copy(other.root, nullptr);
    cursor = root;
    return *this;
}

// Recursive funtion
Node* AVLTree::copy(Node* node, Node* parent) {
    // Student must complete and if necessary change the return value of this function this function

    if (node == nullptr) return nullptr; // if node DNE, return nullptr

    Node* newNode = new Node(node->data.key, node->data.value, parent); // the node
    newNode->left = copy(node->left, newNode); // copy left sub-tree
    newNode->right = copy(node->right, newNode); // recurisvely call to copy right sub-tree

    newNode->height = node->height; // calculate new heights

    return newNode;

}

// Recusive function
void AVLTree::destroy(Node* node) {
    if (node) {
        destroy(node->left);
        destroy(node->right);
        delete node;
    }
    // Student must complete this function
}

const int& AVLTree::cursor_key() const{
    if (cursor != nullptr)
        return cursor->data.key;
    else{
        std::cout << "looks like tree is empty, as cursor == Zero.\n";
        exit(1);
    }
}

const Type& AVLTree::cursor_datum() const{
    if (cursor != nullptr)
        return cursor->data.value;
    else{
        std::cout << "looks like tree is empty, as cursor == Zero.\n";
        exit(1);
    }
}

int AVLTree::cursor_ok() const{
    if(cursor == nullptr)
        return 0;
    return 1;
}

void AVLTree::go_to_root(){
    if(!root) cursor = root;
    cursor = nullptr;
}

