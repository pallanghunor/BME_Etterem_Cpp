CXX = g++
PROG = etterem_nhf
TESTPROG = etterem_tesztek

SRC = src/main.cpp src/restaurant.cpp src/table.cpp src/order.cpp src/orderitem.cpp src/menuitem.cpp src/food.cpp src/drink.cpp src/ui.cpp src/memtrace.cpp
TESTSRC = src/tests.cpp src/restaurant.cpp src/table.cpp src/order.cpp src/orderitem.cpp src/menuitem.cpp src/food.cpp src/drink.cpp src/ui.cpp src/memtrace.cpp

OBJ = $(SRC:.cpp=.o)
TESTOBJ = $(TESTSRC:.cpp=.o)

HEAD = include/drink.hpp include/food.hpp include/gtest_lite.h include/list.hpp include/memtrace.h include/menuitem.hpp include/order.hpp include/orderitem.hpp include/restaurant.hpp include/table.hpp include/ui.hpp include/util.hpp

JPORTA_PACK = $(SRC) $(TESTSRC) $(HEAD) README.md CMakeLists.txt

CXXFLAGS = -std=c++11 -Wall -Werror -g -DCPORTA -DMEMTRACE
CPPFLAGS = -Iinclude

.PHONY: all test clean tar zip

all: $(PROG) $(TESTPROG)

$(PROG): $(OBJ)
	$(CXX) -o $(PROG) $(OBJ)

$(TESTPROG): $(TESTOBJ)
	$(CXX) -o $(TESTPROG) $(TESTOBJ)

%.o: %.cpp $(HEAD)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

test: $(TESTPROG)
	./$(TESTPROG)

clean:
	rm -f $(PROG) $(TESTPROG) $(OBJ) $(TESTOBJ)

tar:
	tar -czf $(PROG).tgz $(JPORTA_PACK)

zip:
	zip kesz.zip $(JPORTA_PACK)