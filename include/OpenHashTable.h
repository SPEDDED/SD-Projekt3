#ifndef OPENHASHTABLE_H
#define OPENHASHTABLE_H

#include "HashTable.h"



class OpenHashTable : public HashTable { //Dziedziczenie z HashTable

private:
    struct Entry {
        int key;
        int value;

        //ZMienne do informmowania, czy slot jest zajęty i czy został zwolniony
        bool isOccupied;
        bool isDeleted;

        Entry() : key(0), value(0), isOccupied(false), isDeleted(false) {} //Konstruktor struktury
    };

    Entry* table;
    void resizeAndRehash();

public:
    OpenHashTable(int cap); //konstruktor
    ~OpenHashTable() override; //dekonstruktor nadpisany z HashTable

    bool insert(int key, int value) override;
    bool remove(int key) override;
    int* find(int key) override;
    void display() override;
    void clear() override;
};

#endif