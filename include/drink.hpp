#ifndef DRINK_HPP
#define DRINK_HPP

#include "menuitem.hpp"

class Drink : public MenuItem {
private:
    double volume;
    bool isAlcoholic;

public:
    Drink(int id, const std::string& name, int price, bool available, double volume, bool isAlcoholic);

    void print(std::ostream& os) const override;
    void save(std::ostream& os) const override;
    MenuItem* clone() const override;
};

#endif // DRINK_HPP