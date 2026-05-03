#ifndef RESTAURANT_HPP
#define RESTAURANT_HPP

#include "list.hpp"
#include "table.hpp"
#include "menuitem.hpp"
#include <string>

class Restaurant {
private:
    List<Table> tables;
    List<MenuItem*> menu;

    // Fájlok elérési útvonalai
    const std::string tablesFile = "asztalok.txt";
    const std::string menuFile = "etelek.txt";
    const std::string ordersFile = "orders.txt";

public:
    Restaurant();

    ~Restaurant();

    // ---------------------------------------------------------
    // * Biztonsági letiltások: Ne lehessen véletlenül az egész 
    // * éttermet lemásolni a memóriában!
    // ---------------------------------------------------------
    Restaurant(const Restaurant& other) = delete;
    Restaurant& operator=(const Restaurant& other) = delete;

    // Fájlok kezelése
    void loadData();
    void saveData() const;

    // Étlap kezelése
    void addMenuItem(MenuItem* item);
    void deleteMenuItem(int id);
    void printMenu() const;
    MenuItem* getMenuItemById(int id) const;

    // Asztalok és rendelések kezelése
    void addTable(int id, int seats, const std::string& desc, int x, int y);
    Table* getTableById(int id);
    void showOccupancyMap() const; // Étterem térkép megjelenítése
};

#endif // RESTAURANT_HPP