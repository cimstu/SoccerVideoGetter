#include "stdafx.h"
#include <sstream>
#include "blob.hpp"
#include "htmldoc.hpp"
#include "parseutils.hpp"

void htmldoc_t::populate (char * fname) {
  std::ifstream fh(fname);
  parse_html (fh, content);
  for (auto & itr : content) {
    itr->expand();
  }
}
