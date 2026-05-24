#include "order.hpp"

#include "memtrace.h"

void Order::addItem(MenuItem* item, int qty) {
    if (item == nullptr || qty <= 0) return;

    // Megnézzük, van-e már ilyen tétel. Ha igen, csak a darabszámot növeljük.
    for (auto it = items.begin(); it != items.end(); ++it) {
        if ((*it).getItem() == item) {
            (*it).setQuantity((*it).getQuantity() + qty);
            return;
        }
    }

    // Ha még nem volt, új elemként a lista végére fűzzük
    items.push_back(OrderItem(item, qty));
}

void Order::removeItem(MenuItem* item) {
    for (auto it = items.begin(); it != items.end(); ++it) {
        if ((*it).getItem() == item) {
            items.erase(it);
            return;
        }
    }
}

void Order::updateItemQuantity(MenuItem* item, int newQty) {
    if (item == nullptr) return;

    if (newQty <= 0) {
        removeItem(item);
        return;
    }

    for (auto it = items.begin(); it != items.end(); ++it) {
        if ((*it).getItem() == item) {
            (*it).setQuantity(newQty);
            return;
        }
    }
}

bool Order::containsItem(MenuItem* item) const {
    for (auto it = items.begin(); it != items.end(); ++it) {
        if ((*it).getItem() == item) {
            return true;
        }
    }
    return false;
}

int Order::getTotal() const {
    int total = 0;
    for (const auto& orderItem : items) {
        total += orderItem.getSubtotal();
    }
    return total;
}

void Order::printReceipt(std::ostream& os) const {
    os << "--------------------------------\n";
    os << "          FOGYASZTAS            \n";
    os << "--------------------------------\n";
    
    for (const auto& orderItem : items) {
        MenuItem* mItem = orderItem.getItem();
        if (mItem) {
            os << mItem->getName() << " x " << orderItem.getQuantity() << " db = " << orderItem.getSubtotal() << " Ft\n";
        }
    }
    os << "--------------------------------\n";
    os << "VEGOSSZEG: " << getTotal() << " Ft\n";
    os << "--------------------------------\n";
}

const List<OrderItem>& Order::getItems() const {
    return items;
}