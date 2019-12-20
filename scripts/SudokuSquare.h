#ifndef SUDOKUSQUARE_H
#define SUDOKUSQUARE_H

// Do not add any #include statements to this file

class SudokuSquareSet {

    // TODO: write your code here
  public:
    unsigned int bit;
    int count;

    SudokuSquareSet(): bit(0), count(0) {}

    int size() const {
      return count;
    }

    bool empty() const {
      return bit == 0;
    }

    void clear() {
      bit = 0;
      count = 0;
    }

    bool operator==(const SudokuSquareSet & other) const {
      return other.bit == bit;
    }

    bool operator!=(const SudokuSquareSet & other) const {
      return bit != other.bit;
    }

    class iterator {
      private:
        // copy of the unsigned int;
        unsigned int cur;
        // position of itr;
        int pos;
      public:

        iterator(unsigned int curIn): cur(curIn) {
          pos = 1;
          while ((cur >> (pos - 1) & 1UL) == 0 && (cur >> (pos - 1)) != 0) {
            ++pos;
          }
        }
        iterator(unsigned int curIn, int posIn): cur(curIn), pos(posIn) {}

        bool operator==(const iterator & other) const {
          return (cur >> (pos - 1)) == (other.cur >> (other.pos - 1));
        }

        bool operator!=(const iterator & other) const {
          return (cur >> (pos - 1)) != (other.cur >> (other.pos - 1));
        }

        int operator*() const {
          return pos;
        }

        iterator operator++() {
          ++pos;
          while ((cur >> (pos - 1) & 1UL) == 0 && (cur >> (pos - 1)) != 0) {
            ++pos;
          }
          return *this;
        }

    };

    iterator begin() const {
      return iterator(bit);
    }

    iterator end() const {
      return iterator(0);
    }

    iterator find(int val) const {
      return (bit >> (val - 1) & 1UL) == 1 ? iterator(bit, val) : end();
    }

    iterator insert(int val) {
      if ((bit >> (val - 1) & 1UL) == 1) {
        return iterator(bit, val);
      }
      bit |= 1UL << (val - 1);
      ++count;
      return iterator(bit, val);
    }

    void erase(int val) {
      if ((bit >> (val - 1) & 1UL) == 0) {
        return;
      }
      bit &= ~(1UL << (val - 1));
      --count;
    }

    void erase(iterator pos) {
      erase(*pos);
    }
};


// Do not write any code below this line
static_assert(sizeof(SudokuSquareSet) == sizeof(unsigned int) + sizeof(int), "The SudokuSquareSet class needs to have exactly two 'int' member variables, and no others");


#endif
