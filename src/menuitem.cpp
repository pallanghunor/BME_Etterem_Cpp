#include "menuitem.hpp"
#include <stdexcept>

#include "memtrace.h"

MenuItem::MenuItem(int id, const std::string &name, int price, bool available) : id(id), price(0), available(available)
{
    setName(name);
    setPrice(price);
}

int MenuItem::getId() const { return id; }
int MenuItem::getPrice() const { return price; }
std::string MenuItem::getName() const { return name; }
bool MenuItem::isAvailable() const { return available; }

void MenuItem::setName(const std::string &name)
{
    validateName(name);
    this->name = name;
}

void MenuItem::setPrice(int price)
{
    validatePrice(price);
    this->price = price;
}

void MenuItem::setAvailable(bool available) { this->available = available; }

void MenuItem::validateName(const std::string &name)
{
    if (name.find_first_not_of(" \t\r\n") == std::string::npos)
    {
        throw std::invalid_argument("Hiba: a nev nem lehet ures!");
    }
}

void MenuItem::validatePrice(int price)
{
    if (price < 0)
    {
        throw std::invalid_argument("Hiba: az ar nem lehet kisebb 0-nal!");
    }
}