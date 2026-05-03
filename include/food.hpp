#ifndef FOOD_HPP
#define FOOD_HPP

#include "menuitem.hpp"

class Food : public MenuItem {
private:
    std::string allergens;

public:
    Food(int id, const std::string& name, int price, bool available, const std::string& allergens);

    void print(std::ostream& os) const override;
    void save(std::ostream& os) const override;
    MenuItem* clone() const override;
};

#endif // FOOD_HPP