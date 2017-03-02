#ifndef _BLOB_HPP
#define _BLOB_HPP

#include <string>
#include <iostream>
#include "elem.hpp"

class blob_t : public elem_t {
  std::string val;
public:
  blob_t (const std::string& txt) : val(txt) {}
  virtual content_list_t split ();
  virtual void dump(int level) {
    for (int i = 0; i < level+1; i++) std::cout << " ";
    std::cout << "start blob\n" << val << "\n";
    for (int i = 0; i < level+1; i++) std::cout << " ";
    std::cout << "end blob\n";
  }
};

#endif // _BLOB_HPP
