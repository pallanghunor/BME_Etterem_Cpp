#ifndef TABLE_HPP
#define TABLE_HPP

/**
 * @file table.hpp
 * @brief Asztal modell deklarációja.
 */

#include <string>
#include <iostream>
#include <memory> // Smart pointer használatához
#include "order.hpp"

/**
 * @brief Egy éttermi asztalt és az esetleges aktív rendelését reprezentálja.
 */
class Table
{
public:
    /**
     * @brief A megengedett minimális férőhelyszám.
     */
    static constexpr int MIN_SEATS = 2;

private:
    int id;
    int seats;
    std::string description;
    int x;
    int y;
    bool occupied;

    /**
     * @brief Egy unique_ptr-en keresztül birtokolja az aktív rendelést.
     */
    std::unique_ptr<Order> currentOrder;

public:
    /**
     * @brief Asztal létrehozása a megadott tulajdonságokkal.
     */
    Table(int id, int seats, const std::string &description, int x, int y);

    /**
     * @brief Ellenőrzi a férőhelyszámot.
     */
    static void validateSeats(int seats);

    /**
     * @brief Másoló konstruktor, amely mély másolatot készít a rendelésről.
     */
    Table(const Table &other);

    /**
     * @brief Értékadó operátor, amely mély másolatot készít a rendelésről.
     */
    Table &operator=(const Table &other);

    /**
     * @brief Visszaadja az asztal azonosítóját.
     */
    int getId() const;

    /**
     * @brief Visszaadja a férőhelyek számát.
     */
    int getSeats() const;

    /**
     * @brief Visszaadja a leírást.
     */
    std::string getDescription() const;

    /**
     * @brief Visszaadja az X koordinátát.
     */
    int getX() const;

    /**
     * @brief Visszaadja a Y koordinátát.
     */
    int getY() const;

    /**
     * @brief Visszaadja az aktív rendelést, vagy nullptr-t.
     */
    Order *getOrder() const;

    /**
     * @brief Beállítja az asztal azonosítóját.
     */
    void setId(int id);

    /**
     * @brief Beállítja a férőhelyek számát.
     */
    void setSeats(int seats);

    /**
     * @brief Beállítja az asztal leírását.
     */
    void setDescription(const std::string &description);

    /**
     * @brief Beállítja az asztal pozícióját.
     */
    void setPosition(int x, int y);

    /**
     * @brief Beállítja a foglalt jelzőt, ha nincs aktív rendelés.
     */
    void setOccupied(bool occupied);

    /**
     * @brief Visszaadja, hogy az asztal foglalt-e.
     */
    bool isOccupied() const;

    /**
     * @brief Visszaadja, hogy az asztalhoz tartozik-e aktív rendelés.
     */
    bool hasActiveOrder() const;

    /**
     * @brief Új rendelést nyit, ha még nincs.
     */
    void openOrder();

    /**
     * @brief Tételt ad az aktív rendeléshez.
     */
    void addItemToOrder(MenuItem *item, int qty);

    /**
     * @brief Lezárja az asztalt és kiírja a nyugtát.
     */
    void closeTable(std::ostream &os);

    /**
     * @brief Kiírja az asztal adatait.
     */
    void printTable(std::ostream &os) const;
};

#endif // TABLE_HPP