/**
 * @file drink.cpp
 * @brief Ital menüpont megvalósítás.
 */

#include "drink.hpp"
#include "util.hpp"
#include <stdexcept>

#include "memtrace.h"

Drink::Drink(int id, const std::string &name, int price, bool available, double volume, bool isAlcoholic) : MenuItem(id, name, price, available), volume(0), isAlcoholic(isAlcoholic)
{
    setVolume(volume);
}

bool Drink::getIsAlcoholic() const
{
    return isAlcoholic;
}

void Drink::setVolume(double volume)
{
    validateVolume(volume);
    this->volume = volume;
}

void Drink::setAlcoholic(bool isAlcoholic)
{
    this->isAlcoholic = isAlcoholic;
}

void Drink::validateVolume(double volume)
{
    if (volume <= 0)
    {
        throw std::invalid_argument("Hiba: az urtartalomnak nagyobbnak kell lennie 0-nal!");
    }
}

void Drink::print(std::ostream &os) const
{
    os << "[ID: " << id << "] [Ital] " << name << " (" << volume << " l) | Ar: " << price << " Ft";
    if (isAlcoholic)
        os << " (Alkoholos)";
    if (!available)
        os << " (Nincs keszleten)";
    os << "\n";
}

void Drink::save(std::ostream &os) const
{
    // I;ID;Név;Ár;Elérhető;Űrtartalom;Alkoholos formátum
    os << "I;" << id << ";" << name << ";" << price << ";" << available << ";" << volume << ";" << isAlcoholic << "\n";
}

MenuItem *Drink::clone() const
{
    return new Drink(*this);
}

void Drink::read(std::istream &is)
{
    std::string temp;
    std::getline(is, temp, ';');
    id = std::stoi(temp);
    std::getline(is, name, ';');
    std::getline(is, temp, ';');
    price = std::stoi(temp);
    std::getline(is, temp, ';');
    available = std::stoi(temp);

    // Űrtartalom
    std::getline(is, temp, ';');
    setVolume(std::stod(temp));

    // Alkoholos-e (sor vége)
    std::getline(is, temp);
    trimCR(temp);
    isAlcoholic = std::stoi(temp);
}

void Drink::accept(MenuItemVisitor& visitor) {
    visitor.visitDrink(this); 
}