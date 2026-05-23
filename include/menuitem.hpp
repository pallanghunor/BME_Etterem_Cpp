#ifndef MENUITEM_HPP
#define MENUITEM_HPP

#include <string>
#include <iostream>

class MenuItem {
protected:
    int id;
    std::string name;
    int price;
    bool available;

public:
    MenuItem() : id(0), price(0), available(false) {}
    MenuItem(int id, const std::string& name, int price, bool available);
    
    virtual ~MenuItem() {};

    int getId() const;
    int getPrice() const;
    std::string getName() const;

    virtual void print(std::ostream& os) const = 0;
    virtual void save(std::ostream& os) const = 0;
    
    virtual MenuItem* clone() const = 0;

    virtual void read(std::istream& is) = 0;
};

#endif // MENUITEM_HPP