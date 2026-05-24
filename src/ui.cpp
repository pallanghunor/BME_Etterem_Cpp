#include "ui.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <filesystem>
#include <iomanip>
#include <chrono>
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

#include "food.hpp"
#include "drink.hpp"
#include "table.hpp"

#include "memtrace.h"

UI::UI(Restaurant &r) : restaurant(r), flashMessage(""), flashType(MsgType::NONE) {}

// Biztonságos egész szám beolvasás validálással
int UI::getIntInput(const std::string &prompt)
{
    int value;
    std::cout << prompt;
    while (!(std::cin >> value))
    {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << COLOR_ERROR << "Ervenytelen bemenet! Kerem szamot adjon meg: " << COLOR_RESET << "\n"
                  << prompt;
    }
    std::cin.ignore(10000, '\n'); // A szám utáni Entert (\n) kivesszük, hogy a következő getline ne ugorjon át!
    return value;
}

// Biztonságos lebegőpontos szám beolvasás validálással
double UI::getDoubleInput(const std::string &prompt)
{
    double value;
    std::cout << prompt;
    while (!(std::cin >> value))
    {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << COLOR_ERROR << "Ervenytelen bemenet! Kerem szamot adjon meg: " << COLOR_RESET << "\n"
                  << prompt;
    }
    std::cin.ignore(10000, '\n');
    return value;
}

// Biztonságos string beolvasás
std::string UI::getStringInput(const std::string &prompt)
{
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    return line;
}

void UI::consoleClear()
{
    // [A legvégén ide jön majd a system("cls") vagy system("clear")]
    std::cout << "\n\n";
}

void UI::printFlashMessage()
{
    if (!flashMessage.empty())
    {
        printMsgBox(flashMessage, flashType);
        flashMessage = "";
        flashType = MsgType::NONE;
    }
}

// A FŐ PROGRAMCIKLUS
void UI::run()
{
    bool running = true;
    while (running)
    {
        consoleClear();
        printFlashMessage();

        std::cout << "+==============================+\n";
        std::cout << "|      BME ETTEREM KEZELO      |\n";
        std::cout << "+==============================+\n";
        std::cout << "1. Asztalok kezelese\n";
        std::cout << "2. Etlap kezelese\n";
        std::cout << "3. Rendelesek kezelese\n";
        std::cout << "4. Foglaltsagi terkep\n";
        std::cout << "0. Kilepes es mentes\n";
        std::cout << "+------------------------------+\n";

        int choice = getIntInput("Valasztas: ");

        switch (choice)
        {
        case 1:
            tableMenu();
            break;
        case 2:
            menuMenu();
            break;
        case 3:
            orderMenu();
            break;
        case 4:
            restaurant.showOccupancyMap(std::cout);
            pause();
            break;
        case 0:
            running = false;
            std::cout << "Adatok mentese...\n";
            try
            {
                restaurant.saveData();
                std::cout << "Sikeres mentes. Viszlat!\n";
            }
            catch (const std::exception &e)
            {
                std::cout << "Hiba mentes kozben: " << e.what() << "\n";
                pause();
            }
            break;
        default:
            flashMessage = "Nincs ilyen menupont!";
            flashType = MsgType::ERROR;
            break;
        }
    }
}

// ASZTALOK ALMENÜ
void UI::tableMenu()
{
    bool back = false;
    while (!back)
    {
        consoleClear();
        printFlashMessage();

        std::cout << "--- ASZTALOK KEZELESE ---\n";
        std::cout << "1. Uj asztal felvetele\n";
        std::cout << "2. Asztal modositasa\n";
        std::cout << "3. Asztal torlese\n";
        std::cout << "0. Vissza a fomenube\n";

        int choice = getIntInput("Valasztas: ");
        switch (choice)
        {
        case 1:
            addTableMenu();
            break;
        case 2:
            modifyTableMenu();
            break;
        case 3:
            deleteTableMenu();
            break;
        case 0:
            back = true;
            break;
        default:
            flashMessage = "Nincs ilyen menupont!";
            flashType = MsgType::ERROR;
            break;
        }
    }
}

// Új asztal felvetele
void UI::addTableMenu()
{
    std::cout << "\n--- UJ ASZTAL FELVETELE ---\n";

    // 1. Az ID-t a Restaurant osztálytól kérjük, hogy mindig egyedi legyen
    int id = restaurant.getNextTableId();
    std::cout << "Asztal ID: " << id << " (Generalt)\n";

    // 2. Férőhely bekérése
    int seats = getValidatedSeats(Table::MIN_SEATS);

    // 3. Leírás bekérése
    std::string description = getStringInput("Leiras: ");

    // 4. Koordináták bekérése és validálása
    std::pair<int, int> position = getValidatedTablePosition();
    int x = position.first;
    int y = position.second;

    try
    {
        restaurant.addTable(id, seats, description, x, y);
        flashMessage = "Asztal sikeresen felveve!";
        flashType = MsgType::SUCCESS;
    }
    catch (const std::exception &e)
    {
        flashMessage = std::string("Varatlan hiba: ") + e.what();
        flashType = MsgType::ERROR;
    }
}

// Asztal módosítása
void UI::modifyTableMenu()
{
    Table *table = selectTableById();
    if (table == nullptr)
    {
        return;
    }

    int id = table->getId();
    bool modifyBack = false;
    while (!modifyBack)
    {
        consoleClear();
        printFlashMessage();

        std::cout << "\n--- ASZTAL MODOSITASA (ID: " << id << ") ---\n\n";
        table->printTable(std::cout);
        std::cout << "\n\n";
        std::cout << "1. Ferohelyek szama\n";
        std::cout << "2. Leiras\n";
        std::cout << "3. Pozicio (X, Y)\n";
        std::cout << "4. Foglaltsag allapota\n";
        std::cout << "0. Vissza\n";

        int modifyChoice = getIntInput("Adja meg a modositando adat sorszamat (0-4): ");

        switch (modifyChoice)
        {
        case 1:
        {
            int seats = getValidatedSeats(Table::MIN_SEATS);
            try
            {
                table->setSeats(seats);
                flashMessage = "Asztal ferohelyei sikeresen modositva!";
                flashType = MsgType::SUCCESS;
            }
            catch (const std::exception &e)
            {
                flashMessage = std::string("Hiba a modositaskor: ") + e.what();
                flashType = MsgType::ERROR;
            }
            break;
        }

        case 2:
        {
            std::string description = getStringInput("Uj leiras: ");
            try
            {
                table->setDescription(description);
                flashMessage = "Asztal leirasa sikeresen modositva!";
                flashType = MsgType::SUCCESS;
            }
            catch (const std::exception &e)
            {
                flashMessage = std::string("Hiba a modositaskor: ") + e.what();
                flashType = MsgType::ERROR;
            }
            break;
        }

        case 3:
        {
            std::pair<int, int> position = getValidatedTablePosition(id);
            int x = position.first;
            int y = position.second;

            try
            {
                table->setPosition(x, y);
                flashMessage = "Asztal pozicioja sikeresen modositva!";
                flashType = MsgType::SUCCESS;
            }
            catch (const std::exception &e)
            {
                flashMessage = std::string("Hiba a modositaskor: ") + e.what();
                flashType = MsgType::ERROR;
            }
            break;
        }

        case 4:
        {
            try
            {
                table->setOccupied(!table->isOccupied());
                flashMessage = std::string("Asztal foglaltsaga atvaltva: ") + (table->isOccupied() ? "FOGLALT" : "SZABAD");
                flashType = MsgType::SUCCESS;
            }
            catch (const std::exception &e)
            {
                flashMessage = e.what();
                flashType = MsgType::ERROR;
            }
            break;
        }

        case 0:
            modifyBack = true;
            break;

        default:
            flashMessage = "Nincs ilyen menupont!";
            flashType = MsgType::ERROR;
            break;
        }
    }
}

// Asztal törlése
void UI::deleteTableMenu()
{
    Table *table = selectTableById();
    if (table == nullptr)
    {
        return;
    }

    try
    {
        restaurant.deleteTable(table->getId());
        flashMessage = "Asztal sikeresen torolve!";
        flashType = MsgType::SUCCESS;
    }
    catch (const std::exception &e)
    {
        flashMessage = e.what();
        flashType = MsgType::ERROR;
    }
}

// Helper függvény: Koordináta bekérés és validálása
int UI::getValidatedCoordinate(const std::string &axis, int maxValue)
{
    int value;
    while (true)
    {
        value = getIntInput(axis + " koordinata (0-" + std::to_string(maxValue) + "): ");
        if (value < 0 || value > maxValue)
        {
            std::cout << COLOR_ERROR << "Hiba: A " << axis << " koordinatanak 0 es "
                      << maxValue << " kozott kell lennie!\n"
                      << COLOR_RESET;
        }
        else
        {
            break;
        }
    }
    return value;
}

// Helper függvény: Férőhely bekérés és validálása
int UI::getValidatedSeats(int minSeats)
{
    int seats;
    while (true)
    {
        seats = getIntInput("Ferohelyek szama: ");
        if (seats < minSeats)
        {
            std::cout << COLOR_ERROR << "Hiba: Az asztalnak legalabb " << minSeats
                      << " ferohelyesnek kell lennie!\n"
                      << COLOR_RESET;
        }
        else
        {
            break;
        }
    }
    return seats;
}

// Helper függvény: Asztal kiválasztása ID alapján
Table *UI::selectTableById()
{
    std::cout << "--- ELERHETO ASZTALOK ---\n\n";
    restaurant.printTables(std::cout);
    std::cout << "\n-------------------------\n";

    int id = getIntInput("Add meg az asztal ID-jat (0: vissza): ");
    if (id == 0)
    {
        return nullptr;
    }

    Table *table = restaurant.getTableById(id);
    if (table == nullptr)
    {
        flashMessage = "Nincs ilyen ID-ju asztal a rendszerben!";
        flashType = MsgType::ERROR;
    }
    return table;
}

// Helper függvény: Asztal pozíciójának bekérése és validálása
std::pair<int, int> UI::getValidatedTablePosition(int excludeTableId)
{
    int maxX = restaurant.getMaxX();
    int maxY = restaurant.getMaxY();
    int x, y;

    while (true)
    {
        x = getValidatedCoordinate("X", maxX);
        y = getValidatedCoordinate("Y", maxY);

        // Ütközés ellenőrzés: ha már van asztal a helyen és nem ugyanez az asztal
        if (restaurant.isTableAt(x, y))
        {
            Table *existingTable = restaurant.getTableById(excludeTableId);

            // Ha módosítás közben van (excludeTableId != -1) és az a hely az, ahol az asztal jelenleg van
            if (excludeTableId != -1 && existingTable != nullptr &&
                existingTable->getX() == x && existingTable->getY() == y)
            {
                // Ez az asztal saját helye, engedélyezzük
                break;
            }

            // Egyébként hiba
            std::cout << COLOR_ERROR << "Hiba: Ezen a helyen (" << x << "," << y
                      << ") mar van egy asztal! Kerem, adjon meg mas koordinatakat.\n"
                      << COLOR_RESET;
        }
        else
        {
            break;
        }
    }

    return std::make_pair(x, y);
}

// ÉTLAP ALMENÜ
void UI::menuMenu()
{
    bool back = false;
    while (!back)
    {
        consoleClear();
        printFlashMessage();

        std::cout << "--- ETLAP KEZELESE ---\n";
        std::cout << "1. Etlap megtekintese\n";
        std::cout << "2. Tetel felvetele\n";
        std::cout << "3. Tetel modositasa\n";
        std::cout << "4. Tetel torlese\n";
        std::cout << "0. Vissza a fomenube\n";

        int choice = getIntInput("Valasztas: ");

        switch (choice)
        {
        case 1:
            viewMenuMenu();
            break;
        case 2:
            addMenuItemMenu();
            break;
        case 3:
            modifyMenuItemMenu();
            break;
        case 4:
            deleteMenuItemMenu();
            break;
        case 0:
            back = true;
            break;
        default:
            flashMessage = "Nincs ilyen menupont!";
            flashType = MsgType::ERROR;
            break;
        }
    }
}

// Étlap megtekintése
void UI::viewMenuMenu()
{
    consoleClear();
    restaurant.printMenu(std::cout);
    pause();
}

// Tétel kiválasztása ID alapján
MenuItem *UI::selectMenuItemById()
{
    consoleClear();
    restaurant.printMenu(std::cout);

    int id = getIntInput("Add meg a tetel ID-jat (0: vissza): ");
    if (id == 0)
    {
        return nullptr;
    }

    MenuItem *item = restaurant.getMenuItemById(id);
    if (item == nullptr)
    {
        flashMessage = "Nincs ilyen ID-ju tetel az etlapon!";
        flashType = MsgType::ERROR;
        return nullptr;
    }

    return item;
}

// Új étel vagy ital felvétele
void UI::addMenuItemMenu()
{
    consoleClear();
    std::cout << "--- UJ TETEL FELVETELE ---\n";
    std::cout << "1. Etel (Food)\n";
    std::cout << "2. Ital (Drink)\n";
    int typeChoice;
    while (true)
    {
        typeChoice = getIntInput("Valassz tipust (1 vagy 2): ");

        if (typeChoice == 1 || typeChoice == 2)
        {
            break;
        }
        else
        {
            std::cout << COLOR_ERROR << "Hiba: ervenytelen tipus! Kerem, valassz 1-et (Etel) vagy 2-t (Ital).\n"
                      << COLOR_RESET;
        }
    }

    int id = restaurant.getNextMenuItemId();
    std::cout << "Azonosito (ID): " << id << " (Generalt)\n";
    std::string name = askValidatedName("Nev: ");

    int price = askValidatedPrice("Ar (Ft): ");

    bool available = askYesNo("Elerheto? (1=Igen, 0=Nem): ");

    std::string allergens;
    double volume = 0.0;
    bool alcoholic = false;

    try
    {
        if (typeChoice == 1)
        { // ÉTEL
            allergens = getStringInput("Allergenek (pl. tej, tojas): ");
            restaurant.addMenuItem(new Food(id, name, price, available, allergens));
            flashMessage = "Uj etel sikeresen felveve az etlapra!";
        }
        else
        { // ITAL
            volume = askValidatedVolume("Urtartalom (liter, pl. 0.5): ");
            alcoholic = askYesNo("Alkoholos? (1=Igen, 0=Nem): ");
            restaurant.addMenuItem(new Drink(id, name, price, available, volume, alcoholic));
            flashMessage = "Uj ital sikeresen felveve az etlapra!";
        }
        flashType = MsgType::SUCCESS;
    }
    catch (const std::exception &e)
    {
        flashMessage = std::string("Hiba a felvetelkor: ") + e.what();
        flashType = MsgType::ERROR;
    }
}

// ============================================================================
// ÚJ: MODIFIVISITOR IMPLEMENTÁCIÓ (Összeköti az objektumot a UI almenükkel)
// ============================================================================
void UI::ModifyVisitor::visitFood(Food *food)
{
    ui.modifyFoodInteractive(food);
}

void UI::ModifyVisitor::visitDrink(Drink *drink)
{
    ui.modifyDrinkInteractive(drink);
}

// ============================================================================
// A MEGÚJULT FŐ MÓDOSÍTÓ MENÜ (Tiszta polimorfizmus, nincs cast!)
// ============================================================================
void UI::modifyMenuItemMenu()
{
    MenuItem *item = selectMenuItemById();
    if (item == nullptr)
    {
        return; // Visszalépés, ha nem választott érvényes tételt
    }

    // Elküldjük a látogatót. Az objektum (Food vagy Drink) magától tudja,
    // melyik visit függvényt kell visszahívnia a típusának megfelelően!
    ModifyVisitor visitor(*this);
    item->accept(visitor);
}

// ============================================================================
// KÜLÖNVÁLASZTOTT, TÍPUSSZPECIFIKUS INTERAKTÍV ALMENÜK
// ============================================================================

void UI::modifyFoodInteractive(Food *food)
{
    bool back = false;
    while (!back)
    {
        consoleClear();
        printFlashMessage();

        std::cout << "--- ETEL MODOSITASA (ID: " << food->getId() << ") ---\n\n";
        food->print(std::cout);
        std::cout << "\n";
        std::cout << "1. Nev\n";
        std::cout << "2. Ar\n";
        std::cout << "3. Elerhetoseg\n";
        std::cout << "4. Allergenek\n";
        std::cout << "0. Vissza\n";

        int choice = getIntInput("Valasztas: ");

        try
        {
            switch (choice)
            {
            case 1:
                food->setName(askValidatedName("Uj nev: "));
                flashMessage = "Nev sikeresen modositva!";
                flashType = MsgType::SUCCESS;
                break;

            case 2:
                food->setPrice(askValidatedPrice("Uj ar (Ft): "));
                flashMessage = "Ar sikeresen modositva!";
                flashType = MsgType::SUCCESS;
                break;

            case 3:
                food->setAvailable(!food->isAvailable());
                flashMessage = std::string("Elerhetoseg atallitva erre: ") + (food->isAvailable() ? "ELERHETO" : "NEM ELERHETO");
                flashType = MsgType::SUCCESS;
                break;

            case 4:
                food->setAllergens(getStringInput("Uj allergenek: "));
                flashMessage = "Allergenek sikeresen modositva!";
                flashType = MsgType::SUCCESS;
                break;

            case 0:
                back = true;
                break;

            default:
                flashMessage = "Nincs ilyen menupont!";
                flashType = MsgType::ERROR;
                break;
            }
        }
        catch (const std::exception &e)
        {
            flashMessage = std::string("Hiba a modositaskor: ") + e.what();
            flashType = MsgType::ERROR;
        }
    }
}

void UI::modifyDrinkInteractive(Drink *drink)
{
    bool back = false;
    while (!back)
    {
        consoleClear();
        printFlashMessage();

        std::cout << "--- ITAL MODOSITASA (ID: " << drink->getId() << ") ---\n\n";
        drink->print(std::cout);
        std::cout << "\n";
        std::cout << "1. Nev\n";
        std::cout << "2. Ar\n";
        std::cout << "3. Elerhetoseg\n";
        std::cout << "4. Urtartalom\n";
        std::cout << "5. Alkoholos\n";
        std::cout << "0. Vissza\n";

        int choice = getIntInput("Valasztas: ");

        try
        {
            switch (choice)
            {
            case 1:
                drink->setName(askValidatedName("Uj nev: "));
                flashMessage = "Nev sikeresen modositva!";
                flashType = MsgType::SUCCESS;
                break;

            case 2:
                drink->setPrice(askValidatedPrice("Uj ar (Ft): "));
                flashMessage = "Ar sikeresen modositva!";
                flashType = MsgType::SUCCESS;
                break;

            case 3:
                drink->setAvailable(!drink->isAvailable());
                flashMessage = std::string("Elerhetoseg atallitva erre: ") + (drink->isAvailable() ? "ELERHETO" : "NEM ELERHETO");
                flashType = MsgType::SUCCESS;
                break;

            case 4:
                drink->setVolume(askValidatedVolume("Uj urtartalom (liter): "));
                flashMessage = "Urtartalom sikeresen modositva!";
                flashType = MsgType::SUCCESS;
                break;

            case 5:
                drink->setAlcoholic(!drink->getIsAlcoholic());
                flashMessage = std::string("Alkoholossag atallitva erre: ") + (drink->getIsAlcoholic() ? "ALKOHOLOS" : "NEM ALKOHOLOS");
                flashType = MsgType::SUCCESS;
                break;

            case 0:
                back = true;
                break;

            default:
                flashMessage = "Nincs ilyen menupont!";
                flashType = MsgType::ERROR;
                break;
            }
        }
        catch (const std::exception &e)
        {
            flashMessage = std::string("Hiba a modositaskor: ") + e.what();
            flashType = MsgType::ERROR;
        }
    }
}

// Tétel törlése az étlapról
void UI::deleteMenuItemMenu()
{
    MenuItem *item = selectMenuItemById();
    if (item == nullptr)
    {
        return;
    }

    try
    {
        restaurant.deleteMenuItem(item->getId());
        flashMessage = "Tetel sikeresen torolve!";
        flashType = MsgType::SUCCESS;
    }
    catch (const std::exception &e)
    {
        flashMessage = std::string("Sikertelen torles: ") + e.what();
        flashType = MsgType::ERROR;
    }
}

// RENDELÉSEK ALMENÜ
void UI::orderMenu()
{
    bool back = false;
    while (!back)
    {
        consoleClear();
        printFlashMessage();

        std::cout << "--- RENDELESEK KEZELESE ---\n\n";

        Table *table = selectTableById();
        if (table == nullptr)
        {
            // A helper nullptr-t ad vissza akkor is, ha hibas az ID.
            // Csak akkor lepunk vissza, ha nincs hibauzenet (0: vissza).
            if (!flashMessage.empty() && flashType == MsgType::ERROR)
            {
                continue;
            }
            back = true;
            continue;
        }

        if (!table->isOccupied())
        {
            table->openOrder();
            flashMessage = "Az asztalhoz uj rendeles nyilt.";
            flashType = MsgType::SUCCESS;
        }
        else
        {
            flashMessage = "Az asztal mar foglalt, a rendeles aktiv.";
            flashType = MsgType::INFO;
        }

        bool tableBack = false;
        while (!tableBack)
        {
            consoleClear();
            printFlashMessage();

            std::cout << "--- RENDELES KEZELESE (ASZTAL ID: " << table->getId() << ") ---\n";
            std::cout << "1. Rendeles megtekintese\n";
            std::cout << "2. Tetel hozzaadasa\n";
            std::cout << "3. Tetel modositasa\n";
            std::cout << "4. Fizetes es lezaras\n";
            std::cout << "0. Vissza\n";

            int orderChoice = getIntInput("Valasztas: ");
            switch (orderChoice)
            {
            case 1:
            {
                consoleClear();
                Order *order = table->getOrder();

                std::cout << "--- AKTIV RENDELES (ASZTAL ID: " << table->getId() << ") ---\n\n";

                if (order == nullptr)
                {
                    flashMessage = "Ehhez az asztalhoz jelenleg nincs aktiv rendeles.";
                    flashType = MsgType::INFO;
                    break;
                }

                const List<OrderItem> &items = order->getItems();
                if (items.getSize() == 0)
                {
                    flashMessage = "Ehhez az asztalhoz jelenleg nincs aktiv rendeles.";
                    flashType = MsgType::INFO;
                    break;
                }

                std::cout << "Tetelnev | Darab | Reszosszeg\n";
                std::cout << "--------------------------------\n";
                for (const auto &orderItem : items)
                {
                    MenuItem *item = orderItem.getItem();
                    if (item != nullptr)
                    {
                        std::cout << item->getName() << " | "
                                  << orderItem.getQuantity() << " db | "
                                  << orderItem.getSubtotal() << " Ft\n";
                    }
                }
                std::cout << "--------------------------------\n";
                std::cout << "Eddigi vegosszeg: " << order->getTotal() << " Ft\n";
                pause();
                break;
            }

            case 2:
            {
                Order *order = table->getOrder();
                if (order == nullptr)
                {
                    table->openOrder();
                }

                MenuItem *item = selectMenuItemById();
                if (item == nullptr)
                {
                    if (!flashMessage.empty() && flashType == MsgType::ERROR)
                    {
                        break;
                    }
                    break;
                }

                int quantity = 0;
                while (quantity <= 0)
                {
                    quantity = getIntInput("Darabszam: ");
                    if (quantity <= 0)
                    {
                        std::cout << COLOR_ERROR << "Hiba: a darabszamnak nagyobbnak kell lennie 0-nal!\n"
                                  << COLOR_RESET;
                    }
                }

                try
                {
                    table->addItemToOrder(item, quantity);
                    flashMessage = "Tetel sikeresen hozzaadva a rendeleshez.";
                    flashType = MsgType::SUCCESS;
                }
                catch (const std::exception &e)
                {
                    flashMessage = std::string("Sikertelen hozzaadas: ") + e.what();
                    flashType = MsgType::ERROR;
                }
                break;
            }

            case 3:
            {
                Order *order = table->getOrder();
                if (order == nullptr)
                {
                    flashMessage = "Ehhez az asztalhoz jelenleg nincs aktiv rendeles.";
                    flashType = MsgType::ERROR;
                    break;
                }

                consoleClear();
                std::cout << "--- AKTIV RENDELES (ASZTAL ID: " << table->getId() << ") ---\n\n";

                const List<OrderItem> &items = order->getItems();
                if (items.getSize() == 0)
                {
                    flashMessage = "A rendeles jelenleg ures.";
                    flashType = MsgType::INFO;
                    break;
                }

                std::cout << "ID | Tetelnev | Darab | Reszosszeg\n";
                std::cout << "-----------------------------------\n";
                for (auto it = items.begin(); it != items.end(); ++it)
                {
                    const OrderItem &orderItem = *it;
                    MenuItem *menuItem = orderItem.getItem();
                    if (menuItem != nullptr)
                    {
                        std::cout << menuItem->getId() << " | "
                                  << menuItem->getName() << " | "
                                  << orderItem.getQuantity() << " db | "
                                  << orderItem.getSubtotal() << " Ft\n";
                    }
                }
                std::cout << "-----------------------------------\n";

                int itemId = getIntInput("Add meg a modositando tetel ID-jat (0: vissza): ");
                if (itemId == 0)
                {
                    break;
                }

                MenuItem *itemToUpdate = nullptr;
                for (auto it = items.begin(); it != items.end(); ++it)
                {
                    MenuItem *menuItem = (*it).getItem();
                    if (menuItem != nullptr && menuItem->getId() == itemId)
                    {
                        itemToUpdate = menuItem;
                        break;
                    }
                }

                if (itemToUpdate == nullptr)
                {
                    flashMessage = "A megadott ID-jú tetel nincs benne ebben a rendelesben.";
                    flashType = MsgType::ERROR;
                    break;
                }

                int newQuantity = -1;
                while (newQuantity < 0)
                {
                    newQuantity = getIntInput("Uj darabszam (0 = torles): ");
                    if (newQuantity < 0)
                    {
                        std::cout << COLOR_ERROR << "Hiba: a darabszam nem lehet negativ!\n"
                                  << COLOR_RESET;
                    }
                }

                try
                {
                    if (newQuantity == 0)
                    {
                        order->removeItem(itemToUpdate);
                        flashMessage = "Tetel sikeresen torolve a rendelesbol.";
                        flashType = MsgType::SUCCESS;
                    }
                    else
                    {
                        order->updateItemQuantity(itemToUpdate, newQuantity);
                        flashMessage = "Tetel darabszama sikeresen modositva.";
                        flashType = MsgType::SUCCESS;
                    }
                }
                catch (const std::exception &e)
                {
                    flashMessage = std::string("Sikertelen modositas: ") + e.what();
                    flashType = MsgType::ERROR;
                }
                break;
            }

            case 4:
            {
                if (table->getOrder() == nullptr || table->getOrder()->getItems().getSize() == 0)
                {
                    flashMessage = "Ehhez az asztalhoz jelenleg nincs aktiv rendeles.";
                    flashType = MsgType::INFO;
                    break;
                }
                try
                {
                    // 1. Mai dátum lekérése és formázása
                    std::time_t t = std::time(nullptr);
                    std::tm *now = std::localtime(&t);

                    std::stringstream dateStream;
                    dateStream << (now->tm_year + 1900) << "_"
                               << std::setw(2) << std::setfill('0') << (now->tm_mon + 1) << "_"
                               << std::setw(2) << std::setfill('0') << now->tm_mday;
                    std::string todayStr = dateStream.str();

                    // 2. Sorszám beolvasása a segédfájlból
                    int nextSeq = 1;
                    std::string counterFileName = "szamla_counter.txt";
                    std::ifstream counterIn(counterFileName);
                    if (counterIn.is_open())
                    {
                        std::string savedDate;
                        int savedSeq;
                        if (counterIn >> savedDate >> savedSeq)
                        {
                            if (savedDate == todayStr)
                            {
                                nextSeq = savedSeq + 1;
                            }
                        }
                        counterIn.close();
                    }

                    // 3. Sorszám mentése a segédfájlba
                    std::ofstream counterOut(counterFileName);
                    if (counterOut.is_open())
                    {
                        counterOut << todayStr << " " << nextSeq;
                        counterOut.close();
                    }

                    // 4. A végső fájlnév összerakása
                    std::string finalFilename = "szamlak/szamla_" + todayStr + "_" + std::to_string(nextSeq) + ".txt";

                    // 5. Fájl megnyitása és asztal lezárása
                    std::ofstream outFile(finalFilename);
                    if (!outFile.is_open())
                    {
                        throw std::runtime_error("Nem sikerult letrehozni a szamlafajlt!");
                    }

                    table->closeTable(outFile);
                    outFile.close();

                    flashMessage = "Sikeres fizetes! Szamla: " + finalFilename;
                    flashType = MsgType::SUCCESS;

                    back = true;
                }
                catch (const std::exception &e)
                {
                    flashMessage = std::string("Hiba a fizeteskor: ") + e.what();
                    flashType = MsgType::ERROR;
                }
                break;
            }

            case 0:
                tableBack = true;
                break;

            default:
                flashMessage = "Nincs ilyen menupont!";
                flashType = MsgType::ERROR;
                break;
            }
        }
    }
}

List<std::string> UI::wordWrap(const std::string &text, int maxWidth)
{
    List<std::string> lines;
    std::istringstream words(text);
    std::string word;
    std::string currentLine;
    while (words >> word)
    {
        if (currentLine.empty())
            currentLine = word;
        else if (currentLine.length() + 1 + word.length() <= (size_t)maxWidth)
            currentLine += " " + word;
        else
        {
            lines.push_back(currentLine);
            currentLine = word;
        }
    }
    if (!currentLine.empty())
        lines.push_back(currentLine);
    return lines;
}

void UI::printMsgBox(const std::string &msg, MsgType type)
{
    if (msg.empty())
        return;
    const int maxContentWidth = HEADER_LENGTH - 4;
    std::string color = COLOR_RESET;
    if (type == MsgType::SUCCESS)
        color = COLOR_SUCCESS;
    else if (type == MsgType::ERROR)
        color = COLOR_ERROR;
    else if (type == MsgType::WARNING)
        color = COLOR_WARNING;
    else if (type == MsgType::INFO)
        color = COLOR_INFO;

    std::string border(HEADER_LENGTH, '=');
    std::cout << color << border << "\n";
    List<std::string> lines = wordWrap(msg, maxContentWidth);
    for (auto it = lines.begin(); it != lines.end(); ++it)
    {
        std::string line = *it;
        int padLeft = (maxContentWidth - line.length()) / 2;
        int padRight = maxContentWidth - line.length() - padLeft;
        std::cout << "| " << std::string(padLeft, ' ') << line << std::string(padRight, ' ') << " |\n";
    }
    std::cout << border << COLOR_RESET << "\n";
}

// "Nyomj ENTER-t a folytatáshoz..." üzenet megjelenítése és várakozás ENTER-re
void UI::pause()
{
    std::cout << "(Nyomj ENTER-t a folytatashoz...)\n";
    std::cin.clear();
    std::cin.get();
}

// --- Helper input validator implementations ---
std::string UI::askValidatedName(const std::string &prompt)
{
    std::string value;
    while (true)
    {
        value = getStringInput(prompt);
        try
        {
            MenuItem::validateName(value);
            return value;
        }
        catch (const std::invalid_argument &e)
        {
            std::cout << COLOR_ERROR << e.what() << "\n"
                      << COLOR_RESET;
        }
    }
}

int UI::askValidatedPrice(const std::string &prompt)
{
    int value;
    while (true)
    {
        value = getIntInput(prompt);
        try
        {
            MenuItem::validatePrice(value);
            return value;
        }
        catch (const std::invalid_argument &e)
        {
            std::cout << COLOR_ERROR << e.what() << "\n"
                      << COLOR_RESET;
        }
    }
}

double UI::askValidatedVolume(const std::string &prompt)
{
    double value;
    while (true)
    {
        value = getDoubleInput(prompt);
        try
        {
            Drink::validateVolume(value);
            return value;
        }
        catch (const std::invalid_argument &e)
        {
            std::cout << COLOR_ERROR << e.what() << "\n"
                      << COLOR_RESET;
        }
    }
}

bool UI::askYesNo(const std::string &prompt)
{
    int v;
    while (true)
    {
        v = getIntInput(prompt);
        if (v == 0 || v == 1)
            return v != 0;
        std::cout << COLOR_ERROR << "Hiba: ervenytelen bemenet! Adj meg 1-et (Igen) vagy 0-t (Nem).\n"
                  << COLOR_RESET;
    }
}