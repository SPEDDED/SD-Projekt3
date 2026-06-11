#include "HashTableAVL.h"
#include <iostream>
#include <algorithm>

HashTableAVL::HashTableAVL(int cap) : HashTable(cap) { //konstruktor z dziedziczeniem
    table = new AVLNode*[capacity];
    for (int i = 0; i < capacity; ++i) table[i] = nullptr; //inicjalizacja wskaźników jako puste (nullptr)
}

HashTableAVL::~HashTableAVL() { 
    clear();
    delete[] table;
}





int HashTableAVL::height(AVLNode* node) {
    return node == nullptr ? 0 : node->height; //jak węzeł jest pusty to wartość jest 0, inaczej zwracana jest wysokość
}

int HashTableAVL::getBalance(AVLNode* node) {
    return node == nullptr ? 0 : height(node->left) - height(node->right); //jak węzeł jest pusty to wartość jest 0, inaczej zwracana jest różnica wysokości
}



HashTableAVL::AVLNode* HashTableAVL::rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;


    x->right = y; // y staje się prawym dzieckiem x
    y->left = T2; // T2 staje się lewym dzieckiem y
    y->height = std::max(height(y->left), height(y->right)) + 1;
    x->height = std::max(height(x->left), height(x->right)) + 1;
    return x;
}

HashTableAVL::AVLNode* HashTableAVL::leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x; // x staje się lewym dzieckiem y
    x->right = T2; // T2 staje się prawym dzieckiem x
    x->height = std::max(height(x->left), height(x->right)) + 1;
    y->height = std::max(height(y->left), height(y->right)) + 1;
    return y;
}



HashTableAVL::AVLNode* HashTableAVL::insertNode(AVLNode* node, int key, int value, bool& success) {
    if (node == nullptr) { //jak węzeł jest pusty to tworzona jest nowa instancja AVLNode
        success = true;
        return new AVLNode(key, value);
    }

    if (key < node->key) node->left = insertNode(node->left, key, value, success);
    else if (key > node->key) node->right = insertNode(node->right, key, value, success);
    else {
        node->value = value; // jeśli klucz już istnieje to aktualizowana jest wartość
        success = false;
        return node;
    }

    node->height = 1 + std::max(height(node->left), height(node->right)); //aktualizacja wysokości węzła
    int balance = getBalance(node);


        //Rozpatrywanie przypadków przeciążeń i rotacji
    if (balance > 1 && key < node->left->key) return rightRotate(node);
    if (balance < -1 && key > node->right->key) return leftRotate(node);

    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}



bool HashTableAVL::insert(int key, int value) {
    int idx = hashFunction(key); //obliczanie indeksu za pomocą funkcji hashującej

    bool success = false;
    table[idx] = insertNode(table[idx], key, value, success);
    if (success) size++;

    return true;
}



HashTableAVL::AVLNode* HashTableAVL::minValueNode(AVLNode* node) {

    AVLNode* current = node;
    while (current->left != nullptr) current = current->left; //usuwanie w lewo (od najmniejszego klucza)

    return current;
}



HashTableAVL::AVLNode* HashTableAVL::removeNode(AVLNode* root, int key, bool& success) {

    if (root == nullptr) return root; //jeżeli węzeł jest pusty to zwracany jest pusty wskaźnik

    if (key < root->key) root->left = removeNode(root->left, key, success);
    else if (key > root->key) root->right = removeNode(root->right, key, success);

    else {
        success = true;

        if ((root->left == nullptr) || (root->right == nullptr)) { //jak węzeł ma jedno dziecko lub nie ma dzieci
            AVLNode* temp = root->left ? root->left : root->right;

            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            } else *root = *temp;
            delete temp;

        } 
        else { // jak ma więcej niż jedno dziecko

            AVLNode* temp = minValueNode(root->right);
            root->key = temp->key;
            root->value = temp->value;
            root->right = removeNode(root->right, temp->key, success);
        }
    }

    if (root == nullptr) return root;
    root->height = 1 + std::max(height(root->left), height(root->right));
    int balance = getBalance(root);


    // Balansowanie drzewa po usunięciu węzła

    //Rozpatrywanie przypadków przeciążeń i rotacji
    if (balance > 1 && getBalance(root->left) >= 0) return rightRotate(root);

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0) return leftRotate(root);

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }


    return root;
}

bool HashTableAVL::remove(int key) {
    int idx = hashFunction(key);

    bool success = false;
    table[idx] = removeNode(table[idx], key, success);
    if (success) size--;

    return success;
}

int* HashTableAVL::findNode(AVLNode* node, int key) {

    if (node == nullptr) return nullptr;
    if (node->key == key) return &(node->value); //jeśli klucz jest znaleziony to zwracany jest wskaźnik na wartość
    if (key < node->key) return findNode(node->left, key); //Jak klucz jest mniejszy to przeszukiwane jest lewe poddrzewo

    return findNode(node->right, key);
}

int* HashTableAVL::find(int key) {
    int idx = hashFunction(key);

    return findNode(table[idx], key);
}

void HashTableAVL::displayTree(AVLNode* node) {

    if (node != nullptr) {
        std::cout << " (" << node->key << "->" << node->value << ")";
        displayTree(node->left);
        displayTree(node->right);
    }
}

void HashTableAVL::display() {
    for (int i = 0; i < capacity; ++i) {

        std::cout << "[" << i << "]:";
        displayTree(table[i]);
        std::cout << "\n";
    }
}

void HashTableAVL::clearTree(AVLNode* node) {

    if (node != nullptr) {

        clearTree(node->left);
        clearTree(node->right);


        delete node;
    }
}

void HashTableAVL::clear() {
    for (int i = 0; i < capacity; ++i) {

        clearTree(table[i]);
        table[i] = nullptr;
    }
    
    size = 0;
}
