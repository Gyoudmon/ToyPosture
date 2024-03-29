#pragma once

#include "../big_bang/game.hpp"
#include "../big_bang/bang.hpp"

#include "../big_bang/datum/box.hpp"

#include <deque>

/*************************************************************************************************/
namespace WarGrey::OS {
    enum PageState { DEFAULT, HIT, REPLACED };

    class IAlgolet : public WarGrey::STEM::IGraphlet {
        public:
            IAlgolet(const char* name, int phges, float gsize, int winsize)
                : physical_page(phges), window_size(winsize), gridsize(gsize), name(name) {}
            virtual ~IAlgolet() {}

        public:
            void feed_extent(float x, float y, float* width = nullptr, float* height = nullptr) override;
            void draw(SDL_Renderer* renderer, float x, float y, float Width, float Height) override;

        public:
            void reset();
            void step(int vpno);

        protected:
            virtual void draw_body(SDL_Renderer* renderer, float x, float y, float Width, float Height) = 0;
            virtual void on_reset() = 0;
            virtual void on_step(int vpno) = 0;

        protected:
            unsigned int grid_background(WarGrey::OS::PageState state);

        protected:
            int physical_page;
            int window_size;
            float gridsize;
            int hit = 0;
            int total = 0;

        protected:
            WarGrey::STEM::shared_font_t label_font = nullptr;
            float chwidth = 0.0F;
            float chheight = 0.0F;

        private:
            std::deque<int> page_stream;
            std::string name;
    };
}
