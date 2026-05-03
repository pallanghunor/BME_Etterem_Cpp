#ifndef ORDER_HPP
#define ORDER_HPP

#include "list.hpp"
#include "orderitem.hpp"
#include <iostream>

class Order {
private:
    List<OrderItem> items;

public:
    Order() {};

    // Tétel hozzáadása a rendeléshez
    void addItem(MenuItem* item, int qty);

    // Tétel eltávolítása a rendelésből
    void removeItem(MenuItem* item);

    // Szerepel-e már az adott tétel a rendelésben
    bool containsItem(MenuItem* item) const;

    // Végösszeg kiszámítása
    int getTotal() const;

    // Formázott blokk kiírása (akár képernyőre, akár számlafájlba)
    void printReceipt(std::ostream& os) const;

    // Rendelés tételeinek elérése
    const List<OrderItem>& getItems() const;
};

#endif // ORDER_HPP