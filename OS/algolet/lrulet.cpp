#include "lrulet.hpp"

#include "../digitama/graphics/text.hpp"
#include "../digitama/graphics/geometry.hpp"
#include "../digitama/graphics/named_colors.hpp"

using namespace WarGrey::STEM;
using namespace WarGrey::OS;

/*************************************************************************************************/
void WarGrey::OS::LRUlet::draw_body(SDL_Renderer* renderer, float x, float y, float Width, float Height) {
    float yoff = (this->gridsize - this->chheight) * 0.5F;

    for (int i = 0; i < this->history.size(); i++) {
        std::vector<std::tuple<int, PageState, int>> self = this->history[i];

        for (int j = 0; j < this->physical_page; j++) {
            float gx = x + this->gridsize * float(i);
            float gy = y + this->gridsize * float(j);
            int vp_no = std::get<0>(self[j]);

            if (vp_no > 0) {
                std::string mark = std::to_string(vp_no);
                float xoff = (this->gridsize - this->chwidth * float(mark.size())) * 0.5F;
                uint32_t gcolor = this->grid_background(std::get<1>(self[j]));

                game_fill_rect(renderer, gx, gy, this->gridsize, this->gridsize, gcolor);
                game_draw_blended_text(this->label_font, renderer, SNOW, gx + xoff, gy + yoff, mark);
            } else {
                game_fill_rect(renderer, gx, gy, this->gridsize, this->gridsize, DIMGRAY);
            }
        }
    }
}

/*************************************************************************************************/
void WarGrey::OS::LRUlet::on_step(int vpno) {
    std::vector<std::tuple<int, PageState, int>> round(this->physical_page);
    
    if (this->history.empty()) {
        round[0] = std::tuple<int, PageState, int>(vpno, PageState::DEFAULT, 0);
        for (int i = 1; i < this->physical_page; i++) {
            round[i] = std::tuple<int, PageState, int>(0, PageState::DEFAULT, 1);
        }
    } else {
        std::vector<std::tuple<int, PageState, int>> prev_one = this->history.back();
        PageState new_state = PageState::REPLACED;
        int replace_idx = 0;
        int max_counter = 0;

        for (int i = 0; i < this->physical_page; i++) {
            std::tuple<int, PageState, int> self = prev_one[i];
            int self_no = std::get<0>(self);
            int self_counter = std::get<2>(self);

            if (self_no == vpno) {
                new_state = PageState::HIT;
                replace_idx = i;
                this->hit ++;
            } else {    
                round[i] = std::tuple<int, PageState, int>(self_no, PageState::DEFAULT, self_counter + 1);
                
                if (new_state == PageState::REPLACED) { 
                    if (max_counter < self_counter) {
                        max_counter = self_counter;
                        replace_idx = i;

                        if (self_no == 0) {
                            new_state = PageState::DEFAULT;
                        }
                    }
                }
            }
        }

        round[replace_idx] = std::tuple<int, PageState, int>(vpno, new_state, 0);
    }

    this->history.push_back(round);

    if (this->history.size() > this->window_size) {
        this->history.pop_front();
    }
}

void WarGrey::OS::LRUlet::on_reset() {
    this->history.clear();
}

/*************************************************************************************************/
void WarGrey::OS::LRUStacklet::draw_body(SDL_Renderer* renderer, float x, float y, float Width, float Height) {
    float yoff = (this->gridsize - this->chheight) * 0.5F;

    for (int i = 0; i < this->history.size(); i++) {
        std::list<std::pair<int, PageState>> self = this->history[i];
        int j = 0;

        for (auto self_j : self) {
            float gx = x + this->gridsize * float(i);
            float gy = y + this->gridsize * float(j++);

            /* draw grid */ {
                std::string mark = std::to_string(self_j.first);
                float xoff = (this->gridsize - this->chwidth * float(mark.size())) * 0.5F;

                game_fill_rect(renderer, gx, gy, this->gridsize, this->gridsize, this->grid_background(self_j.second));
                game_draw_blended_text(this->label_font, renderer, SNOW, gx + xoff, gy + yoff, mark);
            }
        }

        for (; j < this->physical_page; j++) {
            float gx = x + this->gridsize * float(i);
            float gy = y + this->gridsize * float(j);
            
            game_fill_rect(renderer, gx, gy, this->gridsize, this->gridsize, DIMGRAY);
        }
    }
}

/*************************************************************************************************/
void WarGrey::OS::LRUStacklet::on_step(int vpno) {
    std::list<std::pair<int, PageState>> round;
    
    if (this->history.empty()) {
        round.push_front(std::pair<int, PageState>(vpno, PageState::DEFAULT));
    } else {
        std::list<std::pair<int, PageState>> prev_one = this->history.back();
        PageState new_state = PageState::DEFAULT;

        for (auto self : prev_one) {
            int self_no = self.first;

            if (self_no == vpno) {
                new_state = PageState::HIT;
                this->hit ++;
            } else {    
                round.push_back(std::pair<int, PageState>(self_no, PageState::DEFAULT));
            }
        }

        if (round.size() == this->physical_page) {
            round.pop_back();
            round.push_front(std::pair<int, PageState>(vpno, PageState::REPLACED));
        } else {
            round.push_front(std::pair<int, PageState>(vpno, new_state));
        }
    }

    this->history.push_back(round);

    if (this->history.size() > this->window_size) {
        this->history.pop_front();
    }
}

void WarGrey::OS::LRUStacklet::on_reset() {
    this->history.clear();
}

