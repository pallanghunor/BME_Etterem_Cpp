/**
 * @file main.cpp
 * @brief Az alkalmazás belépési pontja.
 */

#include <iostream>

#include "restaurant.hpp"
#include "ui.hpp"
#include "list.hpp"
#include "food.hpp"
#include "drink.hpp"
#include "order.hpp"

#include "memtrace.h"


#ifdef CPORTA

#include "gtest_lite.h"
int main()
{
    // 1. Üres lista és PushBack teszt
    TEST(ListTest, InitAndPushBack)
    {
        List<int> l;
        EXPECT_EQ(0, l.getSize());

        l.push_back(10);
        l.push_back(20);

        EXPECT_EQ(2, l.getSize());
        EXPECT_EQ(10, l[0]);
        EXPECT_EQ(20, l[1]);
    }
    ENDM

    // 2. Indexelési hiba (Kivételkezelés tesztelése)
    TEST(ListTest, OutOfRangeThrow)
    {
        List<int> l;
        l.push_back(5);

        // Várjuk, hogy std::out_of_range kivételt dobjon, ha rossz indexet kérünk
        EXPECT_THROW(l[1], std::out_of_range &);
        EXPECT_THROW(l[100], std::out_of_range &);
    }
    ENDM

    // 3. Mély másolat (Deep copy) tesztelése
    TEST(ListTest, DeepCopyAndAssignment)
    {
        List<int> l1;
        l1.push_back(1);
        l1.push_back(2);

        List<int> l2(l1); // Másoló konstruktor
        EXPECT_EQ(2, l2.getSize());
        EXPECT_EQ(1, l2[0]);

        List<int> l3;
        l3 = l1; // Értékadó operátor
        EXPECT_EQ(2, l3.getSize());

        // Mély másolat bizonyítása
        l1.push_back(3);
        EXPECT_EQ(3, l1.getSize());
        EXPECT_EQ(2, l2.getSize());
        EXPECT_EQ(2, l3.getSize());
    }
    ENDM

    // 4. Törlés (Erase) és Ürítés (Clear) tesztelése
    TEST(ListTest, EraseAndClear)
    {
        List<int> l;
        l.push_back(10);
        l.push_back(20);
        l.push_back(30);

        // Első elem törlése
        l.erase(l.begin());
        EXPECT_EQ(2, l.getSize());
        EXPECT_EQ(20, l[0]);

        // Lista teljes ürítése
        l.clear();
        EXPECT_EQ(0, l.getSize());

        // Üres listán is meghívható az erase hiba nélkül
        EXPECT_NO_THROW(l.erase(l.begin()));
    }
    ENDM

    // 5. Rendelés logikája és Végösszeg számítás
    TEST(OrderTest, AddAndCalculateTotal)
    {
        Order o;
        // Lokális objektumok a memóriaszivárgás elkerülésére a tesztben
        Food f1(1, "Pizza", 2500, true, "Gluten");
        Food f2(2, "Sutemeny", 1000, true, "Tojas");

        o.addItem(&f1, 2); // 2 * 2500 = 5000
        o.addItem(&f2, 1); // 1 * 1000 = 1000

        EXPECT_EQ(6000, o.getTotal());
        EXPECT_EQ(2, o.getItems().getSize());
    }
    ENDM

    // 6. Darabszám frissítése és automata törlés (0 darabnál)
    TEST(OrderTest, UpdateQuantityAndRemove)
    {
        Order o;
        Food f(1, "Hamburger", 3000, true, "");

        o.addItem(&f, 1);
        EXPECT_EQ(3000, o.getTotal());

        // Növelés
        o.updateItemQuantity(&f, 3);
        EXPECT_EQ(9000, o.getTotal());

        // Nullára állítás -> törlődnie kell a listából!
        o.updateItemQuantity(&f, 0);
        EXPECT_EQ(0, o.getTotal());
        EXPECT_EQ(0, o.getItems().getSize());
    }
    ENDM

    // 7. Asztalok felvétele és Ütközésvizsgálat (Kivételek tesztelése)
    TEST(RestaurantTest, TableCollisionHandling)
    {
        Restaurant r;

        // Szabályos felvétel
        r.addTable(1, 4, "Ablak melletti", 5, 5);
        EXPECT_TRUE(r.isTableAt(5, 5));
        EXPECT_TRUE(r.isTableIdTaken(1));

        // Hiba 1: Foglalt ID
        EXPECT_THROW(r.addTable(1, 2, "Masik asztal", 1, 1), std::invalid_argument &);

        // Hiba 2: Foglalt X,Y koordináta
        EXPECT_THROW(r.addTable(2, 2, "Egymas hegyen-hatan", 5, 5), std::invalid_argument &);
    }
    ENDM

    // 8. Étel és Ital (Polimorf) tulajdonságainak tesztelése
    TEST(MenuItemTest, FoodAndDrinkProperties)
    {
        Food f(1, "Margherita", 2500, true, "Gluten, Tej");
        EXPECT_EQ(1, f.getId());
        EXPECT_EQ(std::string("Margherita"), f.getName());
        EXPECT_EQ(2500, f.getPrice());
        EXPECT_EQ(true, f.isAvailable());

        Drink d(2, "Csapolt Sor", 800, true, 0.5, true);
        EXPECT_DOUBLE_EQ(0.5, d.getVolume());
        EXPECT_EQ(true, d.getIsAlcoholic());
    }
    ENDM

    // 9. Asztal (Table) állapotkezelése
    TEST(TableTest, OrderManagement)
    {
        Table t(1, 4, "Ablak melletti", 0, 0);
        EXPECT_FALSE(t.isOccupied());
        EXPECT_FALSE(t.hasActiveOrder());

        // Rendelés nyitása
        t.openOrder();
        EXPECT_TRUE(t.isOccupied());
        EXPECT_TRUE(t.hasActiveOrder());
        EXPECT_TRUE(t.getOrder() != nullptr);
    }
    ENDM

    // 10. Étterem (Restaurant) étlap kezelése
    TEST(RestaurantTest, MenuManagement)
    {
        Restaurant r;
        Food *f = new Food(1, "Pizza", 2000, true, "");
        r.addMenuItem(f);

        // Benne van-e az étlapon?
        EXPECT_EQ(static_cast<MenuItem *>(f), r.getMenuItemById(1));

        // Ha nincs ilyen ID, nullptr-t kell kapnunk
        EXPECT_TRUE(r.getMenuItemById(99) == nullptr);
    }
    ENDM

    // 11. Étterem (Restaurant) - Nem létező tétel törlése
    TEST(RestaurantTest, DeleteNonExistentItem)
    {
        Restaurant r;
        EXPECT_THROW(r.deleteMenuItem(99), std::invalid_argument &);
    }
    ENDM

    // 12. Tétel törlésének megakadályozása aktív rendelésnél
    TEST(RestaurantTest, DeleteItemInActiveOrder)
    {
        Restaurant r;

        // 1. Felveszünk egy ételt és egy asztalt
        Food *f = new Food(1, "Teszt Pizza", 2000, true, "");
        r.addMenuItem(f);
        r.addTable(1, 4, "Teszt Asztal", 0, 0);

        // 2. Az asztalhoz rendelést nyitunk, és beletesszük az ételt
        Table *t = r.getTableById(1);
        t->openOrder();
        t->addItemToOrder(f, 2);

        // 3. Megpróbáljuk törölni az étlapból a tételt.
        // A programnak std::logic_error kivétellel kell reagálnia, mert a tétel egy aktív rendelésben szerepel.
        EXPECT_THROW(r.deleteMenuItem(1), std::logic_error &);
    }
    ENDM

    // 13. Food osztály - Allergenek tesztelése
    TEST(FoodTest, AllergenHandling)
    {
        Food f(1, "Szalonna", 1500, true, "Serteshus");
        EXPECT_EQ(std::string("Szalonna"), f.getName());
        EXPECT_EQ(1500, f.getPrice());

        // Allergenek modositasa
        f.setAllergens("Tejallergia, Gluten");
        EXPECT_EQ(1500, f.getPrice());

        // Ures allergen lista
        Food f2(2, "Hasábburgonya", 1000, true, "");
        EXPECT_EQ(std::string("Hasábburgonya"), f2.getName());
    }
    ENDM

    // 14. Food osztály - Sokmindenes tesztelése
    TEST(FoodTest, FoodProperties)
    {
        Food f(5, "Csirkecomb", 2200, false, "Csirke, Tejallergia");
        EXPECT_EQ(5, f.getId());
        EXPECT_EQ(std::string("Csirkecomb"), f.getName());
        EXPECT_EQ(2200, f.getPrice());
        EXPECT_FALSE(f.isAvailable());
    }
    ENDM

    // 15. Drink osztály - Űrtartalom validálása
    TEST(DrinkTest, VolumeValidation)
    {
        // Normál ital
        Drink d1(1, "Sok", 300, true, 0.2, false);
        EXPECT_DOUBLE_EQ(0.2, d1.getVolume());
        EXPECT_FALSE(d1.getIsAlcoholic());

        // Alkoholos ital
        Drink d2(2, "Sor", 500, true, 0.5, true);
        EXPECT_DOUBLE_EQ(0.5, d2.getVolume());
        EXPECT_TRUE(d2.getIsAlcoholic());

        // Szetvolume validacio - negativ
        Drink d3(3, "Hiba", 400, true, 0.3, false);
        EXPECT_THROW(d3.setVolume(-0.5), std::invalid_argument &);
        EXPECT_THROW(d3.setVolume(0), std::invalid_argument &);
    }
    ENDM

    // 16. Drink osztály - setAlcoholic tesztelése
    TEST(DrinkTest, AlcoholicFlag)
    {
        Drink d(1, "Ital", 400, true, 0.5, false);
        EXPECT_FALSE(d.getIsAlcoholic());

        d.setAlcoholic(true);
        EXPECT_TRUE(d.getIsAlcoholic());

        d.setAlcoholic(false);
        EXPECT_FALSE(d.getIsAlcoholic());
    }
    ENDM

    // 17. OrderItem osztály - Constructor és getters
    TEST(OrderItemTest, Construction)
    {
        Food f(1, "Pizza", 2500, true, "");
        OrderItem oi(&f, 3);

        EXPECT_EQ(3, oi.getQuantity());
        EXPECT_EQ(&f, oi.getItem());
        EXPECT_EQ(7500, oi.getSubtotal()); // 3 * 2500
    }
    ENDM

    // 18. OrderItem osztály - Null pointer ellenőrzése
    TEST(OrderItemTest, NullptrThrow)
    {
        // Null pointer rendeles tetel konstruktorban dobjon ki
        EXPECT_THROW(OrderItem(nullptr, 1), std::invalid_argument &);
    }
    ENDM

    // 19. OrderItem osztály - setQuantity
    TEST(OrderItemTest, UpdateQuantity)
    {
        Drink d(1, "Sor", 600, true, 0.5, true);
        OrderItem oi(&d, 2);

        EXPECT_EQ(1200, oi.getSubtotal()); // 2 * 600

        oi.setQuantity(5);
        EXPECT_EQ(5, oi.getQuantity());
        EXPECT_EQ(3000, oi.getSubtotal()); // 5 * 600

        oi.setQuantity(0);
        EXPECT_EQ(0, oi.getQuantity());
        EXPECT_EQ(0, oi.getSubtotal());
    }
    ENDM

    // 20. Table osztály - Másoló konstruktor
    TEST(TableTest, CopyConstructor)
    {
        Table t1(1, 4, "Ablak mellett", 2, 3);

        // Másoló konstruktor
        Table t2(t1);
        EXPECT_EQ(1, t2.getId());
        EXPECT_EQ(4, t2.getSeats());
        EXPECT_EQ(std::string("Ablak mellett"), t2.getDescription());
        EXPECT_EQ(2, t2.getX());
        EXPECT_EQ(3, t2.getY());
    }
    ENDM

    // 21. Table osztály - Értékadó operátor
    TEST(TableTest, AssignmentOperator)
    {
        Table t1(1, 4, "Ablak mellett", 0, 0);
        Table t2(2, 2, "Sarkos", 5, 5);

        t2 = t1;
        EXPECT_EQ(1, t2.getId());
        EXPECT_EQ(4, t2.getSeats());
        EXPECT_EQ(std::string("Ablak mellett"), t2.getDescription());
    }
    ENDM

    // 22. Table osztály - Validateseats
    TEST(TableTest, ValidateSeats)
    {
        // Minimális jó érték
        Table t1(1, 2, "Elég kicsi", 0, 0);
        EXPECT_EQ(2, t1.getSeats());

        // Nagyobb asztal
        Table t2(2, 8, "Nagy", 1, 1);
        EXPECT_EQ(8, t2.getSeats());

        // Túl kicsi - exception
        EXPECT_THROW(Table t3(3, 1, "Túl kicsi", 2, 2), std::invalid_argument &);
    }
    ENDM

    // 23. Table osztály - setSeats validacio
    TEST(TableTest, SetSeatsValidation)
    {
        Table t(1, 4, "Asztal", 0, 0);
        EXPECT_EQ(4, t.getSeats());

        t.setSeats(6);
        EXPECT_EQ(6, t.getSeats());

        EXPECT_THROW(t.setSeats(1), std::invalid_argument &);
    }
    ENDM

    // 24. Table osztály - setDescription
    TEST(TableTest, SetDescription)
    {
        Table t(1, 4, "Eredeti", 0, 0);
        EXPECT_EQ(std::string("Eredeti"), t.getDescription());

        t.setDescription("Uj leiras");
        EXPECT_EQ(std::string("Uj leiras"), t.getDescription());
    }
    ENDM

    // 25. Table osztály - Position szállítás
    TEST(TableTest, PositionHandling)
    {
        Table t(1, 4, "Asztal", 3, 5);
        EXPECT_EQ(3, t.getX());
        EXPECT_EQ(5, t.getY());

        t.setPosition(7, 2);
        EXPECT_EQ(7, t.getX());
        EXPECT_EQ(2, t.getY());
    }
    ENDM

    // 26. Order osztály - Azonos tétel többszöri hozzáadása
    TEST(OrderTest, AddSameItemMultipleTimes)
    {
        Order o;
        Food f(1, "Burger", 2000, true, "");

        o.addItem(&f, 2);
        EXPECT_EQ(4000, o.getTotal());

        // Ugyanaz az étel újra - a mennyiség nő
        o.addItem(&f, 3);
        EXPECT_EQ(10000, o.getTotal());       // 5 * 2000
        EXPECT_EQ(1, o.getItems().getSize()); // De csak 1 tétel van listán!
    }
    ENDM

    // 27. Order osztály - containsItem
    TEST(OrderTest, ContainsItem)
    {
        Order o;
        Food f1(1, "Pizza", 2500, true, "");
        Food f2(2, "Salata", 1500, true, "");

        o.addItem(&f1, 1);
        EXPECT_TRUE(o.containsItem(&f1));
        EXPECT_FALSE(o.containsItem(&f2));

        o.addItem(&f2, 2);
        EXPECT_TRUE(o.containsItem(&f1));
        EXPECT_TRUE(o.containsItem(&f2));
    }
    ENDM

    // 28. Order osztály - removeItem
    TEST(OrderTest, RemoveItem)
    {
        Order o;
        Food f1(1, "Pizza", 2500, true, "");
        Food f2(2, "Italok", 800, true, "");

        o.addItem(&f1, 2);
        o.addItem(&f2, 3);

        EXPECT_EQ(2, o.getItems().getSize());
        EXPECT_EQ(7400, o.getTotal()); // 2*2500 + 3*800

        o.removeItem(&f1);
        EXPECT_EQ(1, o.getItems().getSize());
        EXPECT_EQ(2400, o.getTotal()); // 3*800
    }
    ENDM

    // 29. Order osztály - addItem nullpointerrel
    TEST(OrderTest, AddNullItem)
    {
        Order o;
        o.addItem(nullptr, 5); // Szabad hagyni, nem szabad dobni
        EXPECT_EQ(0, o.getItems().getSize());
        EXPECT_EQ(0, o.getTotal());
    }
    ENDM

    // 30. Order osztály - addItem negatív mennyiséggel
    TEST(OrderTest, AddNegativeQuantity)
    {
        Order o;
        Food f(1, "Pizza", 2500, true, "");
        o.addItem(&f, -5); // Szabad hagyni, nem szabad dobni
        EXPECT_EQ(0, o.getItems().getSize());
        EXPECT_EQ(0, o.getTotal());
    }
    ENDM

    // 31. List osztály - Praework iteratornal
    TEST(ListTest, IteratorOperations)
    {
        List<int> l;
        l.push_back(10);
        l.push_back(20);
        l.push_back(30);

        int sum = 0;
        for (auto it = l.begin(); it != l.end(); ++it)
        {
            sum += *it;
        }

        EXPECT_EQ(60, sum);
    }
    ENDM

    // 32. List osztály - Üres lista iterátora
    TEST(ListTest, EmptyListIterator)
    {
        List<std::string> l;

        int count = 0;
        for (auto it = l.begin(); it != l.end(); ++it)
        {
            count++;
        }

        EXPECT_EQ(0, count);
    }
    ENDM

    // 33. List osztály - Egyetlen elem iterálása
    TEST(ListTest, SingleElementIterator)
    {
        List<int> l;
        l.push_back(42);

        auto it = l.begin();
        EXPECT_EQ(42, *it);
        ++it;
        EXPECT_TRUE(it == l.end());
    }
    ENDM

    // 34. Restaurant osztály - getMaxX és getMaxY
    TEST(RestaurantTest, MapDimensions)
    {
        Restaurant r;
        EXPECT_EQ(15, r.getMaxX());
        EXPECT_EQ(10, r.getMaxY());
    }
    ENDM

    // 35. Restaurant osztály - Több asztal kezelése
    TEST(RestaurantTest, MultipleTablesHandling)
    {
        Restaurant r;

        r.addTable(1, 2, "Kicsi", 0, 0);
        r.addTable(2, 4, "Kozepes", 5, 5);
        r.addTable(3, 8, "Nagy", 10, 10);

        EXPECT_TRUE(r.isTableIdTaken(1));
        EXPECT_TRUE(r.isTableIdTaken(2));
        EXPECT_TRUE(r.isTableIdTaken(3));
        EXPECT_FALSE(r.isTableIdTaken(4));

        EXPECT_TRUE(r.isTableAt(0, 0));
        EXPECT_TRUE(r.isTableAt(5, 5));
        EXPECT_TRUE(r.isTableAt(10, 10));
    }
    ENDM

    // 36. Restaurant osztály - Több menüpont kezelése
    TEST(RestaurantTest, MultipleMenuItemsHandling)
    {
        Restaurant r;

        Food *f1 = new Food(1, "Pizza", 2500, true, "Gluten");
        Food *f2 = new Food(2, "Salata", 1500, true, "");
        Drink *d1 = new Drink(3, "Sor", 800, true, 0.5, true);

        r.addMenuItem(f1);
        r.addMenuItem(f2);
        r.addMenuItem(d1);

        EXPECT_EQ(static_cast<MenuItem *>(f1), r.getMenuItemById(1));
        EXPECT_EQ(static_cast<MenuItem *>(f2), r.getMenuItemById(2));
        EXPECT_EQ(static_cast<MenuItem *>(d1), r.getMenuItemById(3));
        EXPECT_TRUE(r.getMenuItemById(4) == nullptr);
    }
    ENDM

    // 37. Table osztály - Aktív rendelessel - setOccupied hiba
    TEST(TableTest, OccupiedWithActiveOrder)
    {
        Table t(1, 4, "Asztal", 0, 0);
        EXPECT_FALSE(t.isOccupied());

        t.openOrder();
        EXPECT_TRUE(t.isOccupied());

        // Aktív rendeléssel nem lehet foglaltsagot módosítani
        EXPECT_THROW(t.setOccupied(false), std::logic_error &);
    }
    ENDM

    // 38. Table osztály - closeTable
    TEST(TableTest, CloseTableOperations)
    {
        Table t(1, 4, "Asztal", 0, 0);
        Food f(1, "Pizza", 2000, true, "");

        t.openOrder();
        t.addItemToOrder(&f, 2);
        EXPECT_TRUE(t.hasActiveOrder());

        // Kiírás nélkül szovetkezik
        std::stringstream ss;
        t.closeTable(ss);
        EXPECT_FALSE(t.hasActiveOrder());
        EXPECT_FALSE(t.isOccupied());
    }
    ENDM

    // 39. Order osztály - Total szamítás több tétel
    TEST(OrderTest, ComplexTotal)
    {
        Order o;
        Food f(1, "Pizza", 2500, true, "");
        Drink d(2, "Sor", 600, true, 0.5, true);

        o.addItem(&f, 2); // 5000
        o.addItem(&d, 4); // 2400

        EXPECT_EQ(7400, o.getTotal());
    }
    ENDM

    // 40. List osztály - Több típusú elemek tesztelése
    TEST(ListTest, StringList)
    {
        List<std::string> l;
        l.push_back("Elso");
        l.push_back("Masodik");
        l.push_back("Harmadik");

        EXPECT_EQ(3, l.getSize());
        EXPECT_EQ(std::string("Elso"), l[0]);
        EXPECT_EQ(std::string("Masodik"), l[1]);
        EXPECT_EQ(std::string("Harmadik"), l[2]);
    }
    ENDM

        return 0;
}

#else

int main()
{
    Restaurant bmeEtterem;

    bmeEtterem.loadData();

    UI ui(bmeEtterem);

    ui.run();

    return 0;
}

#endif