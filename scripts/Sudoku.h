#ifndef SUDOKU_H
#define SUDOKU_H

#include "Searchable.h"
#include "SudokuSquare.h"
#include <iostream>
#include <utility>
#include <cmath>
#include <vector>

using std::pair;
using std::sqrt;
using std::pow;
using std::cout;
using std::endl;
using std::vector;


// TODO: Your Sudoku class goes here:

class Sudoku : public Searchable {
  private:
    vector<vector<SudokuSquareSet>> board;
    /* int heur; */
    int completed;
    int sqr;

  public:
    Sudoku(const int size): /*heur(pow(size, 3)),*/ completed(0), sqr(static_cast<int>(sqrt(size))) {
      SudokuSquareSet s = SudokuSquareSet();
      for (int n = 1; n <= size; ++n) {
        s.insert(n);
      }
      board = vector<vector<SudokuSquareSet>>();
      for (int i = 0; i < size; ++i) {
        vector<SudokuSquareSet> col = vector<SudokuSquareSet>();
        for (int j = 0; j < size; ++j) {
          col.push_back(s);
        }
        board.push_back(col);
      }
    }

    virtual bool isSolution() const {
      if (completed < 81) {
        return false;
      }
      for (int c = 0; c < board.size(); ++c) {
        for (int r = 0; r < board.size(); ++r) {
          if (getSquare(r,c) == -1) {
            return false;
          }
        }
      }
      return true;
    }

    virtual void write(ostream & o) const {
      for (int i = 0; i < board.size(); ++i) {
        if (i % sqr == 0) {
          o << endl;
        }
        for (int j = 0; j < board[i].size(); ++j) {
          if (j % sqr == 0) {
            o << "  |";
          }
          if (board[j][i].size() != 1) {
            for (int v = 1; v <= board.size(); ++v) {
              if (board[j][i].find(v) != board[j][i].end()) {
                o << v;
              } else {
                o << "-";
              }
            }
            o << " | ";
          } else {
            for (int v = 1; v <= board.size(); ++v) {
              if (board[j][i].find(v) != board[j][i].end()) {
                o << v;
              } else {
                o << ".";
              }
            }
            o << " | ";
          }
        }
        o << endl;
      }
    }

    virtual vector<unique_ptr<Searchable>> successors() const {
      vector<unique_ptr<Searchable>> suc;
      /* pair<int,int> cell(-1, -1); */
      int col = -1;
      int row = -1;
      bool searching = true;
      int cellSize = (int) board.size() / 2;
      int c = 0;
      while (searching && c < board.size()) {
        int r = 0;
        while (searching && r < board[c].size()) {
          const SudokuSquareSet & set = board[c][r];
          if (set.size() > 1) {
              if (set.size() == 2) {
                return getSucs(c, r);
              } else if (set.size() < cellSize) {
                col = c;
                row = r;
              }
          }
          ++r;
        }
        ++c;
      }
      if (col != -1 && row != -1) {
        return getSucs(col, row);
      }
      if (col == -1 || row == -1) {
        int c = 0;
        while (searching && c < board.size()) {
          int r = 0;
          while (searching && r < board[c].size()) {
            if (board[c][r].size() > 1) {
              return getSucs(c, r);
            }
            ++r;
          }
          ++c;
        }
      }
      return suc;
    }

    vector<unique_ptr<Searchable>> getSucs(const int & col, const int & row) const {
      vector<unique_ptr<Searchable>> suc;
      for (int v : board[col][row]) {
        Sudoku * sud = new Sudoku(*this);
        if(sud->setSquare(row, col, v)){
          suc.emplace_back(sud);
        } else {
          delete sud;
        }
      }
      if (suc.size() == 1 && !suc[0]->isSolution()) {
        return suc[0]->successors();
      }
      return suc;
    }

    bool clearCell(const int & col, const int & row, const int & value) {
      SudokuSquareSet & set = board[col][row];
      int size = set.size();
      set.erase(value);
      if (set.empty()) {
        return false;
      }
      if (set.size() == 1 && set.size() != size) {
        return setSquare(row, col, *(set.begin()));
      }
      /* heur -= 1; */
      return true;
    }


    bool updateSets(const int & col, const int & row, const int & value) {
      /* set<pair<int,int>> rowDups; */
      /* set<pair<int,int>> colDups; */
      vector<pair<int,int>> rowDups;
      vector<pair<int,int>> colDups;
      /* vector<pair<int,int>> boxDups; */
      // remove value from Column and row
      for (int i = 0; i < board.size(); ++i) {
        if (i != row) {
          if(!clearCell(col, i, value)) {
            return false;
          }
          if (board[col][i].size() == 2) {
            colDups.emplace_back(col, i);
            /* colDups.emplace(col, i); */
          }
        }
        if (i != col) {
          if (!clearCell(i, row, value)) {
            return false;
          }
          if (board[i][row].size() == 2) {
            rowDups.emplace_back(i, row);
            /* rowDups.emplace(i, row); */
          }
        }
      }
      // remove value from sub-grid
      int sqCol = col - (col % sqr);
      int sqRow = row - (row % sqr);
      for (int c = sqCol; c < sqCol + sqr; ++c) {
        if (c != col) {
          for (int r = sqRow; r < sqRow + sqr; ++r) {
            if (r != row) {
              if (!clearCell(c, r, value)) {
                return false;
              }
              /* if (board[c][r].size() == 2) { */
              /*   boxDups.emplace_back(c, r); */
              /* } */
            }
          }
        }
      }
      if (!clearRowDups(rowDups) || !clearColDups(colDups)) {
        return false;
      }
      /* if (!clearBoxDups(boxDups)) { */
      /*   return false; */
      /* } */
      return true;
    }

    bool setSquare(const int & row, const int & col, const int & value) {
      /* int size = board[col][row].size(); */
      SudokuSquareSet & set = board[col][row];
      set.clear();
      set.insert(value);

      /* cout << value << "  ->  " << col << " , " << row << endl; */
      if (!updateSets(col, row, value)) {
        return false;
      }
      /* heur -= (size - 1); */
      ++completed;
      return true;
    }

    bool clearRowDups(const vector<pair<int,int>> & dups) {
      if (dups.size() < 2) {
        return true;
      }
      for (auto item : dups) {
        for (auto item2 : dups) {
          if (item != item2) {
            SudokuSquareSet & set = board[item.first][item.second];
            if (set == board[item2.first][item2.second]) {
              for (int c = 0; c < board.size(); ++c) {
                if (c != item.first && c != item2.first) {
                  for (auto i : set) {
                    if(!clearCell(c, item.second, i)) {
                      return false;
                    }
                  }
                }
              }
            }
          }
        }
      }
      return true;
    }

    bool clearColDups(const vector<pair<int,int>> & dups) {
      if (dups.size() < 2) {
        return true;
      }
      for (auto item : dups) {
        for (auto item2 : dups) {
          if (item != item2) {
            SudokuSquareSet & set = board[item.first][item.second];
            if (set == board[item2.first][item2.second]) {
              for (int r = 0; r < board.size(); ++r) {
                if (r != item.second && r != item2.second) {
                  for (auto i : set) {
                    if(!clearCell(item.first, r, i)) {
                      return false;
                    }
                  }
                }
              }
            }
          }
        }
      }
      return true;
    }

    bool clearBoxDups(const vector<pair<int,int>> & dups) {
      if (dups.size() < 2) {
        return true;
      }
      for (auto item : dups) {
        for (auto item2 : dups) {
          if (item != item2) {
            if (board[item.first][item.second] == board[item2.first][item2.second]) {
              /* cout << "found " << item.first << item.second << endl; */
              /* write(cout); */
              int startCol = item.first - (item.first % sqr);
              int startRow = item.second - (item.second % sqr);
              /* cout << "box start" << startCol << startRow << endl; */
              for (int c = startCol; c < startCol + sqr; ++c) {
                for (int r = startRow; r < startRow + sqr; ++r) {
                  if (r != item.second && r != item2.second && c != item.first && c != item2.first) {
                    /* cout << "should remove from " << c << r << endl; */
                    for (auto i : board[item.first][item.second]) {
                      /* cout << "removing: " << i << endl; */
                      if(!clearCell(c, r, i)) {
                        return false;
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
      return true;
    }


    virtual int heuristicValue() const {
      /* return heur; */
      return completed;
    }

    int getSquare(const int & row, const int & col) const {
      SudokuSquareSet s = board[col][row];
      return s.size() == 1 ? *s.begin() : -1;
    }

    void print() {
      for (int i = 0; i < board.size(); ++i) {
        for (int j = 0; j < board[i].size(); ++j) {
          for (int v = 1; v <= board.size(); ++v) {
            if (board[j][i].find(v) != board[j][i].end()) {
              cout << v;
            } else {
              cout << "-";
            }
          }
          cout << " | ";
        }
        cout << endl;
      }
    }
};





#endif
