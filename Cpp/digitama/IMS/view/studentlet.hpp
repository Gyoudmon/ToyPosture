#pragma once

#include "../../big_bang/game.hpp"
#include "../entity/student.hpp"

namespace WarGrey::IMS {
    class StudentSprite : public WarGrey::STEM::Citizen {
    public:
        StudentSprite(uint64_t No, std::string nickname, uint8_t avatar, uint8_t age);
        virtual ~StudentSprite() {}

    public:
        const char* name() override { return this->nickname.c_str(); }
        uint64_t primary_key() { return this->No; }

    private:
        uint64_t No;
        std::string nickname;
    };
}
