#ifndef DRINK_HPP
#define DRINK_HPP

/**
 * @file drink.hpp
 * @brief Ital menüpont deklarációja.
 */

#include "menuitem.hpp"

/**
 * @brief Italt reprezentáló menüpont.
 */
class Drink : public MenuItem
{
private:
    double volume;
    bool isAlcoholic;

public:
    /**
    * @brief Üres ital elem létrehozása.
     */
    Drink() = default;

    /**
    * @brief Ital létrehozása a megadott tulajdonságokkal.
    * @param id Egyedi azonosító.
    * @param name Tétel neve.
    * @param price Ar forintban.
    * @param available Elérhetőségi jelző.
    * @param volume Űrtartalom literben.
    * @param isAlcoholic Alkoholos-e.
     */
    Drink(int id, const std::string &name, int price, bool available, double volume, bool isAlcoholic);
    
    /**
    * @brief Visszaadja, hogy az ital alkoholos-e.
     */
    bool getIsAlcoholic() const;
    
    /**
    * @brief Beállítja az ital űrtartalmát.
     */
    void setVolume(double volume);

    /**
    * @brief Beállítja, hogy az ital alkoholos-e.
     */
    void setAlcoholic(bool isAlcoholic);
    
    /**
    * @brief Ellenőrzi az ital űrtartalmát.
     */
    static void validateVolume(double volume);
    
    /**
    * @brief Kiírja az italt emberi olvasásra alkalmas formában.
     */
    void print(std::ostream &os) const override;

    /**
    * @brief Elmenti az italt szöveges folyamra.
     */
    void save(std::ostream &os) const override;

    /**
    * @brief Lényegileg új példányt készít az italról.
     */
    MenuItem *clone() const override;

    /**
    * @brief Beolvassa az italt szöveges folyamról.
     */
    void read(std::istream &is) override;

    /**
    * @brief Fogadja a menüpont-visitor objektumot.
     */
    void accept(MenuItemVisitor& visitor) override;
};

#endif // DRINK_HPP