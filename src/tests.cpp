#include <iostream>
#include "list.hpp"
#include "food.hpp"
#include "drink.hpp"
#include "order.hpp"
#include "restaurant.hpp"

#include "gtest_lite.h"

int main() {
    // 1. Üres lista és PushBack teszt
    TEST(ListTest, InitAndPushBack) {
        List<int> l;
        EXPECT_EQ(0, l.getSize());
        
        l.push_back(10);
        l.push_back(20);
        
        EXPECT_EQ(2, l.getSize());
        EXPECT_EQ(10, l[0]);
        EXPECT_EQ(20, l[1]);
    } ENDM

    // 2. Indexelési hiba (Kivételkezelés tesztelése)
    TEST(ListTest, OutOfRangeThrow) {
        List<int> l;
        l.push_back(5);
        
        // Várjuk, hogy std::out_of_range kivételt dobjon, ha rossz indexet kérünk
        EXPECT_THROW(l[1], std::out_of_range&);
        EXPECT_THROW(l[100], std::out_of_range&);
    } ENDM

    // 3. Mély másolat (Deep copy) tesztelése
    TEST(ListTest, DeepCopyAndAssignment) {
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
    } ENDM

    // 4. Törlés (Erase) és Ürítés (Clear) tesztelése
    TEST(ListTest, EraseAndClear) {
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
    } ENDM

    // 5. Rendelés logikája és Végösszeg számítás
    TEST(OrderTest, AddAndCalculateTotal) {
        Order o;
        // Lokális objektumok a memóriaszivárgás elkerülésére a tesztben
        Food f1(1, "Pizza", 2500, true, "Gluten");
        Food f2(2, "Sutemeny", 1000, true, "Tojas");

        o.addItem(&f1, 2); // 2 * 2500 = 5000
        o.addItem(&f2, 1); // 1 * 1000 = 1000

        EXPECT_EQ(6000, o.getTotal());
        EXPECT_EQ(2, o.getItems().getSize());
    } ENDM

    // 6. Darabszám frissítése és automata törlés (0 darabnál)
    TEST(OrderTest, UpdateQuantityAndRemove) {
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
    } ENDM

    // 7. Asztalok felvétele és Ütközésvizsgálat (Kivételek tesztelése)
    TEST(RestaurantTest, TableCollisionHandling) {
        Restaurant r;
        
        // Szabályos felvétel
        r.addTable(1, 4, "Ablak melletti", 5, 5);
        EXPECT_TRUE(r.isTableAt(5, 5));
        EXPECT_TRUE(r.isTableIdTaken(1));

        // Hiba 1: Foglalt ID
        EXPECT_THROW(r.addTable(1, 2, "Masik asztal", 1, 1), std::invalid_argument&);

        // Hiba 2: Foglalt X,Y koordináta
        EXPECT_THROW(r.addTable(2, 2, "Egymas hegyen-hatan", 5, 5), std::invalid_argument&);
    } ENDM

    // 8. Étel és Ital (Polimorf) tulajdonságainak tesztelése
    TEST(MenuItemTest, FoodAndDrinkProperties) {
        Food f(1, "Margherita", 2500, true, "Gluten, Tej");
        EXPECT_EQ(1, f.getId());
        EXPECT_EQ(std::string("Margherita"), f.getName());
        EXPECT_EQ(2500, f.getPrice());
        EXPECT_EQ(true, f.isAvailable());

        Drink d(2, "Csapolt Sor", 800, true, 0.5, true);
        EXPECT_DOUBLE_EQ(0.5, d.getVolume());
        EXPECT_EQ(true, d.getIsAlcoholic());
    } ENDM

    // 9. Asztal (Table) állapotkezelése
    TEST(TableTest, OrderManagement) {
        Table t(1, 4, "Ablak melletti", 0, 0);
        EXPECT_FALSE(t.isOccupied());
        EXPECT_FALSE(t.hasActiveOrder());

        // Rendelés nyitása
        t.openOrder();
        EXPECT_TRUE(t.isOccupied());
        EXPECT_TRUE(t.hasActiveOrder());
        EXPECT_TRUE(t.getOrder() != nullptr);
    } ENDM

    // 10. Étterem (Restaurant) étlap kezelése
    TEST(RestaurantTest, MenuManagement) {
        Restaurant r;
        Food* f = new Food(1, "Pizza", 2000, true, "");
        r.addMenuItem(f);
        
        // Benne van-e az étlapon?
        EXPECT_EQ(f, r.getMenuItemById(1));
        
        // Ha nincs ilyen ID, nullptr-t kell kapnunk
        EXPECT_TRUE(r.getMenuItemById(99) == nullptr);
    } ENDM

    // 11. Étterem (Restaurant) - Nem létező tétel törlése
    TEST(RestaurantTest, DeleteNonExistentItem) {
        Restaurant r;
        EXPECT_THROW(r.deleteMenuItem(99), std::invalid_argument&);
    } ENDM

    // 12. Tétel törlésének megakadályozása aktív rendelésnél
    TEST(RestaurantTest, DeleteItemInActiveOrder) {
        Restaurant r;
        
        // 1. Felveszünk egy ételt és egy asztalt
        Food* f = new Food(1, "Teszt Pizza", 2000, true, "");
        r.addMenuItem(f);
        r.addTable(1, 4, "Teszt Asztal", 0, 0);

        // 2. Az asztalhoz rendelést nyitunk, és beletesszük az ételt
        Table* t = r.getTableById(1);
        t->openOrder();
        t->addItemToOrder(f, 2);

        // 3. Megpróbáljuk törölni az étlapból a tételt. 
        // A programnak std::logic_error kivétellel kell reagálnia, mert a tétel egy aktív rendelésben szerepel.
        EXPECT_THROW(r.deleteMenuItem(1), std::logic_error&);
    } ENDM

    return 0;
}