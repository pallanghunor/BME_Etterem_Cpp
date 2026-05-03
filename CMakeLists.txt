cmake_minimum_required(VERSION 3.10)

project(BME_Etterem_NHF CXX)

# C++11 szabvány kikényszerítése az iterátorokhoz és tartományalapú for-ciklusokhoz
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Wall -Wextra -pedantic -g)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(etterem_nhf ${SOURCES})