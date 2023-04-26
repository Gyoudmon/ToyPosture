#pragma once

#include "../big_bang/game.hpp"

#include <vector>

namespace WarGrey::IMS {
    /*********************************************************************************************/
    class AvatarPlane : public WarGrey::STEM::Plane {
    using Plane::Plane;

    public:
        void load(float width, float height) override;
        void reflow(float width, float height) override;

    private:
        WarGrey::STEM::Labellet* title;
        std::vector<WarGrey::STEM::Labellet*> labels;
        std::vector<WarGrey::STEM::TrailKid*> kids;
        std::vector<WarGrey::STEM::TrailStudent*> youths;
    };
}
