#include "OpenHashTable.h"
#include <iostream>



OpenHashTable::OpenHashTable(int cap) : HashTable(cap) { //konstruktor
    table = new Entry[capacity];
}

OpenHashTable::~OpenHashTable() { //Dekonstruktor
    delete[] table;
}



void OpenHashTable::resizeAndRehash() { //funmkcja zmieniająca rozmiar tablicy a potem hashująca ponownie elementy

    int oldCap = capacity;
    Entry* oldTable = table; //Przechowanie starej tablicy


    capacity *= 2; // Dwuktorne zwiększenie rozmiaru
    table = new Entry[capacity];
    size = 0; //Ustawienie licznika elementów na 0 do ponownego wstawiania elementów


    // Przepisanie elementóœ ze starej tablicy
    for (int i = 0; i < oldCap; ++i) {
        if (oldTable[i].isOccupied && !oldTable[i].isDeleted) {
            insert(oldTable[i].key, oldTable[i].value);
        }
    }
    delete[] oldTable;
}







bool OpenHashTable::insert(int key, int value) {

    // Jeżeli liczba elemetów przekracza 70% pojemności to zwiększany jest rozmiar
    if (size >= capacity * 0.7) {
        resizeAndRehash();
    }

    int idx = hashFunction(key); //Obliczanie początkowego indeksu
    int startIdx = idx; // Zapisanie indeksu 
    int firstDeletedIdx = -1; // zmienna do zapamiętywania komórki która została usunięta, żeby można było tam nowy element wstawić

    do {
        if (!table[idx].isOccupied) { //sprawdzenie czy miejsce jest wolna

            int targetIdx = (firstDeletedIdx != -1) ? firstDeletedIdx : idx;
            // Wstawienie elementu do znalezionego slotu (wolnego lub pierwszego usuniętego)


            table[targetIdx].key = key; //Przypisanie klucza i warotści
            table[targetIdx].value = value;


            table[targetIdx].isOccupied = true; //Ustawienie flag
            table[targetIdx].isDeleted = false;
            size++;
            return true;
        }


        if (table[idx].isOccupied && !table[idx].isDeleted && table[idx].key == key) { //Aktualizacja wartości istniejacego klucza
            table[idx].value = value;
            return true;
        }


        if (table[idx].isDeleted && firstDeletedIdx == -1) { //Zapamiętanie pierwszego usuniętego slotu do użycia póżniej
            firstDeletedIdx = idx;
        }

        idx = (idx + 1) % capacity; //przesuwanie indeksu o 1

        
    } while (idx != startIdx);


    if (firstDeletedIdx != -1) { //Jak nie ma miejsca ale jest jakiś slot oznaczony jako usunięty to tam jest wstawiany element

        table[firstDeletedIdx].key = key; //Przypisanie klucza i wartości
        table[firstDeletedIdx].value = value;

        table[firstDeletedIdx].isOccupied = true; //Ustawienie flag
        table[firstDeletedIdx].isDeleted = false;
        size++;
        return true;
    }
    return false;
}



bool OpenHashTable::remove(int key) {

    int idx = hashFunction(key); // Obliczanie początkowego indeksu
    int startIdx = idx;

    do {

        if (!table[idx].isOccupied && !table[idx].isDeleted) return false; // Jak slot jest pusty to klucz nie istnieje

        if (table[idx].isOccupied && !table[idx].isDeleted && table[idx].key == key) { // Znalezienie klucza i oznaczenie jako usunięty

            table[idx].isDeleted = true;
            size--;
            return true;
        }

        idx = (idx + 1) % capacity; //Przesunięcie indeksu o 1 w prawo

    } while (idx != startIdx);

    return false;
}



int* OpenHashTable::find(int key) {

    int idx = hashFunction(key); // Obliczanie początkowego indeksu
    int startIdx = idx;

    do {

        if (!table[idx].isOccupied && !table[idx].isDeleted) return nullptr; // Jak slot jest pusty to klucz nie istnieje
        if (table[idx].isOccupied && !table[idx].isDeleted && table[idx].key == key) { // Znalezienie klucza
            return &(table[idx].value);
        }

        idx = (idx + 1) % capacity; //Przesunięcie indeksu o 1 w prawo

    } while (idx != startIdx);

    return nullptr;
}

void OpenHashTable::display() {

    for (int i = 0; i < capacity; ++i) { //Przejście po całej tablicy

        std::cout << "[" << i << "]: ";

        if (table[i].isOccupied && !table[i].isDeleted) {
            std::cout << "(" << table[i].key << "->" << table[i].value << ")";
        } 
        else if (table[i].isDeleted) {
            std::cout << "<DELETED>";
        } 
        else {
            std::cout << "<EMPTY>";
        }

        std::cout << "\n";
    }
}

void OpenHashTable::clear() {
    for (int i = 0; i < capacity; ++i) { //usuwanie wszystkich elementów
        table[i].isOccupied = false;
        table[i].isDeleted = false;
    }
    size = 0;
}