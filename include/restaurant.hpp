#ifndef RESTAURANT_HPP
#define RESTAURANT_HPP

/**
 * @file restaurant.hpp
 * @brief Az étterem fő domain modellje.
 */

#include "list.hpp"
#include "table.hpp"
#include "menuitem.hpp"
#include <string>

/**
 * @brief Asztalokat és menüpontokat kezel, valamint a perzisztenciát.
 */
class Restaurant
{
private:
    List<Table> tables;
    List<MenuItem *> menu;

    int nextTableId;
    int nextMenuItemId;

    /**
    * @brief A perzisztenciához használt fájlnevek.
     */
    const std::string tablesFile = "asztalok.txt";
    const std::string menuFile = "etelek.txt";
    const std::string ordersFile = "orders.txt";

    static constexpr int MAX_X = 15;
    static constexpr int MAX_Y = 10;

public:
    /**
    * @brief Üres étteremmodell létrehozása.
     */
    Restaurant();

    /**
    * @brief Felszabadítja a tulajdonolt menüpontokat.
     */
    ~Restaurant();

    // ---------------------------------------------------------
    // * Biztonsági letiltások: Ne lehessen véletlenül az egész
    // * éttermet lemásolni a memóriában!
    // ---------------------------------------------------------
    Restaurant(const Restaurant &other) = delete;
    Restaurant &operator=(const Restaurant &other) = delete;

    /**
    * @brief Betölti az asztalokat, menü tételeket és rendeléseket.
     */
    void loadData();

    /**
    * @brief Elmenti az asztalokat, menü tételeket és rendeléseket.
     */
    void saveData() const;

    /**
    * @brief Visszaadja a következő asztalazonosítót.
     */
    int getNextTableId() const { return nextTableId; }

    /**
    * @brief Visszaadja a következő menü tételazonosítót.
     */
    int getNextMenuItemId() const { return nextMenuItemId; }

    /**
    * @brief Visszaadja a térkép maximális X koordinátáját.
     */
    int getMaxX() const { return MAX_X; }

    /**
    * @brief Visszaadja a térkép maximális Y koordinátáját.
     */
    int getMaxY() const { return MAX_Y; }

    /**
    * @brief Menü tétel hozzáadása az étteremhez.
     */
    void addMenuItem(MenuItem *item);

    /**
    * @brief Menü tétel törlése azonosító alapján.
     */
    void deleteMenuItem(int id);

    /**
    * @brief Visszaadja a menü tételt azonosító alapján, vagy nullptr-t.
     */
    MenuItem *getMenuItemById(int id) const;
    
    /**
    * @brief Asztal hozzáadása az étteremhez.
     */
    void addTable(int id, int seats, const std::string &desc, int x, int y);

    /**
    * @brief Visszaadja az asztalt azonosító alapján, vagy nullptr-t.
     */
    Table *getTableById(int id);

    /**
    * @brief Asztal törlése azonosító alapján.
     */
    void deleteTable(int id);
    
    /**
    * @brief Visszaadja, hogy az asztalazonosító foglalt-e már.
     */
    bool isTableIdTaken(int id) const;

    /**
    * @brief Visszaadja, hogy van-e már asztal az adott helyen.
     */
    bool isTableAt(int x, int y) const;
    
    /**
    * @brief Kiírja a jelenlegi étlapot.
     */
    void printMenu(std::ostream &os) const;

    /**
    * @brief Kiírja a jelenlegi asztallistát.
     */
    void printTables(std::ostream &os) const;

    /**
    * @brief Kiírja az étterem foglaltsági térképét.
     */
    void showOccupancyMap(std::ostream &os) const;
};

#endif // RESTAURANT_HPP