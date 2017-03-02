#include "stdafx.h"
#include <iostream>
#include "htmldoc.hpp"

int main (int argc, char ** argv) {
  if (argc > 1) {
    htmldoc_t obj(argv[1]);
    std::cout << "Start dump\n";
    obj.dump();
    std::cout << "End dump\n";
  }
  return 0;
}
