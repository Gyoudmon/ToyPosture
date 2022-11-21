#include "algolet.hpp"

#include "../../digitama/colorspace.hpp"
#include "../../digitama/geometry.hpp"
#include "../../digitama/named_colors.hpp"
#include "../../digitama/font.hpp"
#include "../../digitama/text.hpp"

using namespace WarGrey::STEM;
using namespace WarGrey::OS;

const float label_lineheight_ratio = 1.2F;

/*************************************************************************************************/
void WarGrey::OS::IAlgolet::fill_extent(float x, float y, float* width, float* height) {
    if (this->label_font == nullptr) {
        this->label_font = game_font::monospace;
        game_text_size(this->label_font, &this->chwidth, &this->chheight, "8");
    }

    SET_BOX(width,  this->gridsize * float(this->window_size) + this->chwidth * 4.0F);
    SET_BOX(height, this->gridsize * float(this->physical_page) + this->chheight * label_lineheight_ratio * 3.0F);
}

void WarGrey::OS::IAlgolet::draw(SDL_Renderer* renderer, float x, float y, float Width, float Height) {
    float lineheight = this->chheight * label_lineheight_ratio;
    float line_yoff = (lineheight - this->chheight) * 0.5F;
    float body_xoff = this->chwidth * 4.0F - 1.0F;
    float body_yoff = lineheight * 3.0F - 1.0F;

    /* draw title */ {
        std::string title = this->name;

        if (this->hit > 0) {
            title += " (HIT: " + std::to_string(this->hit)
                + "/" + std::to_string(this->total) + ")";
        }

        game_draw_blended_text(this->label_font, renderer, GHOSTWHITE,
                x + body_xoff + (Width - body_xoff - this->chwidth * title.size()) * 0.5F, y + line_yoff,
                title);
    }

    if ((this->physical_page > 0) && (this->window_size > 0)) {
        /* draw marks */ {
            /* draw physical page numbers */ {
                float yoff = (this->gridsize - this->chheight) * 0.5F;

                for (int i = 1; i <= this->physical_page; i++) {
                    std::string mark = std::to_string(i);
                    float phy_x = x + (this->gridsize - this->chwidth * float(mark.size())) * 0.5F;
                    float phy_y = y + body_yoff + this->gridsize * float(i - 1) + yoff;

                    game_draw_blended_text(this->label_font, renderer, DIMGRAY, phy_x, phy_y, mark);
                }
            }
    
            /* draw timeline */ {
                float t_x0 = x + body_xoff;
                float t_y = y + line_yoff + lineheight * 1.0F;

                for (int i = 1; i <= this->window_size; i++) {
                    std::string mark = std::to_string(i);
                    float xoff = (this->gridsize - this->chwidth * float(mark.size())) * 0.5F;
                    float t_x = t_x0 + this->gridsize * float(i - 1) + xoff;

                    game_draw_blended_text(this->label_font, renderer, DIMGRAY, t_x, t_y, mark);
                }
            }
    
            /* draw virtual page stream */ {
                float virt_x0 = x + body_xoff;
                float virt_y = y + line_yoff + lineheight * 2.0F;

                for (int i = 0; i < this->page_stream.size(); ++i) {
                    std::string mark = std::to_string(this->page_stream[i]);
                    float xoff = (this->gridsize - this->chwidth * float(mark.size())) * 0.5F;
                    float virt_x = virt_x0 + this->gridsize * float(i) + xoff;

                    game_draw_blended_text(this->label_font, renderer, ROYALBLUE, virt_x, virt_y, mark);
                }
            }
        }
    
        /* draw body */ {
            float bx = x + body_xoff;
            float by = y + body_yoff;

            this->draw_body(renderer, bx, by, Width - body_xoff, Height - body_yoff);
            
            RGB_SetRenderDrawColor(renderer, SILVER);
            game_draw_grid(renderer, this->window_size, this->physical_page, this->gridsize, bx, by);
        }
    }
}

/*************************************************************************************************/
void WarGrey::OS::IAlgolet::reset() {
    this->label_font = nullptr;
    this->hit = 0;
    this->total = 0;
    this->page_stream.clear();
    this->on_reset();
}

void WarGrey::OS::IAlgolet::step(int vpno) {
    this->page_stream.push_back(vpno);

    if (this->page_stream.size() > this->window_size) {
        this->page_stream.pop_front();
    }

    this->total ++;
    this->on_step(vpno);
    this->notify_updated();
}

/*************************************************************************************************/
unsigned int WarGrey::OS::IAlgolet::grid_background(PageState state) {
    unsigned int gcolor = 0x0U;

    switch (state) {
        case PageState::HIT: gcolor = GREEN; break;
        case PageState::REPLACED: gcolor = ORANGE; break;
        default: { /* Nothing to do */ }; break;
    }

    return gcolor;
}

