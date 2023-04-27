#include "desklet.hpp"

#include "../../big_bang/datum/flonum.hpp"

using namespace WarGrey::STEM;
using namespace WarGrey::IMS;

/*************************************************************************************************/
static float unit_rad = pi_f / 3.0F;

static MatterAnchor anchors [] = {
    MatterAnchor::CC, MatterAnchor::RC, MatterAnchor::RB,
    MatterAnchor::CB, MatterAnchor::LB, MatterAnchor::LC
};

/*************************************************************************************************/
int WarGrey::IMS::HexagonalDesklet::get_seat_by(float local_x, float local_y) {
    float theta, width, height;
    int idx = 0; 

    this->feed_extent(0.0F, 0.0F, &width, &height);
    theta = flatan(local_y - height * 0.5F, local_x - width * 0.5F);

    if (theta < 0.0F) {
        theta = pi_f * 2.0F + theta;
    }

    idx = int(flfloor(theta / unit_rad));

    return (idx == 0) ? 6 : idx;
}

void WarGrey::IMS::HexagonalDesklet::sit(ISprite* stu, int idx, double duration) {
    if (idx > 0) {
        auto master = this->master();
    
        if (master != nullptr) {
            float theta = (float(idx % this->seat_count()) + 0.5F) * unit_rad;
            float w, h, cx, cy;
            float dx = 0.0F;
            float dy = 0.0F;
            
            this->feed_extent(0.0F, 0.0F, &w, &h);
            master->feed_matter_location(this, &cx, &cy, MatterAnchor::CC);
            
            master->glide_to(duration, stu,
                w * 0.5F * flcos(theta) + cx,
                h * 0.5F * flsin(theta) + cy,
                anchors[idx - 1]);

            switch (idx) {
            case 1: case 4: dy = +1.0F; break;
            case 2: dx = +1.0F; dy = -1.0F; break;
            case 3: dx = +1.0F; dy = +1.0F; break;
            case 5: dx = -1.0F; dy = +1.0F; break;
            case 6: dx = -1.0F; dy = -1.0F; break;
            }

            master->glide(0.1, stu, dx, dy);
        }
    }
}
