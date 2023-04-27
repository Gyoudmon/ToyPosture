#pragma once

#include "../../big_bang/game.hpp"

namespace WarGrey::IMS {
    class ClassSprite : public WarGrey::STEM::Sprite {
    public:
        ClassSprite(uint64_t seq);
        virtual ~ClassSprite() {}

        const char* name() override;

    public:
        uint64_t primary_key() { return this->seq; }

    private:
        uint64_t seq;
    };
}
