#pragma once

#include "../../big_bang/game.hpp"
#include "../entity/discipline.hpp"

namespace WarGrey::IMS {
    class DisciplineSprite : public WarGrey::STEM::Sprite {
    public:
        DisciplineSprite(uint64_t code, DisciplineType type);
        virtual ~DisciplineSprite() {}

    public:
        const char* name() override;
        uint64_t primary_key() { return this->code; }

    public:
        void set_type(DisciplineType type);
        DisciplineType get_type();

    protected:
        virtual int get_initial_costume_index() override;

    private:
        uint64_t code;
        DisciplineType type;
    };
}
