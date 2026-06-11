#ifndef HASHTABLEAVL_H
#define HASHTABLEAVL_H

#include "HashTable.h"

class HashTableAVL : public HashTable { 
private:

    struct AVLNode {
        int key;
        int value;
        int height; 


        AVLNode* left; //wskażnik na lawe dziecko
        AVLNode* right;//wskażnik na prawe dziecko

        AVLNode(int k, int v) : key(k), value(v), height(1), left(nullptr), right(nullptr) {} //konstruktor węzła
    };

    AVLNode** table;

    int height(AVLNode* node);
    int getBalance(AVLNode* node);


    AVLNode* rightRotate(AVLNode* y);
    AVLNode* leftRotate(AVLNode* x);
    AVLNode* insertNode(AVLNode* node, int key, int value, bool& success);
    AVLNode* minValueNode(AVLNode* node);
    AVLNode* removeNode(AVLNode* root, int key, bool& success);


    int* findNode(AVLNode* node, int key);
    void displayTree(AVLNode* node);
    void clearTree(AVLNode* node);

public:
    HashTableAVL(int cap); //konstruktor
    ~HashTableAVL() override;//dekonstruktor

    bool insert(int key, int value) override;
    bool remove(int key) override;
    int* find(int key) override;
    void display() override;
    void clear() override;
};

#endif
