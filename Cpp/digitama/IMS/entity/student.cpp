#include "student.hpp"

#include <strings.h>

using namespace WarGrey::IMS;
using namespace WarGrey::STEM;

/*************************************************************************************************/
static const char line_mark = 's';

static const char* gender_to_name(StudentGender gender) {
    switch (gender) {
    case StudentGender::Girl: return "Girl";
    case StudentGender::Boy: return "Boy";
    default: return "-";
    }
}

static StudentGender name_to_gender(const char* gender) {
    if ((strcasecmp(gender, "girl") == 0) || (strcasecmp(gender, "g") == 0)) {
        return StudentGender::Girl;
    } else if ((strcasecmp(gender, "boy") == 0) || (strcasecmp(gender, "b") == 0)) {
        return StudentGender::Boy;
    } else {
        return StudentGender::_;
    }
}
/*************************************************************************************************/

bool WarGrey::IMS::StudentEntity::match(const std::string& line, int* offset) {
    return GMSEntity::match(line, line_mark, offset);
}

const char* WarGrey::IMS::StudentEntity::prompt() {
    return "{ avatar:byte , No:nat, nickname:str, gender:str, age:byte }";
}

/*************************************************************************************************/
WarGrey::IMS::StudentEntity::StudentEntity(const std::string& s, int idx) {
    size_t pos = size_t(idx);
    size_t end = s.size();
    const char* src = s.c_str();

    this->avatar = scan_natural(src, &pos, end) % 0x100;
    scan_skip_delimiter(src, &pos, end, FIELD_DELIM);

    this->No = scan_natural(src, &pos, end);
    if (this->No == 0U) throw exn_gms("无效学号");
    scan_skip_delimiter(src, &pos, end, FIELD_DELIM);
    
    this->nickname = scan_string(src, &pos, end, FIELD_DELIM);
    scan_skip_delimiter(src, &pos, end, FIELD_DELIM);
    
    this->gender = name_to_gender(scan_string(src, &pos, end, FIELD_DELIM).c_str());
    if (this->gender == StudentGender::_) throw exn_gms("无效性别");
    scan_skip_delimiter(src, &pos, end, FIELD_DELIM);
    
    this->age = scan_natural(src, &pos, end) % 0x100;
}

std::string WarGrey::IMS::StudentEntity::to_string() {
    return make_nstring("%c:%u, %llu, %s, %s, %u, %llu", line_mark,
            this->avatar, this->No,
            this->nickname.c_str(), gender_to_name(this->gender),
            this->age);
}
