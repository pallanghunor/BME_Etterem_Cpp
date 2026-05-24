#ifndef MENUITEM_HPP
#define MENUITEM_HPP

/**
 * @file menuitem.hpp
 * @brief Alap menü tétel absztrakciók és visitor interfész.
 */

#include <string>
#include <iostream>

class Food;
class Drink;

/**
 * @brief Visitor interfész a menü tételekhez.
 */
class MenuItemVisitor {
public:
    /**
    * @brief Egy étel meglátogatása.
     */
    virtual void visitFood(Food* food) = 0;

    /**
    * @brief Egy ital meglátogatása.
     */
    virtual void visitDrink(Drink* drink) = 0;

    /**
    * @brief Virtuális destruktor a polimorf törléshez.
     */
    virtual ~MenuItemVisitor() = default;
};

/**
 * @brief Minden menüpont közös osztálya.
 */
class MenuItem
{
protected:
    int id;
    std::string name;
    int price;
    bool available;

public:
    /**
    * @brief Alapértelmezett menü tétel létrehozása.
     */
    MenuItem() : id(0), price(0), available(false) {}

    /**
    * @brief Menü tétel létrehozása a megadott tulajdonságokkal.
     */
    MenuItem(int id, const std::string &name, int price, bool available);

    /**
    * @brief Virtuális destruktor.
     */
    virtual ~MenuItem() {};

    /**
    * @brief Visszaadja az azonosítót.
     */
    int getId() const;

    /**
    * @brief Visszaadja az árat.
     */
    int getPrice() const;

    /**
    * @brief Visszaadja a nevet.
     */
    std::string getName() const;
    
    /**
    * @brief Beállítja a nevet.
     */
    void setName(const std::string &name);

    /**
    * @brief Beállítja az árat.
     */
    void setPrice(int price);

    /**
    * @brief Beállítja, hogy elérhető-e.
     */
    void setAvailable(bool available);
    
    /**
    * @brief Visszaadja, hogy elérhető-e.
     */
    bool isAvailable() const;

    /**
    * @brief Ellenőrzi a menü tétel nevét.
     */
    static void validateName(const std::string& name);

    /**
    * @brief Ellenőrzi a menü tétel árát.
     */
    static void validatePrice(int price);

    /**
    * @brief Kiírja a menü tételt emberi olvasásra alkalmas formában.
     */
    virtual void print(std::ostream &os) const = 0;

    /**
    * @brief Elmenti a menü tételt szöveges folyamra.
     */
    virtual void save(std::ostream &os) const = 0;

    /**
    * @brief Lényegileg új példányt készít a menü tételből.
     */
    virtual MenuItem *clone() const = 0;

    /**
    * @brief Beolvassa a menü tételt szöveges folyamról.
     */
    virtual void read(std::istream &is) = 0;

    /**
    * @brief Fogadja a menü tétel-visitor objektumot.
     */
    virtual void accept(MenuItemVisitor& visitor) = 0;
};

#endif // MENUITEM_HPP