#ifndef DRINK_HPP
#define DRINK_HPP

#include "menuitem.hpp"

class Drink : public MenuItem
{
private:
    double volume;
    bool isAlcoholic;

public:
    Drink() = default;
    Drink(int id, const std::string &name, int price, bool available, double volume, bool isAlcoholic);
    
    bool getIsAlcoholic() const;
    
    void setVolume(double volume);
    void setAlcoholic(bool isAlcoholic);
    
    static void validateVolume(double volume);
    
    void print(std::ostream &os) const override;
    void save(std::ostream &os) const override;
    MenuItem *clone() const override;

    void read(std::istream &is) override;
};

#endif // DRINK_HPP