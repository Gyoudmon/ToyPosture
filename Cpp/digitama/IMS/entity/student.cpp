#include "student.hpp"

#include <strings.h>

using namespace WarGrey::IMS;
using namespace WarGrey::STEM;

/*************************************************************************************************/
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
    return GMSEntity::match(line, student_mark, offset);
}

const char* WarGrey::IMS::StudentEntity::prompt() {
    return "{ avatar:byte , No:nat, nickname:str, gender:str, age:byte }";
}

const char* WarGrey::IMS::StudentEntity::update_prompt() {
    return "{ No:nat, nickname:str, age:byte }";
}

/*************************************************************************************************/
WarGrey::IMS::StudentEntity::StudentEntity(const std::string& s, int idx) {
    size_t pos = size_t(idx);
    size_t end = s.size();
    const char* src = s.c_str();

    scan_skip_space(src, &pos, end);

    this->avatar = scan_natural(src, &pos, end) % 0x100;
    scan_skip_delimiter(src, &pos, end, field_delimiter);

    this->No = scan_natural(src, &pos, end);
    if (this->No == 0U) throw exn_gms("无效学号");
    scan_skip_delimiter(src, &pos, end, field_delimiter);
    
    this->nickname = scan_string(src, &pos, end, field_delimiter);
    if (this->nickname.empty()) throw exn_gms("无效昵称");
    scan_skip_delimiter(src, &pos, end, field_delimiter);
    
    this->gender = name_to_gender(scan_string(src, &pos, end, field_delimiter).c_str());
    if (this->gender == StudentGender::_) throw exn_gms("无效性别");
    scan_skip_delimiter(src, &pos, end, field_delimiter);
    
    this->age = scan_natural(src, &pos, end) % 0x100;
}

bool WarGrey::IMS::StudentEntity::update(const char* s, size_t end, size_t idx) {
    std::string new_nname = "";
    uint64_t new_age = 0U;
    bool updated = false;
    
    scan_skip_space(s, &idx, end);
    new_nname = scan_string(s, &idx, end, field_delimiter);
    scan_skip_delimiter(s, &idx, end, field_delimiter);
    new_age = scan_natural(s, &idx, end) % 0x100;

    if (!new_nname.empty()) {
        this->nickname = new_nname;
        updated = true;
    }

    if (new_age > 0U) {
        this->age = new_age;
        updated = true;
    }

    return updated;
}

std::string WarGrey::IMS::StudentEntity::to_string() {
    return make_nstring("%c:%u,%llu,%s,%s,%u", student_mark,
            this->avatar, this->No,
            this->nickname.c_str(), gender_to_name(this->gender),
            this->age);
}
