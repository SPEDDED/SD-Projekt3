#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include "ChainingHashTable.h"
#include "OpenHashTable.h"
#include "HashTableAVL.h"



// generator liczb losowych z seedem
std::mt19937& getGenerator() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return gen;
}

void runResearch() {

}

void handleStructureMenu(HashTable* table){
}



int main(){
    int mainChoice;
    do{

        std::cout << "\n=== STRUKTURY DANYCH TABLICE MIESZAJĄCE ===\n"
                  << "1. Tablica z metodą łancuchową (Lista wiązana)\n"
                  << "2. Tablica z adresowaniem otwartym (Probkowanie liniowe)\n"
                  << "3. Tablica z metodą łancuchową (Drzewo AVL)\n"
                  << "4. AUTOMATYCZNE BADANIA\n"
                  << "0. Wyjście\n"
                  << "Wybór: ";
        std::cin >> mainChoice;

        if(mainChoice == 1) {

            ChainingHashTable table(100);
            handleStructureMenu(&table);
        } else if (mainChoice == 2) {

            OpenHashTable table(100);
            handleStructureMenu(&table);
        } else if (mainChoice == 3) {

            HashTableAVL table(100);
            handleStructureMenu(&table);
        } else if (mainChoice == 4) {
            
            runResearch();
        }

    } while (mainChoice !=0);

    return 0;
}