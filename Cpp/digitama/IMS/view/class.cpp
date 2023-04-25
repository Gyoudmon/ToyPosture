#include "class.hpp"

using namespace WarGrey::IMS;
using namespace WarGrey::STEM;

/*************************************************************************************************/
WarGrey::IMS::ClassSprite::ClassSprite(uint64_t id) : Sprite(digimon_path("IMS/classroom", ".png")), id(id) {}

const char* WarGrey::IMS::ClassSprite::name() {
    static std::string s;

    s = std::to_string(this->id);

    return s.c_str();
}
