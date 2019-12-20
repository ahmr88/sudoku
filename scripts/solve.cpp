#include "Sudoku.h"
#include <string>
#include <stack>
#include "BestFirstSearch.h"
#include "SudokuSquare.h"
#include <chrono>
#include <fstream>
#include <regex>

#include <iostream>
using namespace std::chrono;
using namespace std;
using std::string;


bool fillBoard(Sudoku * board, const string & fill) {
  regex digit("\\d");
  cmatch m;
  stack<char> brackets;
  int row = -1;
  int col = -1;
  for (char c : fill) {
    char cs[1];
    cs[0] = c;
    if (c == '[') {
      brackets.push(c);
      if (brackets.size() == 2) {
        ++row;
      }
    } else if (c == ']') {
      col = -1;
      brackets.pop();
    } else if (c != ' ' && c != ',') {
      ++col;
      const int setTo = std::stoi(string(1,c));
      if (setTo != 0) {
        cout << row << " , " << col << " --> " << setTo << endl;
        if (!board->setSquare(row, col, setTo)) {
          
            cout << "Error: setSquare returned false after setting square[" << row << "][" << col << "] to " << setTo << " - this should happen as the example boards are solvable\n";
            return false;
        }
      }
    }
  }
  return true;
}



int main(int argc, char** argv) {

  unique_ptr<Sudoku> board(new Sudoku(9));
  fillBoard(board.get(), argv[1]);

  BestFirstSearch search(std::move(board));

  Searchable * solution = search.solve();
  Sudoku * done = static_cast<Sudoku*>(solution);
  done->write(cout);
}



/* bool fillBoard(Sudoku * board, const vector<string> & fillWith) { */
/*     for (size_t row = 0; row < fillWith.size(); ++row) { */
/*         for (size_t col = 0; col < fillWith.size(); ++col) { */
/*             if (fillWith[row][col] != ' ') { */
/*                 const int setTo = std::stoi(fillWith[row].substr(col,1)); */
/*                 if (!board->setSquare(row, col, setTo)) { */
/*                     cout << "Error: setSquare returned false after setting square[" << row << "][" << col << "] to " << setTo << " - this should happen as the example boards are solvable\n"; */
/*                     return false; */
/*                 } */
/*             } */
/*         } */
/*     } */
/*     return true; */
/* } */



