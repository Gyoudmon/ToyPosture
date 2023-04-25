#include "class.hpp"

using namespace WarGrey::IMS;
using namespace WarGrey::STEM;

/*************************************************************************************************/
static const char line_mark = 'c';

bool WarGrey::IMS::ClassEntity::match(const std::string& line, int* offset) {
    return GMSEntity::match(line, line_mark, offset);
}

const char* WarGrey::IMS::ClassEntity::prompt() {
    return "%{id:nat}";
}

/*************************************************************************************************/
WarGrey::IMS::ClassEntity::ClassEntity(const std::string& s, int idx) {
    size_t pos = size_t(idx);
    size_t end = s.size();
    const char* src = s.c_str();

    this->id = scan_natural(src, &pos, end);
}

std::string WarGrey::IMS::ClassEntity::to_string() {
    return make_nstring("%c:%lld", line_mark, this->id);
}
