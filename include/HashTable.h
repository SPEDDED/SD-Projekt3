#ifndef HASHTABLE_H
#define HASHTABLE_H


class HashTable {
protected:
    int size;
    int capacity;

    
    int hashFunction(int key) const {// Hashopwanie przez dzielenie modulo
        int hash = key % capacity;
        return hash < 0 ? hash + capacity : hash;
        // Jak klucz jest ujemny to dodawane jest capacity, co po prostu zmienia liczbę na dodatnią
    }


public:

    HashTable(int cap) : size(0), capacity(cap) {} //konstruktor

    virtual ~HashTable() {} //dekonstruktor z "virtual" bo jeszcze z tego będą dziedziczyć pozostałę klasy tablic


    //Funkcje virtualne zrobione dla pozostałych klas
    virtual bool insert(int key, int value) = 0; 
    virtual bool remove(int key) = 0;           
    virtual int* find(int key) = 0;
    virtual void display() = 0;
    virtual void clear() = 0;


    int getSize() const { return size; }
    int getCapacity() const { return capacity; }
};

#endif