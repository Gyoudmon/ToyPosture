#include "class.hpp"

using namespace WarGrey::IMS;
using namespace WarGrey::STEM;

/*************************************************************************************************/
bool WarGrey::IMS::ClassEntity::match(const std::string& line, int* offset) {
    return GMSEntity::match(line, class_mark, offset);
}

const char* WarGrey::IMS::ClassEntity::prompt() {
    return "{ seq:nat }";
}

/*************************************************************************************************/
WarGrey::IMS::ClassEntity::ClassEntity(const std::string& s, int idx) {
    size_t pos = size_t(idx);
    size_t end = s.size();
    const char* src = s.c_str();

    scan_skip_space(src, &pos, end);
    this->seq = scan_natural(src, &pos, end);
    if (this->seq == 0U) throw exn_gms("无效班级");
}

std::string WarGrey::IMS::ClassEntity::to_string() {
    return make_nstring("%c:%llu", class_mark, this->seq);
}
