cmake_minimum_required(VERSION 3.14)
project(sudoku)

set(CMAKE_CXX_STANDARD 14)

include_directories(scripts)

add_executable(sudoku
        scripts/BestFirstSearch.h
        scripts/Searchable.h
        scripts/Searcher.h
        scripts/solve.cpp
        scripts/Sudoku.h
        scripts/SudokuSquare.h)
