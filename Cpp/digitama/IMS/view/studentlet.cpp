#include "studentlet.hpp"

using namespace WarGrey::STEM;
using namespace WarGrey::IMS;

/*************************************************************************************************/
WarGrey::IMS::StudentSprite::StudentSprite(uint64_t No, std::string nickname, uint8_t avatar, uint8_t age)
    : Citizen(avatar, (age > 10) ? TRAIL_STUDENTS_PATH : TRAIL_KIDS_PATH), No(No), nickname(nickname) {}

void WarGrey::IMS::StudentSprite::draw(SDL_Renderer* renderer, float x, float y, float Width, float Height) {
    Sprite::draw(renderer, x, y, Width, Height);

    if ((this->sbar_alpha > 0.0) && (this->sbar_percentage > 0.0)) {
        float ly = y + 1.0F;

        game_draw_line(renderer, x, ly,
            x + Width * this->sbar_percentage, ly,
            this->sbar_color, this->sbar_alpha);
    }
}

void WarGrey::IMS::StudentSprite::set_score_percentage(double percentage, uint32_t color, double alpha) {
    if ((this->sbar_percentage != percentage) || (color != this->sbar_color) || (alpha != this->sbar_alpha)) {
        this->sbar_percentage = percentage;
        this->sbar_color = color;
        this->sbar_alpha = alpha;

        this->notify_updated();
    }
}
