#pragma once

#include "../../big_bang/game.hpp"

namespace WarGrey::IMS {
    class DoorSprite : public WarGrey::STEM::Sprite {
    public:
        DoorSprite(uint64_t seq);
        virtual ~DoorSprite() {}

        const char* name() override;
        int preferred_local_fps() override { return 24; }

    public:
        uint64_t primary_key() { return this->seq; }

    public:
        void open();
        void close();

    private:
        uint64_t seq;
    };
}
