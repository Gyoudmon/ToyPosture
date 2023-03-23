#include "randomlet.hpp"

#include "../../big_bang/game.hpp"
#include "../../big_bang/physics/random.hpp"

using namespace WarGrey::STEM;
using namespace WarGrey::OS;

/*************************************************************************************************/
void WarGrey::OS::Randomlet::draw_body(SDL_Renderer* renderer, float x, float y, float Width, float Height) {
    float yoff = (this->gridsize - this->chheight) * 0.5F;

    for (int i = 0; i < this->history.size(); i++) {
        std::vector<std::pair<int, PageState>> self = this->history[i];

        for (int j = 0; j < this->physical_page; j++) {
            float gx = x + this->gridsize * float(i);
            float gy = y + this->gridsize * float(j);
            int vp_no = self[j].first;

            if (vp_no > 0) {
                std::string mark = std::to_string(vp_no);
                uint32_t gcolor = this->grid_background(self[j].second);
                float xoff = (this->gridsize - this->chwidth * float(mark.size())) * 0.5F;
                
                game_fill_rect(renderer, gx, gy, this->gridsize, this->gridsize, gcolor);
                game_draw_blended_text(this->label_font, renderer, SNOW, gx + xoff, gy + yoff, mark);
            } else {
                game_fill_rect(renderer, gx, gy, this->gridsize, this->gridsize, DIMGRAY);
            }
        }
    }
}

/*************************************************************************************************/
void WarGrey::OS::Randomlet::on_step(int vpno) {
    std::vector<std::pair<int, PageState>> round(this->physical_page);
    
    if (this->history.empty()) {
        round[0] = std::make_pair(vpno, PageState::DEFAULT);
        for (int i = 1; i < this->physical_page; i++) {
            round[i] = std::pair(0, PageState::DEFAULT);
        }
    } else {
        std::vector<std::pair<int, PageState>> prev_one = this->history.back();
        bool needs_replace = true;

        for (int i = 0; i < this->physical_page; i++) {
            std::pair<int, PageState> self = prev_one[i];

            if (self.first == vpno) {
                round[i] = std::pair(vpno, PageState::HIT);
                needs_replace = false;
                this->hit ++;
            } else if (self.first == 0) {
                if (needs_replace) {
                    round[i] = std::pair(vpno, PageState::DEFAULT);
                    needs_replace = false;
                } else {
                    round[i] = self;
                }
            } else {
                round[i] = std::pair(self.first, PageState::DEFAULT);
            }
        }

        if (needs_replace) {
            round[random_uniform(1, this->physical_page) - 1] = std::pair(vpno, PageState::REPLACED);
        }
    }

    this->history.push_back(round);

    if (this->history.size() > this->window_size) {
        this->history.pop_front();
    }
}

void WarGrey::OS::Randomlet::on_reset() {
    this->history.clear();
}
