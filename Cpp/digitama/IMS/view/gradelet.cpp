#include "gradelet.hpp"

#include "../../big_bang/datum/string.hpp"
#include "../../big_bang/datum/vector.hpp"

#include "../../big_bang/graphics/image.hpp"

using namespace WarGrey::STEM;
using namespace WarGrey::IMS;

/*************************************************************************************************/
static const float horizontal_inset = 8.0F;

static const uint32_t head_color = GHOSTWHITE;
static const uint32_t body_color = SNOW;
static const uint32_t tail_color = WHITESMOKE;
static const uint32_t line_color = LIGHTGRAY;

#define HEAD_FONT GameFont::serif(FontSize::medium)
#define BODY_FONT GameFont::monospace()
#define MATH_FONT GameFont::math()

/*************************************************************************************************/
WarGrey::IMS::Gradelet::Gradelet(const char* name, float width) : _name(name), width(width) {
    this->camouflage(true);
    this->enable_resize(false);
}

void WarGrey::IMS::Gradelet::construct(SDL_Renderer* renderer) {
    this->title = std::make_shared<Texture>(game_blended_text_texture(renderer, this->_name, HEAD_FONT, head_color));
    this->timestamp = std::make_shared<Texture>(game_blended_text_texture(renderer, " ", MATH_FONT, tail_color));
    this->total_score = std::make_shared<Texture>(game_blended_text_texture(renderer, "-", MATH_FONT, tail_color));
}

void WarGrey::IMS::Gradelet::feed_extent(float x, float y, float* width, float* height) {
    float title_height, total_score_height;
    float line_count = float(this->disciplines.size());

    this->title->feed_extent(nullptr, &title_height);
    this->total_score->feed_extent(nullptr, &total_score_height);

    SET_BOX(width, flabs(this->width));
    SET_BOX(height, title_height + total_score_height + this->score_line_height() * line_count);
}

void WarGrey::IMS::Gradelet::draw(SDL_Renderer* renderer, float x, float y, float width, float height) {
    float dis_height = this->score_line_height();
    float rx = x + width;
    float by = y + height;
    float dis_lx = x + horizontal_inset;
    float dis_rx = rx - horizontal_inset;    
    float cw, ch;
    
    /* background decoration */ {
        game_fill_rect(renderer, x, y, width, height, BLACK, 0.5);
        game_draw_rect(renderer, x, y, width, height, ROYALBLUE, 1.0);
    }

    /* draw title */ {
        this->title->feed_extent(&cw, &ch);
        game_draw_image(renderer, this->title->self(), x + (width - cw) * 0.5F, y);
        game_draw_line(renderer, x, y + ch, rx, y + ch, line_color);
    }

    if (dis_height > 0.0F) {
        float dis_row_by = y + ch + dis_height;
        
        for (size_t idx = 0; idx < this->disciplines.size(); idx ++) {
            float dff_w = 0.0F;
        
            this->disciplines[idx]->feed_extent(&cw, &ch);
            game_draw_image(renderer, this->disciplines[idx]->self(), dis_lx, dis_row_by - ch, cw, ch);
        
            if (this->diffs[idx].use_count() > 0) {
                this->diffs[idx]->feed_extent(&dff_w, &ch);
                game_draw_image(renderer, this->diffs[idx]->self(), dis_rx - dff_w, dis_row_by - ch, dff_w, ch);
            }
        
            this->scores[idx]->feed_extent(&cw, &ch);
            game_draw_image(renderer, this->scores[idx]->self(), dis_rx - cw - dff_w, dis_row_by - ch, cw, ch);
            
            dis_row_by += dis_height;
        }
    }

    /* draw tail */ {
        this->total_score->feed_extent(&cw, &ch);
        game_draw_image(renderer, this->timestamp->self(), dis_lx, by - ch);
        game_draw_image(renderer, this->total_score->self(), dis_rx - cw, by - ch);
        game_draw_line(renderer, x, by - ch, rx, by - ch, line_color);
    }
}

void WarGrey::IMS::Gradelet::set_title(const std::string& title, WarGrey::STEM::MatterAnchor anchor) {
    SDL_Renderer* renderer = this->master_renderer();

    if (renderer != nullptr) {
        this->moor(anchor);
        this->title.reset(new Texture(game_blended_text_texture(renderer, title, HEAD_FONT, head_color)));
        this->notify_updated();
        this->clear_moor();
    }
}

void WarGrey::IMS::Gradelet::set_title(const char* title, ...) {
    VSNPRINT(t, title);
    this->set_title(t);
}

void WarGrey::IMS::Gradelet::set_title(WarGrey::STEM::MatterAnchor anchor, const char* title, ...) {
    VSNPRINT(t, title);
    this->set_title(t, anchor);
}

void WarGrey::IMS::Gradelet::set_disciplines(const std::vector<DisciplineType>& dis) {
    SDL_Renderer* renderer = this->master_renderer();

    if (renderer != nullptr) {
        size_t total = dis.size();

        this->disciplines.resize(total);
        this->scores.resize(total);
        this->diffs.resize(total);
        this->percentages.resize(total);

        for (size_t idx = 0; idx < total; idx ++) {
            std::string name = make_nstring("%s", DisciplineEntity::type_to_name(dis[idx]));

            this->disciplines[idx].reset(new Texture(game_blended_text_texture(renderer, name, BODY_FONT, body_color)));
            this->scores[idx].reset(new Texture(game_blended_text_texture(renderer, "-", MATH_FONT, body_color)));
            this->diffs[idx].reset();
            this->percentages[idx].clear();
        }

        this->notify_updated();
    }
}

void WarGrey::IMS::Gradelet::set_scores(uint64_t timestamp, const std::vector<double>& scores) {
    SDL_Renderer* renderer = this->master_renderer();

    if (renderer != nullptr) {
        size_t having = this->scores.size();
        size_t given = scores.size();
        double total = flnan;
        
        for (size_t idx = 0; idx < having; idx ++) {
            if (idx < given) {
                double s = scores[idx];

                if (s >= 0.0) {
                    this->scores[idx].reset(new Texture(game_blended_text_texture(renderer,
                        make_nstring("%.2f", scores[idx]), MATH_FONT, this->score_color(scores, s))));
                    total = flisnan(total) ? s : total + s; 
                } else {
                    this->clear_score(renderer, idx);
                }
            } else {
                this->clear_score(renderer, idx);
            }
        }

        this->set_total_score(renderer, timestamp, total);
        this->notify_updated();
    }
}

void WarGrey::IMS::Gradelet::set_score_via_points(uint64_t timestamp, const std::vector<std::vector<double>>& score_pts) {
    SDL_Renderer* renderer = this->master_renderer();

    if (renderer != nullptr) {
        size_t having = this->scores.size();
        size_t given = scores.size();
        double total = flnan;
        
        for (size_t idx = 0; idx < having; idx ++) {
            if (idx < given) {
                std::vector<double> pts = score_pts[idx];

                if (pts.size() > 0) {
                    double s = vector_sum(pts);

                    if (s > 0.0) {
                        this->scores[idx].reset(new Texture(game_blended_text_texture(renderer,
                            make_nstring("%.1f", s), MATH_FONT, this->score_color(pts, s))));
                        total = flisnan(total) ? s : total + s; 
                    } else {
                        this->clear_score(renderer, idx);
                    }
                } else {
                    this->clear_score(renderer, idx);
                }
            } else {
                this->clear_score(renderer, idx);
            }
        }

        this->set_total_score(renderer, timestamp, total);
        this->notify_updated();
    }
}

void WarGrey::IMS::Gradelet::set_score_diffs(const std::vector<double>& diffs) {
    SDL_Renderer* renderer = this->master_renderer();

    if (renderer != nullptr) {
        size_t total = this->scores.size();
        size_t given = scores.size();
        
        for (size_t idx = 0; idx < total; idx ++) {
            if (idx < given) {
                double diff = diffs[idx];

                if (diff > 0.0) {
                    this->diffs[idx].reset(new Texture(game_blended_text_texture(renderer,
                        make_nstring("[+%.1f]", diff), MATH_FONT, GREEN)));
                } else if (diff < 0.0) {
                    this->diffs[idx].reset(new Texture(game_blended_text_texture(renderer,
                        make_nstring("[%.1f]", diff), MATH_FONT, ORANGERED)));
                } else if (diff == 0.0) {
                    this->diffs[idx].reset(new Texture(game_blended_text_texture(renderer,
                        "[=0.0]", MATH_FONT, DIMGRAY)));
                } else { // NaN
                    this->diffs[idx].reset();
                }
            } else {
                this->diffs[idx].reset();
            }
        }

        this->notify_updated();
    }
}

void WarGrey::IMS::Gradelet::set_total_score(SDL_Renderer* renderer, uint64_t timestamp, double score) {
    this->timestamp.reset(new Texture(game_blended_text_texture(renderer,
        (timestamp > 0U) ? make_nstring("%llu", timestamp) : "-",
        MATH_FONT, tail_color)));

    this->total_score.reset(new Texture(game_blended_text_texture(renderer,
        (score >= 0.0) ? make_nstring("%.1f", score) : "-",
        MATH_FONT, tail_color)));
}

void WarGrey::IMS::Gradelet::clear(MatterAnchor anchor) {
    SDL_Renderer* renderer = this->master_renderer();

    if (renderer != nullptr) {
        this->moor(anchor);

        this->set_title(" ");

        for (size_t idx = 0; idx < this->scores.size(); idx ++) {
            this->clear_score(renderer, idx);
        }

        this->notify_updated();
        this->clear_moor();
    }
}

void WarGrey::IMS::Gradelet::clear_score(SDL_Renderer* renderer, int idx) {
    this->scores[idx].reset(new Texture(game_blended_text_texture(renderer, "-", MATH_FONT, body_color)));
    this->diffs[idx].reset();
}

/**************************************************************************************************/
float WarGrey::IMS::Gradelet::score_line_height() {
    float score_height = 0.0F;

    if (this->disciplines.size() > 0) {
        float dh, sh;

        this->disciplines[0]->feed_extent(nullptr, &dh);
        this->scores[0]->feed_extent(nullptr, &sh);
        score_height = flmax(dh, sh);
    }

    return score_height;
}

uint32_t WarGrey::IMS::Gradelet::score_color(const std::vector<double>& scores, double score) {
    return body_color;
}
