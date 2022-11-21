#include "digitama/game.hpp"
#include "digitama/world.hpp"

#include "digitama/datum/box.hpp"

#include "digitama/graphlet/shapelet.hpp"
#include "digitama/graphlet/ui/textlet.hpp"

using namespace WarGrey::STEM;

/*************************************************************************************************/
namespace {
    class GradeManagementSystem : public World {
        public:
            GradeManagementSystem(int fps) : World("Grade Management System", fps) {
                this->set_snapshot_folder("/Users/wargrey/Desktop");
                this->set_cmdwin_height(24);
            }

        public:
            void construct(int argc, char* argv[]) override {
            }

            void load(float width, float height) override {
                this->rbox = self->insert_one(new RegularPolygonlet(6, 32.0F, 0xFF0000U));
                this->gbox = self->insert_one(new RegularPolygonlet(6, 32.0F, 0x00FF00U));
                this->bbox = self->insert_one(new RegularPolygonlet(6, 32.0F, 0x0000FFU));
            }

            void reflow(float width, float height) override {
                float x = width / 2.0F;
                float y = height / 2.0F;

                self->move_to(this->rbox, x, y, GraphletAnchor::CB);
                self->move_to(this->gbox, x, y, GraphletAnchor::RT);
                self->move_to(this->bbox, x, y, GraphletAnchor::LT);
            }

        private:
            IGraphlet* rbox;
            IGraphlet* gbox;
            IGraphlet* bbox;
    };
}

/*************************************************************************************************/
int main(int argc, char* args[]) {
    GradeManagementSystem universe(24);

    universe.construct(argc, args);
    universe.big_bang();

    return 0;
}

