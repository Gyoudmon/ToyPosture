#include "digitama/big_bang/game.hpp"

#include "digitama/big_bang/datum/box.hpp"

#include "digitama/big_bang/matter/graphlet/shapelet.hpp"

using namespace WarGrey::STEM;

/*************************************************************************************************/
namespace {
    class GradeManagementPlane : public Plane {
    public:
        GradeManagementPlane() : Plane("Grade Management System") { }

    public:
        void load(float width, float height) override {
            this->rbox = this->insert(new RegularPolygonlet(6, 32.0F, 0xFF0000U));
            this->gbox = this->insert(new RegularPolygonlet(6, 32.0F, 0x00FF00U));
            this->bbox = this->insert(new RegularPolygonlet(6, 32.0F, 0x0000FFU));
        }

        void reflow(float width, float height) override {
            float x = width / 2.0F;
            float y = height / 2.0F;

            this->move_to(this->rbox, x, y, MatterAnchor::CB);
            this->move_to(this->gbox, x, y, MatterAnchor::RT);
            this->move_to(this->bbox, x, y, MatterAnchor::LT);
        }

    private:
        IGraphlet* rbox;
        IGraphlet* gbox;
        IGraphlet* bbox;
    };

    class GradeManagementSystem : public Cosmos {
    public:
        GradeManagementSystem(int fps) : Cosmos(fps) {
            this->set_snapshot_folder("/Users/wargrey/Desktop");
            this->set_cmdwin_height(24);
        }

    public:
        void construct(int argc, char* argv[]) override {
            this->push_plane(new GradeManagementPlane());
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
