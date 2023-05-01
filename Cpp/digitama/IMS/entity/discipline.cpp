#include "discipline.hpp"

#include <strings.h>

using namespace WarGrey::IMS;
using namespace WarGrey::STEM;

/*************************************************************************************************/
bool WarGrey::IMS::DisciplineEntity::match(const std::string& line, int* offset) {
    return GMSEntity::match(line, discipline_mark, offset);
}

const char* WarGrey::IMS::DisciplineEntity::prompt() {
    return "{ code:nat, name:str, credit:nat }";
}

const char* WarGrey::IMS::DisciplineEntity::update_prompt() {
    return "{ code:nat, credit:nat }";
}

const char* WarGrey::IMS::DisciplineEntity::type_to_name(DisciplineType type) {
    switch (type) {
    case DisciplineType::Mathematics: return "Mathematics";
    case DisciplineType::Programming: return "Programming";
    case DisciplineType::Physics: return "Physics";
    case DisciplineType::Chemistry: return "Chemistry";
    case DisciplineType::Biology: return "Biology";
    default: return "Blank";
    }
}

DisciplineType WarGrey::IMS::DisciplineEntity::name_to_type(const char* name) {
    if ((strcasecmp(name, "mathematics") == 0) || (strcasecmp(name, "math") == 0)) {
        return DisciplineType::Mathematics;
    } else if ((strcasecmp(name, "programming") == 0) || (strcasecmp(name, "coding") == 0)) {
        return DisciplineType::Programming;
    } else if (strcasecmp(name, "physics") == 0) {
        return DisciplineType::Physics;
    } else if (strcasecmp(name, "chemistry") == 0) {
        return DisciplineType::Chemistry;
    } else if ((strcasecmp(name, "biology") == 0) || (strcasecmp(name, "bio") == 0)) {
        return DisciplineType::Biology;
    } else {
        return DisciplineType::Blank;
    }
}

/*************************************************************************************************/
WarGrey::IMS::DisciplineEntity::DisciplineEntity(const std::string& s, int idx) {
    size_t pos = size_t(idx);
    size_t end = s.size();
    const char* src = s.c_str();

    scan_skip_space(src, &pos, end);

    this->code = scan_natural(src, &pos, end);
    if (this->code == 0U) throw exn_gms("无效课程代号");
    scan_skip_delimiter(src, &pos, end, field_delimiter);

    this->type = name_to_type(scan_string(src, &pos, end, field_delimiter).c_str());
    scan_skip_delimiter(src, &pos, end, field_delimiter);

    this->credit = scan_natural(src, &pos, end);
}

bool WarGrey::IMS::DisciplineEntity::update(const char* s, size_t end, size_t idx) {
    uint64_t new_credit = 0U;

    scan_skip_space(s, &idx, end);
    new_credit = scan_natural(s, &idx, end);

    if (new_credit > 0U) {
        this->credit = new_credit;
    }

    return this->credit == new_credit;
}

std::string WarGrey::IMS::DisciplineEntity::to_string() {
    return make_nstring("%c:%llu,%s,%llu", discipline_mark, this->code, this->cannonical_name(), this->credit);
}

const char* WarGrey::IMS::DisciplineEntity::cannonical_name() {
    return DisciplineEntity::type_to_name(this->type);
}
