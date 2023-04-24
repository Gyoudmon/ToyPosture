#include "class.hpp"

using namespace WarGrey::IMS;
using namespace WarGrey::STEM;

/*************************************************************************************************/
static const char line_mark = 'c';

int WarGrey::IMS::ClassEntity::match(const std::string& line) {
    int idx = -1;

    if (line.size() > 2) {
        if ((line[0] == line_mark) && (line[1] == ':')) {
            idx = 2;
        }
    }

    return idx;
}

const char* WarGrey::IMS::ClassEntity::prompt() {
    return "%{id:nat}";
}

/*************************************************************************************************/
WarGrey::IMS::ClassEntity::ClassEntity(const std::string& s, int idx) {
    size_t pos = size_t(idx);
    size_t end = s.size();
    const char* src = &s[idx];

    this->id = scan_natural(src, &pos, end);
}

std::string WarGrey::IMS::ClassEntity::to_string() {
    return make_nstring("%c:%lld", line_mark, this->id);
}
