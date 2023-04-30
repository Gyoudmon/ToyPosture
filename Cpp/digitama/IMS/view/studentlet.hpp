#pragma once

#include "../../big_bang/game.hpp"
#include "../entity/student.hpp"

namespace WarGrey::IMS {
    class StudentSprite : public WarGrey::STEM::Citizen {
    public:
        StudentSprite(uint64_t No, std::string nickname, uint8_t avatar, uint8_t age);
        virtual ~StudentSprite() {}

    public:
        void draw(SDL_Renderer* renderer, float x, float y, float Width, float Height) override;

    public:
        const char* name() override { return this->nickname.c_str(); }
        uint64_t primary_key() { return this->No; }

    public:
        void set_score_percentage(double percentage, uint32_t color = 0x00FF00U, double alpha = 1.0);

    private:
        uint64_t No;
        std::string nickname;

    private:
        uint32_t sbar_color = 0U;
        double sbar_alpha = 0.0;
        double sbar_percentage = 0.0;
    };
}
