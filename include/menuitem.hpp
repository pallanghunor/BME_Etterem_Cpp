#ifndef MENUITEM_HPP
#define MENUITEM_HPP

#include <string>
#include <iostream>

class Food;
class Drink;

// Látogató (Visitor) interfész
class MenuItemVisitor {
public:
    virtual void visitFood(Food* food) = 0;
    virtual void visitDrink(Drink* drink) = 0;
    virtual ~MenuItemVisitor() = default;
};

class MenuItem
{
protected:
    int id;
    std::string name;
    int price;
    bool available;

public:
    MenuItem() : id(0), price(0), available(false) {}
    MenuItem(int id, const std::string &name, int price, bool available);

    virtual ~MenuItem() {};

    // Getters
    int getId() const;
    int getPrice() const;
    std::string getName() const;
    
    // Setters
    void setName(const std::string &name);
    void setPrice(int price);
    void setAvailable(bool available);
    
    bool isAvailable() const;

    // Validációs függvények
    static void validateName(const std::string& name);
    static void validatePrice(int price);

    virtual void print(std::ostream &os) const = 0;
    virtual void save(std::ostream &os) const = 0;

    virtual MenuItem *clone() const = 0;

    virtual void read(std::istream &is) = 0;

    // Látogató minta elfogadó metódusa
    virtual void accept(MenuItemVisitor& visitor) = 0;
};

#endif // MENUITEM_HPP