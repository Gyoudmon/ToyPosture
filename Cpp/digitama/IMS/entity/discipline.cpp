#include "discipline.hpp"

#include <strings.h>

using namespace WarGrey::IMS;
using namespace WarGrey::STEM;

/*************************************************************************************************/
static const char line_mark = 'd';

bool WarGrey::IMS::DisciplineEntity::match(const std::string& line, int* offset) {
    return GMSEntity::match(line, line_mark, offset);
}

const char* WarGrey::IMS::DisciplineEntity::prompt() {
    return "{ code:nat, name:str, score:nat }";
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

    this->code = scan_natural(src, &pos, end);
    if (this->code == 0U) throw exn_gms("无效课程代号");
    scan_skip_delimiter(src, &pos, end, FIELD_DELIM);

    this->type = name_to_type(scan_string(src, &pos, end, FIELD_DELIM).c_str());
    scan_skip_delimiter(src, &pos, end, FIELD_DELIM);

    this->score = scan_natural(src, &pos, end);
}

std::string WarGrey::IMS::DisciplineEntity::to_string() {
    return make_nstring("%c:%llu, %s, %llu", line_mark, this->code, this->cannonical_name(), this->score);
}

const char* WarGrey::IMS::DisciplineEntity::cannonical_name() {
    return DisciplineEntity::type_to_name(this->type);
}
