#ifndef BEST_FIRST_SEARCH_H
#define BEST_FIRST_SEARCH_H

#include "Searchable.h"
#include "Sudoku.h"

#include <deque>
#include <queue>
using std::priority_queue;

#include <memory>
using std::unique_ptr;

struct CompareSudoku {
  bool operator()(const unique_ptr<Searchable> & one, const unique_ptr<Searchable> & two) {
    return one->heuristicValue() < two->heuristicValue();
  }
};

class BestFirstSearch {

protected:

    priority_queue<unique_ptr<Searchable>, std::deque<unique_ptr<Searchable>>, CompareSudoku> Q;
    /// Make sure you increment this every time you 'expand' a node, by getting it successors and putting them on the queue
    int nodes = 0;


public:

    BestFirstSearch(std::unique_ptr<Searchable> && startFrom) {
        // TODO Put startFrom onto the queue:
        Q.push(std::move(startFrom));
    }

    int getNodesExpanded() const {
        return nodes;
    }

    Searchable * solve() {
      while (!Q.empty()) {

            if (Q.top()->isSolution()) {
                return Q.top().get(); // return the pointer
            }

            /* cout << endl; */
            /* cout << "opening new Node" << endl; */
            ++nodes; // we've now looked at one more node, increment our counter

            // Steal the pointer to the board at the front of the queue, by moving it into a unique_ptr here
            // After this, the pointer on the front of the queue is `nullptr`...

            /* unique_ptr<Searchable> test(new Sudoku(dynamic_cast<Sudoku & >(*Q.top().get()))); */
            /* unique_ptr<Searchable> current(std::move(test)); */
            unique_ptr<Searchable> current(std::move(const_cast<unique_ptr<Searchable>&>(Q.top())));

            // ...which can then be popped off the front of the queue
            Q.pop();

            // Get the successors...
            /* current->write(cout); */
            vector<unique_ptr<Searchable>> successors = current->successors();

            for (auto & successor : successors) {
                // and push each one onto the back of queue.
                Q.push(std::move(successor));
            }
      }

        // TODO Implement search, returning a pointer to the solution (if one is found)


        return nullptr;
    }
};


// Do not edit below this line

#endif
