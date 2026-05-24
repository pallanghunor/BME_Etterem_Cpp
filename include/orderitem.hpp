#ifndef ORDERITEM_HPP
#define ORDERITEM_HPP

#include "menuitem.hpp"

class OrderItem {
private:
    MenuItem* item;
    int quantity;

public:
    OrderItem(MenuItem* item, int quantity);

    // Getterek
    MenuItem* getItem() const;
    int getQuantity() const;
    int getSubtotal() const;

    // Setterek
    void setQuantity(int qty);

};

#endif // ORDERITEM_HPP