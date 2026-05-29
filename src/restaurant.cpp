/**
 * @file restaurant.cpp
 * @brief Az étterem fő modelljének megvalósítás.
 */

#include "restaurant.hpp"
#include "food.hpp"
#include "drink.hpp"
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "memtrace.h"

Restaurant::Restaurant() : nextTableId(1), nextMenuItemId(1) {}

Restaurant::~Restaurant()
{
    // Felszabadítjuk a dinamikusan foglalt MenuItem-eket
    for (auto it = menu.begin(); it != menu.end(); ++it)
    {
        delete (*it);
    }
}

void Restaurant::addMenuItem(MenuItem *item)
{
    if (item != nullptr)
    {
        menu.push_back(item);
        nextMenuItemId = std::max(nextMenuItemId, item->getId() + 1);
    }
}

// Biztonságos törlés (Dangling pointer védelem)
void Restaurant::deleteMenuItem(int id)
{
    // 1. Megkeressük a törlendő elemet az étlapon
    MenuItem *itemToDelete = nullptr;
    auto menuIt = menu.begin();

    for (; menuIt != menu.end(); ++menuIt)
    {
        if ((*menuIt)->getId() == id)
        {
            itemToDelete = *menuIt;
            break;
        }
    }

    if (itemToDelete == nullptr)
    {
        throw std::invalid_argument("Nincs ilyen ID-ju tetel az etlapon!");
    }

    // 2. Biztonsági ellenőrzés: Használja-e valamelyik asztal ezt az elemet?
    for (auto tableIt = tables.begin(); tableIt != tables.end(); ++tableIt)
    {
        if ((*tableIt).hasActiveOrder())
        {
            Order *order = (*tableIt).getOrder();
            if (order->containsItem(itemToDelete))
            {
                // Ha igen, kivételt dobunk, nem engedjük a törlést!
                throw std::logic_error("A tetel nem torolheto, mert egy aktiv rendelesben szerepel!");
            }
        }
    }

    menu.erase(menuIt);  // Eltávolítjuk a mutatót a listából
    delete itemToDelete; // Felszabadítjuk magát a memóriát
}

void Restaurant::printMenu(std::ostream &os) const
{
    if (menu.getSize() == 0)
    {
        os << "Az etlap jelenleg ures.\n";
        return;
    }
    os << "--- ELERHETO ETLAP ---\n\n";
    for (auto it = menu.begin(); it != menu.end(); ++it)
    {
        (*it)->print(os);
    }
    os << "\n----------------------\n";
}

MenuItem *Restaurant::getMenuItemById(int id) const
{
    for (auto it = menu.begin(); it != menu.end(); ++it)
    {
        if ((*it)->getId() == id)
        {
            return *it;
        }
    }
    return nullptr;
}

void Restaurant::addTable(int id, int seats, const std::string &desc, int x, int y)
{
    if (isTableIdTaken(id))
    {
        throw std::invalid_argument("Ez az asztal ID mar letezik!");
    }
    if (isTableAt(x, y))
    {
        throw std::invalid_argument("Erre a koordinatara mar van asztal rogzitve!");
    }

    tables.push_back(Table(id, seats, desc, x, y));

    nextTableId = std::max(nextTableId, id + 1);
}

Table *Restaurant::getTableById(int id)
{
    for (auto it = tables.begin(); it != tables.end(); ++it)
    {
        if ((*it).getId() == id)
        {
            return &(*it);
        }
    }
    return nullptr;
}

void Restaurant::deleteTable(int id)
{
    for (auto it = tables.begin(); it != tables.end(); ++it)
    {
        if ((*it).getId() == id)
        {

            // Biztonsági ellenőrzés: Van aktív rendelés az asztalon?
            if ((*it).hasActiveOrder())
            {
                throw std::logic_error("Az asztal nem torolheto, mert epp aktiv rendelessel rendelkezik!");
            }

            // Ha nincs rendelés, az erase kiveszi a List-ből
            tables.erase(it);
            return;
        }
    }

    throw std::invalid_argument("Nincs ilyen ID-ju asztal a rendszerben!");
}

bool Restaurant::isTableIdTaken(int id) const
{
    for (auto it = tables.begin(); it != tables.end(); ++it)
    {
        if ((*it).getId() == id)
            return true;
    }
    return false;
}

bool Restaurant::isTableAt(int x, int y) const
{
    for (auto it = tables.begin(); it != tables.end(); ++it)
    {
        if ((*it).getX() == x && (*it).getY() == y)
            return true;
    }
    return false;
}

void Restaurant::printTables(std::ostream &os) const
{
    if (tables.getSize() == 0)
    {
        os << "Jelenleg nincsenek asztalok a rendszerben.\n";
        return;
    }

    for (auto it = tables.begin(); it != tables.end(); ++it)
    {
        (*it).printTable(os);
        os << "\n";
    }
}

void Restaurant::loadData()
{
    // 1. Étlap betöltése
    std::ifstream mFile(menuFile);
    if (!mFile.is_open())
        return;

    std::string type;
    int lineNum = 0;

    // A típust jelző karakter kiolvasása pontosvesszőig (pl. "E" vagy "I")
    while (std::getline(mFile, type, ';'))
    {
        lineNum++;

        // Üres sorok és extra sortörések elleni védelem
        if (type.length() > 0 && type[0] == '\n')
            type.erase(0, 1);
        if (type.length() > 0 && type[0] == '\r')
            type.erase(0, 1);
        if (type.empty())
            continue;

        MenuItem *item = nullptr;

        // 1. Lépés: Példányosítás, típus alapján
        if (type == "E")
        {
            item = new Food();
        }
        else if (type == "I")
        {
            item = new Drink();
        }

        if (item != nullptr)
        {
            try
            {
                // 2. Lépés: Az objektum beolvassa a saját maradék adatait a fájlból
                item->read(mFile);

                // 3. Lépés: Hozzáadás a heterogén listához
                addMenuItem(item);
            }
            catch (const std::exception &e)
            {
                std::cerr << "Hiba: Etlap elem beolvasasa sikertelen (sor " << lineNum << "):\\n"
                          << "  Tipus: '" << type << "'\\n"
                          << "  Oka: " << e.what() << std::endl;
                delete item;
            }
        }
        else
        {
            // Ismeretlen típus kezelése
            std::string discard;
            std::getline(mFile, discard);
            std::cerr << "Hiba: Etlap elem beolvasasa sikertelen (sor " << lineNum << "):\\n"
                      << "  Ismeretlen tipus: '" << type << "'\\n"
                      << "  Elengedett adat: '" << discard << "'\\n"
                      << "  Megjegyzes: Csak 'E' (etel) vagy 'I' (ital) tipusok ervenyek." << std::endl;
        }
    }
    mFile.close();

    // 2. Asztalok betöltése
    std::ifstream tFile(tablesFile);
    if (tFile.is_open())
    {
        std::string line;
        int lineNum = 0;
        while (std::getline(tFile, line))
        {
            lineNum++;
            trimCR(line);
            if (line.empty())
                continue;

            std::stringstream ss(line);
            std::string idStr, seatsStr, desc, xStr, yStr, occStr;

            std::getline(ss, idStr, ';');
            std::getline(ss, seatsStr, ';');
            std::getline(ss, desc, ';');
            std::getline(ss, xStr, ';');
            std::getline(ss, yStr, ';');

            try
            {
                addTable(std::stoi(idStr), std::stoi(seatsStr), desc, std::stoi(xStr), std::stoi(yStr));
            }
            catch (const std::exception &e)
            {
                std::cerr << "Hiba: Asztal betoltese sikertelen (sor " << lineNum << "):\n"
                          << "  Adat: \"" << line << "\"\n"
                          << "  Mezo ertekek: ID=\"" << idStr << "\", Ferohelyek=\"" << seatsStr
                          << "\", Leiras=\"" << desc << "\", X=\"" << xStr << "\", Y=\"" << yStr << "\"\n"
                          << "  Oka: " << e.what() << std::endl;
            }
        }
        tFile.close();
    }

    // 3. Rendelések betöltése
    std::ifstream oFile(ordersFile);
    if (oFile.is_open())
    {
        std::string line;
        int lineNum = 0;
        while (std::getline(oFile, line))
        {
            lineNum++;
            trimCR(line);
            if (line.empty())
                continue;

            std::stringstream ss(line);
            std::string tIdStr, mIdStr, qtyStr;

            std::getline(ss, tIdStr, ';');
            std::getline(ss, mIdStr, ';');
            std::getline(ss, qtyStr, ';');

            try
            {
                int tId = std::stoi(tIdStr);
                int mId = std::stoi(mIdStr);
                int qty = std::stoi(qtyStr);

                // A memóriacímek megkeresése (Linking)
                Table *t = getTableById(tId);
                MenuItem *item = getMenuItemById(mId);

                if (t != nullptr && item != nullptr)
                {
                    t->openOrder(); // Biztosítjuk, hogy az asztal "Foglalt" legyen
                    t->addItemToOrder(item, qty);
                }
                else if (t == nullptr)
                {
                    std::cerr << "Figyelmeztetés: Asztal betöltése sikertelen (sor " << lineNum
                              << "): Asztal ID \"" << tIdStr << "\" nem található." << std::endl;
                }
                else if (item == nullptr)
                {
                    std::cerr << "Figyelmeztetés: Rendelés tétel betöltése sikertelen (sor " << lineNum
                              << "): Étlap elem ID \"" << mIdStr << "\" nem található." << std::endl;
                }
            }
            catch (const std::exception &e)
            {
                std::cerr << "Hiba: Rendelés betöltése sikertelen (sor " << lineNum << "):\n"
                          << "  Adat: \"" << line << "\"\n"
                          << "  Mező értékek: AsztalID=\"" << tIdStr << "\", ÉtelID=\"" << mIdStr
                          << "\", Darab=\"" << qtyStr << "\"\n"
                          << "  Oka: " << e.what() << std::endl;
            }
        }
        oFile.close();
    }
}

void Restaurant::saveData() const
{
    // 1. Étlap kimentése
    std::ofstream mFile(menuFile);
    if (mFile.is_open())
    {
        for (auto it = menu.begin(); it != menu.end(); ++it)
        {
            (*it)->save(mFile);
        }
        mFile.close();
    }
    else
    {
        throw std::runtime_error("Nem sikerult megnyitni a menufajlt mentesre!");
    }

    // 2. Asztalok kimentése
    std::ofstream tFile(tablesFile);
    if (tFile.is_open())
    {
        for (auto it = tables.begin(); it != tables.end(); ++it)
        {
            // Formátum: ID;Férőhely;Leírás;X;Y;Foglalt-e
            tFile << (*it).getId() << ";"
                  << (*it).getSeats() << ";"
                  << (*it).getDescription() << ";"
                  << (*it).getX() << ";"
                  << (*it).getY() << ";"
                  << (*it).isOccupied() << "\n";
        }
        tFile.close();
    }
    else
    {
        throw std::runtime_error("Nem sikerult megnyitni az asztalfajlt mentesre!");
    }

    // 3. Rendelések kimentése (Csak az aktív rendelések)
    std::ofstream oFile(ordersFile);
    if (oFile.is_open())
    {
        for (auto it = tables.begin(); it != tables.end(); ++it)
        {
            if ((*it).hasActiveOrder())
            {
                Order *order = (*it).getOrder();
                // Végigmegyünk az asztal rendelésének tételein
                for (const auto &orderItem : order->getItems())
                {
                    if (orderItem.getItem() != nullptr)
                    {
                        // Formátum: AsztalID;ÉtelID;Darab
                        oFile << (*it).getId() << ";"
                              << orderItem.getItem()->getId() << ";"
                              << orderItem.getQuantity() << "\n";
                    }
                }
            }
        }
        oFile.close();
    }
    else
    {
        throw std::runtime_error("Nem sikerult megnyitni a rendelesfajlt mentesre!");
    }
}

void Restaurant::showOccupancyMap(std::ostream &os) const
{
    if (tables.getSize() == 0)
    {
        os << "Jelenleg nincsenek asztalok az etteremben.\n";
        return;
    }

    // 2. ANSI Színkódok
    const std::string COLOR_RED = "\x1b[31m";
    const std::string COLOR_GREEN = "\x1b[32m";
    const std::string COLOR_RESET = "\x1b[0m";

    os << "\n=== FOGLALTSAGI TERKEP ===\n\n";

    // 3. A rács kirajzolása (Y és X koordináták szerint)
    for (int y = 0; y <= MAX_Y; y++)
    {
        for (int x = 0; x <= MAX_X; x++)
        {

            bool foundTable = false;

            for (auto it = tables.begin(); it != tables.end(); ++it)
            {
                if ((*it).getX() == x && (*it).getY() == y)
                {

                    // Szín beállítása foglaltság alapján
                    if ((*it).isOccupied())
                    {
                        os << COLOR_RED;
                    }
                    else
                    {
                        os << COLOR_GREEN;
                    }

                    // Azonosító kiírása mindig 2 karakter hosszan (pl. [01], [12])
                    os << "["
                       << std::setw(2) << std::setfill('0') << (*it).getId()
                       << "]" << COLOR_RESET;

                    foundTable = true;
                    break;
                }
            }

            if (!foundTable)
            {
                os << " .. ";
            }
        }
        os << "\n";
    }

    os << "==========================\n";
    os << "Jelmagyarazat: "
       << COLOR_GREEN << "[01]" << COLOR_RESET << " = Szabad, "
       << COLOR_RED << "[01]" << COLOR_RESET << " = Foglalt\n";
}