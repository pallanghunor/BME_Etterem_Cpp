/**
 * @file food.cpp
 * @brief Étel menüpont megvalósítás.
 */

#include "food.hpp"
#include "util.hpp"

#include "memtrace.h"

Food::Food(int id, const std::string &name, int price, bool available, const std::string &allergens) : MenuItem(id, name, price, available), allergens(allergens) {}

void Food::setAllergens(const std::string& allergens) {
    this->allergens = allergens;
}

void Food::print(std::ostream &os) const{
    os << "[ID: " << id << "] [Etel] " << name << " | Ar: " << price << " Ft | Allergenek: " << allergens;
    if (!available)
        os << " (Nincs keszleten)";
    os << "\n";
}

void Food::save(std::ostream &os) const
{
    // E;ID;Név;Ár;Elérhető;Allergének formátum a fájlba mentéshez
    os << "E;" << id << ";" << name << ";" << price << ";" << available << ";" << allergens << "\n";
}

MenuItem *Food::clone() const
{
    return new Food(*this); // Létrehoz egy pontos másolatot saját magáról (deep copy előkészítés)
}

void Food::read(std::istream &is)
{
    std::string temp;

    // ID beolvasása (pontosvesszőig)
    std::getline(is, temp, ';');
    id = std::stoi(temp);

    // Név
    std::getline(is, name, ';');

    // Ár
    std::getline(is, temp, ';');
    price = std::stoi(temp);

    // Elérhetőség
    std::getline(is, temp, ';');
    available = std::stoi(temp);

    // Allergének (Ez a sor VÉGE, itt már nem pontosvesszőig, hanem újsorig olvasunk!)
    std::getline(is, allergens);
    trimCR(allergens); // Jporta \r levágása
}

void Food::accept(MenuItemVisitor& visitor) {
    visitor.visitFood(this); 
}