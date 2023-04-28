#include "doorlet.hpp"

using namespace WarGrey::IMS;
using namespace WarGrey::STEM;

/*************************************************************************************************/
#define DOOR_PATH digimon_path("IMS/door", "")

/*************************************************************************************************/
WarGrey::IMS::DoorSprite::DoorSprite(uint64_t seq) : Sprite(DOOR_PATH), seq(seq) {}

void WarGrey::IMS::DoorSprite::open() {
    this->play("door", 1);
}

void WarGrey::IMS::DoorSprite::close() {
    this->stop();
    this->switch_to_costume("door");
}

const char* WarGrey::IMS::DoorSprite::name() {
    static std::string s;

    s = std::to_string(this->seq);

    return s.c_str();
}
