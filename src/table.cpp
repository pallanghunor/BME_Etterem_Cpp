/**
 * @file table.cpp
 * @brief Asztal modell megvalósítás.
 */

#include "table.hpp"
#include <stdexcept>

#include "memtrace.h"

Table::Table(int id, int seats, const std::string &description, int x, int y)
    : id(id), seats(0), description(description), x(x), y(y), occupied(false), currentOrder(nullptr)
{
    validateSeats(seats);
    this->seats = seats;
}

// Másoló konstruktor: Mély másolás (Deep copy)
Table::Table(const Table &other)
    : id(other.id), seats(other.seats), description(other.description),
      x(other.x), y(other.y), occupied(other.occupied), currentOrder(nullptr)
{

    // Ha a másik asztalnak van rendelése, lemásoljuk azt is
    if (other.currentOrder != nullptr)
    {
        currentOrder.reset(new Order(*(other.currentOrder)));
    }
}

// Értékadó operátor
Table &Table::operator=(const Table &other)
{
    if (this != &other)
    {
        id = other.id;
        seats = other.seats;
        description = other.description;
        x = other.x;
        y = other.y;
        occupied = other.occupied;

        if (other.currentOrder != nullptr)
        {
            currentOrder.reset(new Order(*(other.currentOrder)));
        }
        else
        {
            currentOrder.reset();
        }
    }
    return *this;
}

// Getters
int Table::getId() const { return id; }
int Table::getSeats() const { return seats; }
std::string Table::getDescription() const { return description; }
int Table::getX() const { return x; }
int Table::getY() const { return y; }
bool Table::isOccupied() const { return occupied; }
void Table::printTable(std::ostream &os) const
{
    os << "[ID: " << id << "] "
       << seats << " fos asztal (" << description << ") "
       << "| Hely: X:" << x << " Y:" << y << " "
       << "| Allapot: " << (isOccupied() ? "FOGLALT" : "SZABAD");
}

// Setters

void Table::setSeats(int seats)
{
    validateSeats(seats);
    this->seats = seats;
}

void Table::validateSeats(int seats)
{
    if (seats < MIN_SEATS)
    {
        throw std::invalid_argument("Egy asztalnak legalabb " + std::to_string(MIN_SEATS) + " ferohelyesnek kell lennie!");
    }
}

void Table::setDescription(const std::string &description)
{
    this->description = description;
}

void Table::setPosition(int x, int y)
{
    this->x = x;
    this->y = y;
}

void Table::setOccupied(bool occupied)
{
    if (hasActiveOrder())
    {
        throw std::logic_error("A foglaltsag nem modosithato, mert az asztalhoz aktiv rendeles tartozik!");
    }
    this->occupied = occupied;
}

// Rendelés menedzselése
void Table::openOrder()
{
    if (!currentOrder)
    {
        currentOrder.reset(new Order());
        occupied = true;
    }
}

void Table::addItemToOrder(MenuItem *item, int qty)
{
    if (currentOrder)
    {
        currentOrder->addItem(item, qty);
    }
}

void Table::closeTable(std::ostream &os)
{
    if (currentOrder)
    {
        currentOrder->printReceipt(os);
        currentOrder.reset(nullptr);
    }

    occupied = false;
}

bool Table::hasActiveOrder() const
{
    return currentOrder != nullptr;
}

Order *Table::getOrder() const
{
    return currentOrder.get();
}