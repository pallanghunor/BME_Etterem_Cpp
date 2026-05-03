#ifndef ORDERITEM_HPP
#define ORDERITEM_HPP

#include "menuitem.hpp"

class OrderItem {
private:
    MenuItem* item;
    int quantity;

public:
    OrderItem(MenuItem* item, int quantity);

    MenuItem* getItem() const;
    int getQuantity() const;
    void setQuantity(int qty);

    int getSubtotal() const;
};

#endif // ORDERITEM_HPP