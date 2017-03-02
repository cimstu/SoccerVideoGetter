#ifndef _PARSEUTILS_HPP
#define _PARSEUTILS_HPP

#include <istream>
#include "tag.hpp"

void get_tag_name_and_attrs (std::istream& fh, tag_t & tg);
void get_tag_content (std::istream& fh, tag_t & tg);
void handle_tag (std::istream& fh, content_list_t & content);
void parse_html (std::istream& fh, content_list_t & content);

#endif // _PARSEUTILS_HPP
