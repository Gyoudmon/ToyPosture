#pragma once

#include <vector>

#include "digitama/graphlet.hpp"

/*************************************************************************************************/
namespace WarGrey::OS {
    class IAlgolet : public IGraphlet {
        public:
            IAlgolet(float grid_size) : gsize(gsize) {}
            virtual ~IAlgolet();

        public:
            void fill_extent(float x, float y, float* width = nullptr, float* height = nullptr) override;
            void draw(SDL_Renderer* renderer, float x, float y, float Width, float Height) override;

        public:
            void set_physical_page_number(int pno);

        private:
            float gsize;
            int avail_phy_page;

        private:
            std::vector<int> v_pages;
            int cusor;
    };
}

