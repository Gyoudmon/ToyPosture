#include "seat.hpp"

using namespace WarGrey::IMS;
using namespace WarGrey::STEM;

/*************************************************************************************************/
bool WarGrey::IMS::SeatEntity::match(const std::string& line, int* offset) {
    return GMSEntity::match(line, student_mark, class_mark, offset);
}

const char* WarGrey::IMS::SeatEntity::prompt() {
    return "{ sNo:nat, clsId:nat, deak:nat, seat:nat }";
}

/*************************************************************************************************/
WarGrey::IMS::SeatEntity::SeatEntity(const std::string& s, int idx) {
    size_t pos = size_t(idx);
    size_t end = s.size();
    const char* src = s.c_str();

    scan_skip_space(src, &pos, end);

    this->student_No = scan_natural(src, &pos, end);
    if (this->student_No == 0U) throw exn_gms("无效学号");
    scan_skip_delimiter(src, &pos, end, field_delimiter);

    this->class_id = scan_natural(src, &pos, end);
    if (this->class_id == 0U) throw exn_gms("无效班号");
    scan_skip_delimiter(src, &pos, end, field_delimiter);
    
    this->desk = scan_natural(src, &pos, end);
    scan_skip_delimiter(src, &pos, end, field_delimiter);
    
    this->seat = scan_natural(src, &pos, end);
    scan_skip_delimiter(src, &pos, end, field_delimiter);
}

WarGrey::IMS::SeatEntity::SeatEntity(uint64_t sNo, uint64_t clsId, uint64_t dsk, uint64_t st)
    : student_No(sNo), class_id(clsId), desk(dsk), seat(st) {}

std::string WarGrey::IMS::SeatEntity::to_string() {
    return make_nstring("%c%c:%llu,%llu,%llu,%llu",
            student_mark, class_mark,
            this->student_No, this->class_id,
            this->desk, this->seat);
}
