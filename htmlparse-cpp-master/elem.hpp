#ifndef _ELEM_HPP
#define _ELEM_HPP

#include <list>
#include <iostream>

class elem_t;
typedef std::list<elem_t *> content_list_t;
class elem_t {
public:
  void print_n_blanks (int n) {
    for (int i = 0; i < n; i++) std::cout << " ";
  }
  virtual void dump (int level) {}
  virtual void expand () {}
  virtual content_list_t split () { return content_list_t (); }
  virtual bool is_blob () const { return true; }
};

#endif // _ELEM_HPP
