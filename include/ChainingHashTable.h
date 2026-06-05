#ifndef CHAININGHASHTABLE_H
#define CHAININGHASHTABLE_H

#include "HashTable.h"



class ChainingHashTable : public HashTable {
private:

    struct Node {
        int key;
        int value;
        Node* next;


        Node(int k, int v) : key(k), value(v), next(nullptr) {} //konstruktor węzłą
    };

    Node** table;

public:

    ChainingHashTable(int cap); //Konstruktor tworzący tablicę wskaźników na węzły
    ~ChainingHashTable() override; //Destruktor

    bool insert(int key, int value) override;
    bool remove(int key) override;
    int* find(int key) override;
    void display() override;
    void clear() override;
};

#endif