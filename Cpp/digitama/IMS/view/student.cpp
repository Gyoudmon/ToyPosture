#include "student.hpp"

using namespace WarGrey::STEM;
using namespace WarGrey::IMS;

/*************************************************************************************************/
WarGrey::IMS::StudentSprite::StudentSprite(uint64_t No, std::string nickname, uint8_t avatar, uint8_t age)
    : Citizen(avatar, (age > 10) ? TRAIL_STUDENTS_PATH : TRAIL_KIDS_PATH), No(No), nickname(nickname) {}
