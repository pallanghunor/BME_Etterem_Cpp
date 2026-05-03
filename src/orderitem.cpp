#include "orderitem.hpp"

OrderItem::OrderItem(MenuItem* item, int quantity) : item(item), quantity(quantity) {}

MenuItem* OrderItem::getItem() const {
    return item;
}

int OrderItem::getQuantity() const {
    return quantity;
}

void OrderItem::setQuantity(int qty) {
    quantity = qty;
}

int OrderItem::getSubtotal() const {
    if (item == nullptr) return 0;
    return item->getPrice() * quantity;
}