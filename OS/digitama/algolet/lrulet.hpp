#pragma once

#include "../algolet.hpp"

#include <deque>
#include <list>
#include <vector>
#include <utility>

/*************************************************************************************************/
namespace WarGrey::OS {
    class LRUlet : public WarGrey::OS::IAlgolet {
        public:
            LRUlet(int phges, float gsize, int winsize)
                : IAlgolet("LRU Replacement Algorithm", phges, gsize, winsize) {}

        public:
            void on_reset() override;
            void on_step(int vpno) override;

        protected:
            void draw_body(SDL_Renderer* renderer, float x, float y, float width, float height) override;

        private:
            std::deque<std::vector<std::tuple<int, WarGrey::OS::PageState, int>>> history;
    };
    
    class LRUStacklet : public WarGrey::OS::IAlgolet {
        public:
            LRUStacklet(int phges, float gsize, int winsize)
                : IAlgolet("LRU Replacement Algorithm [Stack]", phges, gsize, winsize) {}

        public:
            void on_reset() override;
            void on_step(int vpno) override;

        protected:
            void draw_body(SDL_Renderer* renderer, float x, float y, float width, float height) override;

        private:
            std::deque<std::list<std::pair<int, WarGrey::OS::PageState>>> history;
    };
}

