#ifndef TABLE_HPP
#define TABLE_HPP

#include <string>
#include <iostream>
#include <memory> // Smart pointer használatához
#include "order.hpp"

class Table
{
public:
    static constexpr int MIN_SEATS = 2;

private:
    int id;
    int seats;
    std::string description;
    int x;
    int y;
    bool occupied;

    // Nyers mutató helyett okos mutató
    std::unique_ptr<Order> currentOrder;

public:
    Table(int id, int seats, const std::string &description, int x, int y);
    static void validateSeats(int seats);

    // ~Table(); // Nem szükséges, mert a unique_ptr automatikusan kezeli a memóriát

    Table(const Table &other);
    Table &operator=(const Table &other);

    // Getters
    int getId() const;
    int getSeats() const;
    std::string getDescription() const;
    int getX() const;
    int getY() const;
    bool isOccupied() const;
    void printTable(std::ostream &os) const;

    void setId(int id);
    void setSeats(int seats);
    void setDescription(const std::string &description);
    void setPosition(int x, int y);
    void setOccupied(bool occupied);

    // Rendelés menedzselése
    void openOrder();
    void addItemToOrder(MenuItem *item, int qty);

    // Asztal lezárása és számla kiírása a megadott kimenetre
    void closeTable(std::ostream &os);

    bool hasActiveOrder() const;
    Order *getOrder() const;
};

#endif // TABLE_HPP