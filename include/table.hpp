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

    // Getterek
    int getId() const;
    int getSeats() const;
    std::string getDescription() const;
    int getX() const;
    int getY() const;
    Order *getOrder() const;
    
    // Setterek
    void setId(int id);
    void setSeats(int seats);
    void setDescription(const std::string &description);
    void setPosition(int x, int y);
    void setOccupied(bool occupied);
    
    // Állapot lekérdezése
    bool isOccupied() const;
    bool hasActiveOrder() const;

    // Rendelés menedzselése
    void openOrder();
    void addItemToOrder(MenuItem *item, int qty);
    
    // Asztal lezárása és számla kiírása a megadott kimenetre
    void closeTable(std::ostream &os);
    
    // Asztal információinak kiírása
    void printTable(std::ostream &os) const;
};

#endif // TABLE_HPP