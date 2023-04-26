#include "class.hpp"

using namespace WarGrey::IMS;
using namespace WarGrey::STEM;

/*************************************************************************************************/
#define DOOR_PATH digimon_path("IMS/door", ".png")

/*************************************************************************************************/
WarGrey::IMS::ClassSprite::ClassSprite(uint64_t seq) : Sprite(DOOR_PATH), seq(seq) {}

const char* WarGrey::IMS::ClassSprite::name() {
    static std::string s;

    s = std::to_string(this->seq);

    return s.c_str();
}
