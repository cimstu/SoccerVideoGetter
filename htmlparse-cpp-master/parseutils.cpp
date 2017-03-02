#include "stdafx.h"
#include <sstream>
#include <string>
#include "parseutils.hpp"
#include "blob.hpp"

void
get_tag_attrs (std::istream& fh, tag_t&tg) {
  std::stringstream strm;
  std::string attrname, attrval;
  while (fh.good()) {
    char c;
    fh.get(c);
    if (c == '=') {
      attrname = strm.str();
      strm = std::stringstream();
    }
    else if (c == ' ') {
      if (attrname.size()) {
        attrval = strm.str();
        tg.add_attr(attrname,attrval);
        attrname = "";
        attrval  = "";
      }
      strm = std::stringstream();
    }
    else {
      strm << c;
    }
  }
  if (attrname.size()) {
    attrval = strm.str();
    tg.add_attr(attrname,attrval);
  }
}

void
get_tag_name_and_attrs (std::istream& fh, tag_t & tg) {
  std::stringstream strm;
  bool name_found = false;
  while (fh.good()) {
    char c;
    fh.get(c);
    if (c == '>') {
      break;
    }
    else if (!name_found && c == ' ') {
      tg.set_name(strm.str());
      name_found = true;
      strm = std::stringstream();
    }
    else {
      strm << c;
    }
  }
  if (!name_found) {
    tg.set_name(strm.str());
  }
  else {
    get_tag_attrs (strm, tg);
  }
}

static bool
matches_from_end (std::string str, std::string pat) {
  std::string::reverse_iterator itr_str = str.rbegin();
  std::string::reverse_iterator itr_pat = pat.rbegin();
  for (; itr_pat != pat.rend(); itr_pat++, itr_str++) {
    if (*itr_str != *itr_pat) return false;
  }
  return true;
}

void
get_tag_content (std::istream& fh, tag_t & tg) {
  std::stringstream strm;
  std::string tgend = "</";
  tgend += tg.get_name();
  while (fh.good()) {
    char c;
    fh.get(c);
    if (matches_from_end (strm.str(),tgend)) {
      break;
    }
    else {
      strm << c;
    }
  }
  std::string cntntstr = strm.str();
  size_t tgendlen = tgend.size();
  size_t cntntstrlen = cntntstr.size();
  cntntstr.erase(cntntstrlen-tgendlen,tgendlen);
  blob_t * blb = new blob_t(cntntstr);
  tg.add_content(blb);
}

void
handle_tag (std::istream& fh, content_list_t & content) {
  tag_t * tg = new tag_t;
  get_tag_name_and_attrs (fh, *tg);
  get_tag_content (fh, *tg);
  content.push_back(tg);
}

void
parse_html (std::istream& fh, content_list_t & content) {
  while (fh.good()) {
    char c;
    fh.get(c);
    if (c == '<') {
      handle_tag (fh,content);
    }
  }
}
