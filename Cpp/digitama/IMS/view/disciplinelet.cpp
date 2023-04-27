#include "disciplinelet.hpp"

using namespace WarGrey::STEM;
using namespace WarGrey::IMS;

/*************************************************************************************************/
#define DISCIPLINE_PATH digimon_path("IMS/discipline", "")

/*************************************************************************************************/
WarGrey::IMS::DisciplineSprite::DisciplineSprite(uint64_t code, DisciplineType type)
    : Sprite(DISCIPLINE_PATH), code(code), type(type) {}

const char* WarGrey::IMS::DisciplineSprite::name() {
    static std::string s;

    s = make_nstring("%s[%llu]", DisciplineEntity::type_to_name(this->get_type()), this->code);

    return s.c_str();
}

int WarGrey::IMS::DisciplineSprite::get_initial_costume_index() {
    return this->costume_name_to_index(DisciplineEntity::type_to_name(this->type));
}

void WarGrey::IMS::DisciplineSprite::set_type(DisciplineType type) {
    if (this->type != type) {
        this->type = type;
        this->switch_to_costume(DisciplineEntity::type_to_name(this->type));
    }
}

DisciplineType WarGrey::IMS::DisciplineSprite::get_type() {
    return DisciplineEntity::name_to_type(this->current_costume_name());       
}
