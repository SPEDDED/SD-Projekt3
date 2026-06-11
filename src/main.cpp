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
    std::cout<<"\n=== MENU BADAŃ TABLIC MIESZAJĄCYCH ===\n";
    
    std::ofstream csvFile("../data/wyniki_pomiarow.csv");
    if (!csvFile.is_open()) {
        std::cerr<< "Bład: Nie mozna utworzyć pliku w katalogu 'data/'.\n";
        return;
    }

    // Rozmiary tablic
    int sizes[] ={5000, 10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000, 110000, 120000, 130000, 140000, 150000}; 
    
    const int numSeeds = 10;  // 10 różnych seedów
    const int numReps = 100;   // 100 powtórzeń na każdy seed
    const int totalIterationsPerSize = numSeeds * numReps;

    unsigned int testSeeds[numSeeds] = {1111, 2222, 3333, 4444, 5555, 6666, 7777, 8888, 9999, 1234};

    csvFile << "Rozmiar;Struktura;Czas_Insert_ns;Czas_Remove_ns\n";
    std::cout <<"Pomiary w toku... Wyniki zapisane są w folderze 'data/wyniki_pomiarow.csv'\n";



    for (int size : sizes) { //Główna pętla, dla każdego rozmiaru tablicy
        long long totalInsertChaining = 0, totalRemoveChaining = 0;
        long long totalInsertOpen = 0, totalRemoveOpen = 0;
        long long totalInsertAVL = 0, totalRemoveAVL = 0;



        // Alokacja tablic danych przed pomiarami czasu
        int* keys = new int[size];
        int* vals = new int[size];


        for (int s =0; s < numSeeds; ++s) { //Pętla po seedach

            std::mt19937 gen(testSeeds[s]);
            std::uniform_int_distribution<int> dist(1, 2000000);


            for (int rep = 0; rep < numReps; ++rep) {

                // Wypełnienie tablicy kluczy i wartości danymi
                for (int i = 0; i< size; ++i) {
                    keys[i] = dist(gen);
                    vals[i] = dist(gen);
                }



                // --- CHAINING (LISTA WIĄZANA) ---
                {
                    ChainingHashTable table1(size * 1.3); // Zwiększenie pojemności, aby uniknąć zbyt wielu kolizji
                    
                    // Pomiar operacji insert
                    auto start = std::chrono::high_resolution_clock::now();
                    for (int i= 0; i <size; ++i) {
                        table1.insert(keys[i], vals[i]);
                    }

                    auto end = std::chrono::high_resolution_clock::now();
                    totalInsertChaining += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();



                    // Pomiar operacji remove
                    start = std::chrono::high_resolution_clock::now();
                    for (int i = 0; i < size; ++i) {
                        table1.remove(keys[i]);
                    }

                    end = std::chrono::high_resolution_clock::now();
                    totalRemoveChaining += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
                }




                // --- ADRESOWANIE OTWARTE ---
                {
                    OpenHashTable table2(size*1.5); // Zwiększenie pojemności, aby uniknąć zbyt wielu kolizji
                    
                    // Pomiar operacji insert
                    auto start = std::chrono::high_resolution_clock::now();
                    for (int i = 0; i <size; ++i) {
                        table2.insert(keys[i], vals[i]);
                    }

                    auto end = std::chrono::high_resolution_clock::now();
                    totalInsertOpen += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();




                    // Pomiar operacji remove
                    start = std::chrono::high_resolution_clock::now();
                    for (int i = 0; i <size; ++i) {
                        table2.remove(keys[i]);
                    }

                    end = std::chrono::high_resolution_clock::now();
                    totalRemoveOpen += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
                }

                // --- METODA ŁAŃCUCHOWA (DRZEWO AVL) ---
                {
                    HashTableAVL table3(size*1.3); // Zwiększenie pojemności, aby uniknąć zbyt wielu kolizji
                    
                    // Pomiar operacji insert
                    auto start = std::chrono::high_resolution_clock::now();
                    for (int i = 0; i<size; ++i) {
                        table3.insert(keys[i], vals[i]);
                    }

                    auto end = std::chrono::high_resolution_clock::now();
                    totalInsertAVL += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();




                    // Pomiar operacji remove
                    start = std::chrono::high_resolution_clock::now();
                    for (int i= 0; i <size; ++i) {
                        table3.remove(keys[i]);
                    }

                    end = std::chrono::high_resolution_clock::now();
                    totalRemoveAVL += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
                }
            }
        }

        //Dzielenie sumy czasów przez liczbę iteracji, aby uzyskać średni czas jednej operacji
        long long divisor= (long long)totalIterationsPerSize * size;


        csvFile << size << ";Lista_Wiazana;" << totalInsertChaining / divisor << ";" << totalRemoveChaining / divisor << "\n";
        csvFile << size << ";Adresowanie_Otwarte;" << totalInsertOpen / divisor << ";" << totalRemoveOpen / divisor << "\n";
        csvFile << size << ";Drzewo_AVL;" << totalInsertAVL / divisor << ";" << totalRemoveAVL / divisor <<"\n";


        delete[] keys;
        delete[] vals;
        std::cout<< "-> Ukończono pomiary dla rozmiaru: "<< size << "\n";
    }

    csvFile.close();
    std::cout << "\n=== BADANIA ZAKONCZONE SUKCESEM ===\n";
}

void handleStructureMenu(HashTable* table){
    int choice;
    do {

        std::cout << "\n--- Menu Operacji ---\n"
                  << "1. Zbuduj z pliku\n"
                  << "2. Dodaj element (insert)\n"
                  << "3. Usuń element (remove)\n"
                  << "4. Znajdź element\n"
                  << "5. Utwórz losowo\n"
                  << "6. Wyświetl strukturę\n"
                  << "0. Powrót\n"
                  << "Wybór: ";
        std::cin >>choice;


        if (choice==1) {

            table->clear();
            std::string filename;
            std::cout <<"Podaj nazwe pliku (np. dane.txt): ";
            std::cin >> filename;
            std::ifstream file(filename);


            if (file.is_open()){
                int k, v;
                while (file >> k >> v) table->insert(k, v);
                std::cout << "Wczytano dane pomyślnie.\n";
            }else std::cout << "Bład otwarcia pliku.\n";
        }

        else if (choice == 2) {
            int k, v;
            std::cout << "Podaj klucz (int) i wartość (int): ";
            std::cin >> k >> v;
            table->insert(k, v);
        }
        else if (choice == 3) {

            int k;
            std::cout << "Podaj klucz do usuniecia: ";
            std::cin >> k;

            if (table->remove(k)) std::cout << "Usunięto.\n";
            else std::cout << "Nie znaleziono klucza.\n";
        }
        else if (choice == 4) {

            int k;
            std::cout << "Podaj klucz: ";
            std::cin >> k;
            int* res = table->find(k);

            if (res) std::cout << "Znaleziono! Wartość: " << *res << "\n";
            else std::cout << "Klucz nie istnieje.\n";
        }
        else if (choice == 5) {

            table->clear();
            int limit;
            std::cout << "Ile elementow wylosować? ";
            std::cin >> limit;
            std::mt19937& gen = getGenerator();
            std::uniform_int_distribution<int> dist(1, 100000);

            for (int i = 0; i < limit; ++i) {
                table->insert(dist(gen), dist(gen));
            }
            std::cout << "Wygenerowano.\n";
        }
        else if (choice== 6) {
            table->display();

        }

    } while (choice != 0);
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
