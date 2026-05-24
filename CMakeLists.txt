cmake_minimum_required(VERSION 3.10)

project(BME_Etterem_NHF CXX)

# C++11 szabvány kikényszerítése az iterátorokhoz és tartományalapú for-ciklusokhoz
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Wall -Wextra -pedantic -g -DMEMTRACE)

include_directories(include)

add_executable(etterem_nhf 
    src/main.cpp
    src/restaurant.cpp
    src/table.cpp
    src/order.cpp
    src/orderitem.cpp
    src/menuitem.cpp
    src/food.cpp
    src/drink.cpp
    src/ui.cpp 
    src/memtrace.cpp
)

# Tesztelő program
add_executable(etterem_tesztek
    src/tests.cpp
    src/restaurant.cpp
    src/table.cpp
    src/order.cpp
    src/orderitem.cpp
    src/menuitem.cpp
    src/food.cpp
    src/drink.cpp
    src/ui.cpp 
    src/memtrace.cpp
)

target_include_directories(etterem_tesztek PRIVATE include)