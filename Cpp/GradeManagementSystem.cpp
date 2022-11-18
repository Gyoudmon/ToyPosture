#include "digitama/game.hpp"
#include "digitama/cosmos.hpp"
#include "digitama/graphlet.hpp"
#include "digitama/datum/box.hpp"

#include "digitama/graphlet/ui/textlet.hpp"

using namespace WarGrey::STEM;

/*************************************************************************************************/
namespace {
    class GMSlet : public IGraphlet {
        public:
            GMSlet(uint32_t c, float s = 64.0F) : color(c), size(s) {}

        public:
            void fill_extent(float x, float y, float* width = nullptr, float* height = nullptr) override {
                SET_VALUES(width, this->size, height, this->size);
            }

            void draw(SDL_Renderer* renderer, float x, float y, float Width, float Height) override {
                game_fill_rect(renderer, x, y, Width, Height, this->color);
            }

        private:
            uint32_t color;
            float size;
    };

    class GMSPlanet : public Planet {
        public:
            GMSPlanet() : Planet("Grade Management System") {}

        public:
            void load(float width, float height) override {
                this->rbox = this->insert_one(new GMSlet(0xFF0000U));
                this->gbox = this->insert_one(new GMSlet(0x00FF00U));
                this->bbox = this->insert_one(new GMSlet(0x0000FFU));
                this->label = this->insert_one(new Labellet(game_font::unicode, "中文测试"));
            }

            void reflow(float width, float height) override {
                float x = width / 2.0F;
                float y = height / 2.0F;

                this->move_to(this->rbox, x, y, GraphletAnchor::CB);
                this->move_to(this->gbox, x, y, GraphletAnchor::RT);
                this->move_to(this->bbox, x, y, GraphletAnchor::LT);

                this->move_to(this->label, x, y + 100.0F, GraphletAnchor::CC);
            }

        private:
            IGraphlet* rbox;
            IGraphlet* gbox;
            IGraphlet* bbox;
            IGraphlet* label;
    };

    class GradeManagementSystem : public Cosmos {
        public:
            GradeManagementSystem(int fps) : Cosmos(fps) {}
        
        public:
            void construct(int argc, char* argv[]) {
                this->set_snapshot_folder("/Users/wargrey/Desktop");
                this->push_planet(new GMSPlanet());
            }
    };
}

/*************************************************************************************************/
int main(int argc, char* args[]) {
    GradeManagementSystem universe(24);

    universe.construct(argc, args);
    universe.big_bang();

    return 0;
}

