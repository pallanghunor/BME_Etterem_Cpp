#include <iostream>
#include <limits>
#include <stdexcept>
#include "restaurant.hpp"
#include "food.hpp"
#include "drink.hpp"
#include "ui.hpp"

#include "memtrace.h"

// Segédfüggvény a biztonságos szám bekéréshez (4. fázis előfutára)
int getIntInput() {
    int choice;
    std::cout << "Valasztas: ";
    while (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Ervenytelen bemenet! Kerem szamot adjon meg: ";
    }
    return choice;
}

int main() {
    Restaurant bmeEtterem;
    bmeEtterem.loadData(); // Adatok betöltése fájlokból

    bool fut = true;
    
    // --- FLASH MESSAGE VÁLTOZÓK ---
    // Ezt a két változót használjuk arra, hogy üzenetet küldjünk "vissza" a menü tetejére
    std::string flashMessage = "";
    MsgType flashType = MsgType::NONE;

    while (fut) {
        // [Ide jön majd a system("clear") vagy ("cls") a legvégén!]
        std::cout << "\n\n";

        // 1. FLASH MESSAGE MEGJELENÍTÉSE
        // Ha van beállítva üzenet (pl. egy korábbi sikeres művelet miatt), akkor kiírjuk a tetejére
        if (!flashMessage.empty()) {
            UI::printMsgBox(flashMessage, flashType);
            
            // AZONNAL KIÜRÍTJÜK, hogy a következő képernyőfrissítésnél eltűnjön!
            flashMessage = "";
            flashType = MsgType::NONE;
        }

        // 2. FŐMENÜ KIRAJZOLÁSA (Ahogy a print.c-ben is volt)
        std::cout << "+==============================+\n";
        std::cout << "|      BME ETTEREM KEZELO      |\n";
        std::cout << "+==============================+\n";
        std::cout << "1. Asztalok kezelese\n";
        std::cout << "2. Etlap kezelese\n";
        std::cout << "3. Rendelesek\n";
        std::cout << "4. Foglaltsagi terkep\n";
        std::cout << "0. Kilepes es mentes\n";
        std::cout << "+------------------------------+\n";

        int valasztas = getIntInput();

        switch (valasztas) {
            case 1:
                // TODO: Asztal almenü
                flashMessage = "Az asztalok kezelese menu meg nincs kesz!";
                flashType = MsgType::WARNING;
                break;

            case 2:
                // TODO: Étlap almenü
                flashMessage = "Az etlap kezelese menu meg nincs kesz!";
                flashType = MsgType::WARNING;
                break;

            case 3:
                // SZIMULÁCIÓ: Valaki felvett egy rendelést a 3-as menüben
                // Miután végigment a logikán, beállítja az üzenetet és visszadobja a főmenübe
                flashMessage = "Sikeresen felvetted a rendelest a 2-es asztalhoz!";
                flashType = MsgType::SUCCESS;
                break;

            case 4:
                bmeEtterem.showOccupancyMap();
                UI::pause();
                break;

            case 0:
                fut = false;
                std::cout << "Adatok mentese...\n";
                try {
                    bmeEtterem.saveData();
                    std::cout << "Sikeres mentes. Kilepes!\n";
                } catch (const std::exception& e) {
                    std::cout << "Hiba mentes kozben: " << e.what() << "\n";
                }
                break;

            default:
                flashMessage = "Nincs ilyen menupont!";
                flashType = MsgType::ERROR;
                break;
        }
    }

    return 0;
}