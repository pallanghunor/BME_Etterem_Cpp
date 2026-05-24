#ifndef FOOD_HPP
#define FOOD_HPP

/**
 * @file food.hpp
 * @brief Étel menüpont deklarációja.
 */

#include "menuitem.hpp"
#include "util.hpp"

/**
 * @brief Ételt reprezentáló menüpont.
 */
class Food : public MenuItem {
private:
    std::string allergens;

public:
    /**
    * @brief Üres étel elem létrehozása.
     */
    Food() = default;

    /**
    * @brief Étel létrehozása a megadott tulajdonságokkal.
    * @param id Egyedi azonosító.
    * @param name Tétel neve.
    * @param price Ar forintban.
    * @param available Elérhetőségi jelző.
    * @param allergens Allergének listája.
     */
    Food(int id, const std::string& name, int price, bool available, const std::string& allergens);
    
    /**
    * @brief Beállítja az allergének szövegét.
     */
    void setAllergens(const std::string& allergens);

    /**
    * @brief Kiírja az ételt emberi olvasásra alkalmas formában.
     */
    void print(std::ostream& os) const override;

    /**
    * @brief Elmenti az ételt szöveges folyamra.
     */
    void save(std::ostream& os) const override;

    /**
    * @brief Lényegileg új példányt készít az ételről.
     */
    MenuItem* clone() const override;

    /**
    * @brief Beolvassa az ételt szöveges folyamról.
     */
    void read(std::istream& is) override;

    /**
    * @brief Fogadja a menüpont-visitor objektumot.
     */
    void accept(MenuItemVisitor& visitor) override;
};

#endif // FOOD_HPP