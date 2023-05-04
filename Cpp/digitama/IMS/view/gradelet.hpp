#pragma once

#include "../../big_bang/game.hpp"
#include "../entity/discipline.hpp"

#include <vector>

namespace WarGrey::IMS {
    class Gradelet : public WarGrey::STEM::IGraphlet {
    public:
        Gradelet(const char* name, float width);
        virtual ~Gradelet() noexcept {}

        const char* name() override { return this->_name.c_str(); }

    public:
        void construct(SDL_Renderer* renderer) override;
        void feed_extent(float x, float y, float* width, float* height) override;
        void draw(SDL_Renderer* renderer, float x, float y, float width, float height) override;

    public:
        void set_title(const char* title, ...);
        void set_title(const std::string& title) { this->set_title(title, WarGrey::STEM::MatterAnchor::CB); }
        void set_title(WarGrey::STEM::MatterAnchor anchor, const char* title, ...);
        void set_title(const std::string& title, WarGrey::STEM::MatterAnchor anchor);
        void set_disciplines(const std::vector<WarGrey::IMS::DisciplineType>& dis, WarGrey::STEM::MatterAnchor anchor);
        void set_scores(uint64_t timestamp, const std::vector<double>& scores);
        void set_score_via_points(uint64_t timestamp, const std::vector<std::vector<double>>& score_pts);
        void set_score_diffs(const std::vector<double>& diffs);
        void clear(WarGrey::STEM::MatterAnchor anchor = WarGrey::STEM::MatterAnchor::CB);

    protected:
        virtual uint32_t score_color(const std::vector<double>& scores, double score);

    private:
        float score_line_height();
        void clear_score(SDL_Renderer* renderer, int idx);
        void set_total_score(SDL_Renderer* renderer, uint64_t timestamp, double score);

    private:
        WarGrey::STEM::shared_texture_t title;
        WarGrey::STEM::shared_texture_t total_score;
        WarGrey::STEM::shared_texture_t timestamp;
        std::vector<WarGrey::STEM::shared_texture_t> disciplines;
        std::vector<WarGrey::STEM::shared_texture_t> scores;
        std::vector<WarGrey::STEM::shared_texture_t> diffs;
        std::vector<std::vector<double>> percentages;

    private:
        std::string _name;
        float width;
    };
}
