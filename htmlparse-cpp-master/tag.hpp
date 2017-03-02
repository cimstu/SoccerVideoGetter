#ifndef _TAG_HPP
#define _TAG_HPP

#include <string>
#include <map>
#include <list>
#include <iostream>
#include "elem.hpp"

typedef std::string tag_name_t;
typedef std::map<std::string, std::string> attr_list_t;
class tag_t : public elem_t {
  tag_name_t name;
  attr_list_t attrs;
  content_list_t content;
public:
  void set_name(const std::string& name) { this->name = name; }
  const std::string & get_name() const { return this->name; }
  void add_content(elem_t * cntnt) { content.push_back(cntnt); }
  void add_attr(const std::string& attrname, const std::string& attrval) { attrs[attrname] = attrval; }
  virtual void expand ();
  virtual bool is_blob () const { return false; }
  virtual void dump(int level) {
    print_n_blanks(level+1);
    std::cout << "start tag " << name << "\n";
    for (auto itr : attrs) {
      print_n_blanks(level+1);
      std::cout << "attr " << itr.first << "=" << itr.second << "\n";
    }
    for (auto itr : content) {
      itr->dump(level+1);
    }
    print_n_blanks(level+1);
    std::cout << "end tag " << name << "\n";
  }
};

#endif // _TAG_HPP
