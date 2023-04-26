#include "avatar.hpp"

#include "../big_bang/datum/fixnum.hpp"

using namespace WarGrey::STEM;
using namespace WarGrey::IMS;

/*************************************************************************************************/
void WarGrey::IMS::AvatarPlane::load(float width, float height) {
    auto label_font = GameFont::math();
    size_t kid_count = TrailKid::name_count();
    size_t youth_count = TrailStudent::name_count();
    size_t n = fxmax(kid_count, youth_count);

    this->title = this->insert(new Labellet(GameFont::serif(FontSize::medium), BLACK, "%s", this->name()));

    for (size_t idx = 0; idx < n; idx ++) {
        this->labels.push_back(this->insert(new Labellet(label_font, BLACK, "%d", idx)));
        
        if (idx < kid_count) {
            this->kids.push_back(this->insert(new TrailKid(idx)));
        }
        
        if (idx < youth_count) {
            this->youths.push_back(this->insert(new TrailStudent(idx)));
        }
    }
}

void WarGrey::IMS::AvatarPlane::reflow(float width, float height) {
    float grid_height = 0.0F;
    float grid_y, grid_width;

    this->title->feed_extent(0.0F, 0.0F, nullptr, &grid_y);

    if (!this->kids.empty()) {
        this->kids[0]->feed_extent(0.0F, 0.0F, nullptr, &grid_height);
    }

    if (!this->youths.empty()) {
        float h;

        this->youths[0]->feed_extent(0.0F, 0.0F, nullptr, &h);
        grid_height = flmax(grid_height, h);
    }

    grid_width = grid_height * 0.618F;
    for (size_t idx = 0; idx < this->labels.size(); idx ++) {
        this->move_to(this->labels[idx], grid_width * 0.5F, grid_y + grid_height, MatterAnchor::CB);

        if (idx < this->kids.size()) {
            this->move_to(this->kids[idx], grid_width * 1.5F, grid_y + grid_height, MatterAnchor::CB);
        }

        if (idx < this->youths.size()) {
            this->move_to(this->youths[idx], grid_width * 2.5F, grid_y + grid_height, MatterAnchor::CB);
        }

        grid_y += grid_height;
    }

    this->move_to(this->title, grid_width * 1.5F, 0.0F, MatterAnchor::CT);
}
