#include "drink.hpp"
#include "util.hpp"

#include "memtrace.h"

Drink::Drink(int id, const std::string& name, int price, bool available, double volume, bool isAlcoholic) : MenuItem(id, name, price, available), volume(volume), isAlcoholic(isAlcoholic) {}

void Drink::print(std::ostream& os) const {
    os << "[Ital] " << name << " (" << volume << " l) | Ar: " << price << " Ft";
    if (isAlcoholic) os << " (Alkoholos)";
    if (!available) os << " (Nincs keszleten)";
    os << "\n";
}

void Drink::save(std::ostream& os) const {
    // I;ID;Név;Ár;Elérhető;Űrtartalom;Alkoholos formátum
    os << "I;" << id << ";" << name << ";" << price << ";" << available << ";" << volume << ";" << isAlcoholic << "\n";
}

MenuItem* Drink::clone() const {
    return new Drink(*this);
}

void Drink::read(std::istream& is) {
    std::string temp;
    std::getline(is, temp, ';'); id = std::stoi(temp);
    std::getline(is, name, ';');
    std::getline(is, temp, ';'); price = std::stoi(temp);
    std::getline(is, temp, ';'); available = std::stoi(temp);
    
    // Űrtartalom
    std::getline(is, temp, ';'); volume = std::stod(temp);
    
    // Alkoholos-e (sor vége)
    std::getline(is, temp);
    trimCR(temp);
    isAlcoholic = std::stoi(temp);
}