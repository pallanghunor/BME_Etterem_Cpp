#ifndef RESTAURANT_HPP
#define RESTAURANT_HPP

#include "list.hpp"
#include "table.hpp"
#include "menuitem.hpp"
#include <string>

class Restaurant
{
private:
    List<Table> tables;
    List<MenuItem *> menu;

    int nextTableId;
    int nextMenuItemId;

    // Fájlok elérési útvonalai
    const std::string tablesFile = "asztalok.txt";
    const std::string menuFile = "etelek.txt";
    const std::string ordersFile = "orders.txt";

    static constexpr int MAX_X = 15;
    static constexpr int MAX_Y = 10;

public:
    Restaurant();

    ~Restaurant();

    // ---------------------------------------------------------
    // * Biztonsági letiltások: Ne lehessen véletlenül az egész
    // * éttermet lemásolni a memóriában!
    // ---------------------------------------------------------
    Restaurant(const Restaurant &other) = delete;
    Restaurant &operator=(const Restaurant &other) = delete;

    // Fájlok kezelése
    void loadData();
    void saveData() const;

    // Egyedi ID-k generálása új asztalokhoz és menüpontokhoz
    int getNextTableId() const { return nextTableId; }
    int getNextMenuItemId() const { return nextMenuItemId; }

    // Getterek a térkép méretéhez (UI-nak)
    int getMaxX() const { return MAX_X; }
    int getMaxY() const { return MAX_Y; }

    // Étlap kezelése
    void addMenuItem(MenuItem *item);
    void deleteMenuItem(int id);
    void printMenu() const;
    MenuItem *getMenuItemById(int id) const;

    // Asztalok és rendelések kezelése
    void addTable(int id, int seats, const std::string &desc, int x, int y);
    Table *getTableById(int id);
    void deleteTable(int id);

    bool isTableIdTaken(int id) const;
    bool isTableAt(int x, int y) const;

    void printTables() const;

    // Étterem térkép megjelenítése
    void showOccupancyMap() const;
};

#endif // RESTAURANT_HPP