#include "menuitem.hpp"

#include "memtrace.h"

MenuItem::MenuItem(int id, const std::string& name, int price, bool available) : id(id), name(name), price(price), available(available) {}

int MenuItem::getId() const { return id; }
int MenuItem::getPrice() const { return price; }
std::string MenuItem::getName() const { return name; }