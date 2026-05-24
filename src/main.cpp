#include "restaurant.hpp"
#include "ui.hpp"

#include "memtrace.h"

int main() {
    Restaurant bmeEtterem;
    
    bmeEtterem.loadData();

    UI ui(bmeEtterem);
    
    ui.run();

    return 0;
}