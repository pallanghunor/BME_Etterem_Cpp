#include "restaurant.hpp"
#include "food.hpp"
#include "drink.hpp"
#include <iostream>
#include <stdexcept>
#include <fstream>

Restaurant::Restaurant() {
    // Alapértelmezett konstruktor
}

Restaurant::~Restaurant() {
    // Felszabadítjuk a dinamikusan foglalt MenuItem-eket
    for (auto it = menu.begin(); it != menu.end(); ++it) {
        delete (*it); 
    }
}

void Restaurant::addMenuItem(MenuItem* item) {
    if (item != nullptr) {
        menu.push_back(item);
    }
}

// Biztonságos törlés (Dangling pointer védelem)
void Restaurant::deleteMenuItem(int id) {
    // 1. Megkeressük a törlendő elemet az étlapon
    MenuItem* itemToDelete = nullptr;
    auto menuIt = menu.begin();
    
    for (; menuIt != menu.end(); ++menuIt) {
        if ((*menuIt)->getId() == id) {
            itemToDelete = *menuIt;
            break;
        }
    }
    
    if (itemToDelete == nullptr) {
        throw std::invalid_argument("Nincs ilyen ID-ju tetel az etlapon!");
    }

    // 2. Biztonsági ellenőrzés: Használja-e valamelyik asztal ezt az elemet?
    for (auto tableIt = tables.begin(); tableIt != tables.end(); ++tableIt) {
        if ((*tableIt).hasActiveOrder()) {
            Order* order = (*tableIt).getOrder();
            if (order->containsItem(itemToDelete)) {
                // Ha igen, kivételt dobunk, nem engedjük a törlést!
                throw std::logic_error("A tetel nem torolheto, mert egy aktiv rendelesben szerepel!");
            }
        }
    }

    menu.erase(menuIt);   // Eltávolítjuk a mutatót a listából
    delete itemToDelete;  // Felszabadítjuk magát a memóriát
}

MenuItem* Restaurant::getMenuItemById(int id) const {
    for (auto it = menu.begin(); it != menu.end(); ++it) {
        if ((*it)->getId() == id) {
            return *it;
        }
    }
    return nullptr;
}

void Restaurant::addTable(int id, int seats, const std::string& desc, int x, int y) {
    for (auto it = tables.begin(); it != tables.end(); ++it) {
        if ((*it).getId() == id) {
            throw std::invalid_argument("Ez az asztal ID mar letezik!");
        }
        if ((*it).getX() == x && (*it).getY() == y) {
            throw std::invalid_argument("Erre a koordinatara mar van asztal rogzitve!");
        }
    }
    
    tables.push_back(Table(id, seats, desc, x, y));
}

Table* Restaurant::getTableById(int id) {
    for (auto it = tables.begin(); it != tables.end(); ++it) {
        if ((*it).getId() == id) {
            return &(*it);
        }
    }
    return nullptr;
}

void Restaurant::showOccupancyMap() const {
    std::cout << "[Foglaltsagi terkep helye]\n";
}

void Restaurant::loadData() {
    std::cout << "Adatok betoltese folyamatban ...\n";
}

void Restaurant::saveData() const {
    std::cout << "Adatok mentese folyamatban ...\n";
}