#ifndef ORDERITEM_HPP
#define ORDERITEM_HPP

/**
 * @file orderitem.hpp
 * @brief Rendelési tétel deklarációja.
 */

#include "menuitem.hpp"

/**
 * @brief Egy rendelt menüpont a mennyiséggel.
 */
class OrderItem {
private:
    MenuItem* item;
    int quantity;

public:
    /**
    * @brief Rendelési tétel létrehozása.
    * @param item A hivatkozott menüpont mutatója.
    * @param quantity A rendelt mennyiség.
     */
    OrderItem(MenuItem* item, int quantity);

    /**
    * @brief Visszaadja a hivatkozott menüpontot.
     */
    MenuItem* getItem() const;

    /**
    * @brief Visszaadja a rendelt mennyiséget.
     */
    int getQuantity() const;

    /**
    * @brief Visszaadja a tétel részösszegét.
     */
    int getSubtotal() const;

    /**
    * @brief Beállítja a rendelt mennyiséget.
     */
    void setQuantity(int qty);

};

#endif // ORDERITEM_HPP