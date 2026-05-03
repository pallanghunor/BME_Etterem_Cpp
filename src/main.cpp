#include <iostream>
#include <limits>
#include <stdexcept>
#include "restaurant.hpp"
#include "food.hpp"
#include "drink.hpp"

// Segédfüggvény a menüpontok bekéréséhez (hibakezeléssel)
int getMenuChoice() {
    int choice;
    std::cout << "\nValasszon menupontot: ";
    while (!(std::cin >> choice)) {
        std::cin.clear(); // Hibaállapot törlése
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Szemét kipucolása a pufferből
        std::cout << "Ervenytelen bemenet! Kerem, szamot adjon meg: ";
    }
    return choice;
}

int main() {
    // 1. A központi vezérlő példányosítása
    Restaurant bmeEtterem;

    std::cout << "=== BME Etterem Kezelo Rendszer (Skeleton) ===\n\n";

    try {
        // 2. Kezdetleges tesztadatok betöltése a működés demonstrálására
        // (A végleges verzióban ez a loadData() fájlbeolvasásából jönne)
        std::cout << "[Rendszer] Tesztadatok inicializalasa...\n";
        
        // Polimorfizmus bemutatása: Különböző típusok mennek ugyanabba a listába
        bmeEtterem.addMenuItem(new Food(1, "Becsi Szelet", 3500, true, "Gluten, Tojas"));
        bmeEtterem.addMenuItem(new Drink(2, "Csapolt Sor", 1200, true, 0.5, true));
        bmeEtterem.addMenuItem(new Drink(3, "Limonade", 1500, true, 0.5, false));

        // Asztalok felvétele
        bmeEtterem.addTable(1, 4, "Ablak melletti", 0, 0);
        bmeEtterem.addTable(2, 2, "Kozepso kicsi", 2, 2);
        
        // 3. Osztályok kapcsolatának demonstrálása (Rendelés felvétele az 1-es asztalhoz)
        std::cout << "[Rendszer] Rendeles felvetele az 1-es asztalhoz...\n";
        Table* t1 = bmeEtterem.getTableById(1);
        if (t1) {
            t1->openOrder();
            t1->addItemToOrder(bmeEtterem.getMenuItemById(1), 2); // 2 db Bécsi szelet
            t1->addItemToOrder(bmeEtterem.getMenuItemById(2), 2); // 2 db Sör
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Kritikus hiba az inditaskor: " << e.what() << "\n";
        return 1;
    }

    // 4. Interaktív Felhasználói Felület (UI) ciklusa
    bool fut = true;
    while (fut) {
        std::cout << "\n--- Fomenu ---\n";
        std::cout << "1. Etlap megtekintese\n";
        std::cout << "2. 1-es asztal szamlajanak lezarasa es kinyomtatasa\n";
        std::cout << "3. Biztonsagos torles tesztelese (Sor torlese)\n";
        std::cout << "0. Kilepes\n";

        int valasztas = getMenuChoice();

        switch (valasztas) {
            case 1:
                std::cout << "\n--- ETLAP ---\n";
                for (int i = 1; i <= 3; ++i) {
                    MenuItem* item = bmeEtterem.getMenuItemById(i);
                    if (item) {
                        item->print(std::cout);
                    }
                }
                break;

            case 2:
                std::cout << "\n--- SZAMLA ZARASA (1-es asztal) ---\n";
                try {
                    Table* t = bmeEtterem.getTableById(1);
                    if (t && t->hasActiveOrder()) {
                        t->closeTable(std::cout); 
                        std::cout << "Az asztal sikeresen lezarva es felszabaditva.\n";
                    } else {
                        std::cout << "Az asztal nem talalhato, vagy nincs aktiv rendelese.\n";
                    }
                } catch (const std::exception& e) {
                    std::cout << "Hiba a szamlazas soran: " << e.what() << "\n";
                }
                break;

            case 3:
                std::cout << "\n--- TORLES TESZT ---\n";
                try {
                    std::cout << "Megprobalom torolni a 2-es ID-ju tetelt (Sor)...\n";
                    bmeEtterem.deleteMenuItem(2);
                    std::cout << "Sikeresen torolve!\n";
                } catch (const std::exception& e) {
                    std::cout << "HIBA: " << e.what() << "\n";
                }
                break;

            case 0:
                fut = false;
                std::cout << "Kilepes... Adatok mentese es memoria takaritasa.\n";
                break;

            default:
                std::cout << "Nincs ilyen menupont!\n";
                break;
        }
    }
    
    return 0;
}