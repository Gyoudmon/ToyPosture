#include "disciplinelet.hpp"

using namespace WarGrey::STEM;
using namespace WarGrey::IMS;

/*************************************************************************************************/
#define DISCIPLINE_PATH digimon_path("IMS/discipline", "")

/*************************************************************************************************/
WarGrey::IMS::DisciplineSprite::DisciplineSprite(DisciplineType type) : Sprite(DISCIPLINE_PATH), type(type) {}

int WarGrey::IMS::DisciplineSprite::get_initial_costume_index() {
    return this->costume_name_to_index(DisciplineEntity::type_to_name(this->type));
}

void WarGrey::IMS::DisciplineSprite::set_type(DisciplineType type) {
    if (this->type != type) {
        this->type = type;
        this->switch_to_costume(DisciplineEntity::type_to_name(this->type));
    }
}
