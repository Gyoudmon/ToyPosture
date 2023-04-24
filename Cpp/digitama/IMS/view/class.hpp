#pragma once

#include "../../big_bang/game.hpp"

namespace WarGrey::IMS {
    class ClassSprite : public WarGrey::STEM::Sprite {
    public:
        ClassSprite(uint64_t id);
        virtual ~ClassSprite() {}

        const char* name() override;

    public:
        uint64_t uuid() { return this->id; }

    private:
        uint64_t id;
    };
}
