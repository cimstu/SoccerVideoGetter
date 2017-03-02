#include "stdafx.h"
#include <sstream>
#include "blob.hpp"
#include "tag.hpp"
#include "parseutils.hpp"

content_list_t
blob_t::split () {
  std::stringstream strm(this->val);
  content_list_t cntnt;
  parse_html (strm, cntnt);
  return cntnt;
}
