#include "ChainingHashTable.h"
#include <iostream>


ChainingHashTable::ChainingHashTable(int cap) : HashTable(cap) { //Konstruktor tablicy wskaźników
    table = new Node*[capacity];
    for (int i = 0; i < capacity; ++i) table[i] = nullptr;
}

ChainingHashTable::~ChainingHashTable() { //Destruktor
    clear();
    delete[] table;
}



bool ChainingHashTable::insert(int key, int value) { 

    int idx = hashFunction(key); //Obliczanie początkowego indeksu
    Node* curr = table[idx]; //wskaźnik na pierwszy węzeł w łańcuchu

    while (curr != nullptr) { //przeszukanie całego łąńcucha

        if (curr->key == key) {
            curr->value = value; //Aktualizacja wartości jeśli klucz istnieje
            return true;
        }
        curr = curr->next; // Przejście do następnego węzła
    }

    Node* newNode = new Node(key, value); //Jeśli klucz nie został znaleziony to tworzony jest nowy węzeł
    newNode->next = table[idx];
    table[idx] = newNode;

    size++;
    return true;
}



bool ChainingHashTable::remove(int key) {

    int idx = hashFunction(key); //Obliczanie początkowego indeksu
    Node* curr = table[idx]; //wskaźnik na pierwszy węzeł w łańcuchu
    Node* prev = nullptr; //wskaźnik na poprzedni węzeł, potrzebny do usuwania z łańcucha


    while (curr != nullptr) { //przeszukanie całego łańcucha

        if (curr->key == key) {
            if (prev == nullptr) table[idx] = curr->next; //Jak jest pusty to usuwany jest pierwszy węzeł

            else prev->next = curr->next; //a jak nie jest pusty to węzeł ze środka albo końca
            delete curr;
            size--;
            return true;
        }

        prev = curr;
        curr = curr->next;
    }
    return false;
}



int* ChainingHashTable::find(int key) {

    int idx = hashFunction(key); //Obliczanie początkowego indeksu
    Node* curr = table[idx]; //wskaźnik na pierwszy węzeł w łańcuchu

    while (curr != nullptr) { //przeszukanie całego łańcucha
        if (curr->key == key) return &(curr->value); //Zwrócenie wskaźnika na wartość
        curr = curr->next;
    }
    return nullptr;
}

void ChainingHashTable::display() {

    for (int i = 0; i < capacity; ++i) { //Przejście po całej tablicy

        std::cout << "[" << i << "]: ";
        Node* curr = table[i];

        while (curr != nullptr) {
            std::cout << "(" << curr->key << "->" << curr->value << ") ";
            curr = curr->next;
        }
        std::cout << "\n";
    }
}

void ChainingHashTable::clear() {

    for (int i = 0; i < capacity; ++i) { //Przejście po całej tablicy i usuwanie wszystkich węzłów

        Node* curr = table[i];

        while (curr != nullptr) {
            Node* toDelete = curr;
            curr = curr->next;
            delete toDelete;
        }

        table[i] = nullptr;
    }
    size = 0;
}