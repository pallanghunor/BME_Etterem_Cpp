#include "drink.hpp"

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