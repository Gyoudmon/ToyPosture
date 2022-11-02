#pragma once

#include "../algolet.hpp"

#include <deque>
#include <vector>
#include <tuple>

/*************************************************************************************************/
namespace WarGrey::OS {
    class FIFOlet : public WarGrey::OS::IAlgolet {
        public:
            FIFOlet(int phges, float gsize, int winsize)
                : IAlgolet("FIFO Replacement Algorithm", phges, gsize, winsize) {}

        public:
            void on_reset() override;
            void on_step(int vpno) override;

        protected:
            void draw_body(SDL_Renderer* renderer, float x, float y, float width, float height) override;

        private:
            std::deque<std::vector<std::tuple<int, WarGrey::OS::PageState, int>>> history;
    };
}

