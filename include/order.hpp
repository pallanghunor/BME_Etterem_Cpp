#ifndef ORDER_HPP
#define ORDER_HPP

/**
 * @file order.hpp
 * @brief Rendelés konténer deklarációja.
 */

#include "list.hpp"
#include "orderitem.hpp"
#include <iostream>

/**
 * @brief Egy asztal rendelt tételeit tároló konténer.
 */
class Order {
private:
    List<OrderItem> items;

public:
    /**
    * @brief Üres rendelés létrehozása.
     */
    Order() {};

    /**
    * @brief Tétel hozzáadása a rendeléshez.
     */
    void addItem(MenuItem* item, int qty);

    /**
    * @brief Tétel eltávolítása a rendelésből.
     */
    void removeItem(MenuItem* item);

    /**
    * @brief Egy tétel mennyiségének módosítása.
     */
    void updateItemQuantity(MenuItem* item, int newQty);

    /**
    * @brief Visszaadja, hogy az adott tétel szerepel-e már a rendelésben.
     */
    bool containsItem(MenuItem* item) const;

    /**
    * @brief Visszaadja a rendelés végösszegét.
     */
    int getTotal() const;

    /**
    * @brief Formázott blokkot nyomtat a nyugtáról.
     */
    void printReceipt(std::ostream& os) const;

    /**
    * @brief Visszaadja a tárolt rendelési tételeket.
     */
    const List<OrderItem>& getItems() const;
};

#endif // ORDER_HPP