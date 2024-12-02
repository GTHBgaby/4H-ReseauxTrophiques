cmake_minimum_required(VERSION 3.26)
project(4E_ReseauxTrophiques C)

set(CMAKE_C_STANDARD 11)

add_executable(4E_ReseauxTrophiques main.c Graph.h Graph.c evolution.c evolution.h population.h population.c A_star.c
        generationDOT.c generationDOT.h)

