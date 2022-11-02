#pragma once

#include "../algolet.hpp"

#include <deque>
#include <vector>
#include <utility>

/*************************************************************************************************/
namespace WarGrey::OS {
    class Randomlet : public WarGrey::OS::IAlgolet {
        public:
            Randomlet(int phges, float gsize, int winsize)
                : IAlgolet("Random Replacement Algorithm", phges, gsize, winsize) {}

        public:
            void on_reset() override;
            void on_step(int vpno) override;

        protected:
            void draw_body(SDL_Renderer* renderer, float x, float y, float width, float height) override;

        private:
            std::deque<std::vector<std::pair<int, WarGrey::OS::PageState>>> history;
    };
}

