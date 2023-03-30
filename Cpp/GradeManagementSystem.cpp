#include "digitama/big_bang/game.hpp"
#include "digitama/big_bang/bang.hpp"

#include "digitama/big_bang/mascot/character.hpp"

using namespace WarGrey::STEM;

/*************************************************************************************************/
namespace {
    static const size_t DESK_COUNT = 7;

    class GradeManagementPlane : public Plane {
    public:
        GradeManagementPlane() : Plane("成绩管理系统") { }

    public:
        void load(float width, float height) override {
            this->platform = this->insert(new Rectanglet(400.0F, 64.0F, STEELBLUE));
            this->side_border = this->insert(new VLinelet(height, GRAY));
            
            for (size_t idx = 0; idx < DESK_COUNT; idx ++) {
                this->desks.push_back(this->insert(new RegularPolygonlet(6, 90.0F, DARKORANGE)));
            }

            this->agent = this->insert(new Linkmon());
            this->title = this->insert(new Labellet(GameFont::Title(), BLACK, "%s", this->name()));
            
            this->bracers.push_back(this->insert(new Estelle()));
            this->bracers.push_back(this->insert(new Joshua()));
            this->bracers.push_back(this->insert(new Agate()));

            for (auto name : TrailKid::list_names()) {
                this->kids.push_back(this->insert(new TrailKid(name)));
            }

            for (auto name : TrailStudent::list_names()) {
                this->students.push_back(this->insert(new TrailStudent(name)));
            }

            for (auto name : Citizen::list_special_names()) {
                this->specials.push_back(this->insert(Citizen::create_special(name)));
            }

            this->tooltip = this->insert(make_label_for_tooltip(GameFont::Tooltip()));

            this->set_sentry_sprite(this->agent);
            this->set_tooltip_matter(this->tooltip);
            this->agent->scale(-1.0F, 1.0F);
        }

        void reflow(float width, float height) override {
            float sidebar_pos;

            this->move_to(this->title, this->agent, MatterAnchor::RB, MatterAnchor::LB);

            this->feed_matter_location(this->title, &sidebar_pos, nullptr, MatterAnchor::RC);
            sidebar_pos += float(generic_font_size(FontSize::medium));
            
            this->move_to(this->side_border, sidebar_pos, height, MatterAnchor::CB);
            this->move_to(this->desks[0], (width - sidebar_pos) * 0.25F + sidebar_pos, height * 0.64F, MatterAnchor::CC);
            this->move_to(this->desks[1], (width - sidebar_pos) * 0.50F + sidebar_pos, height * 0.75F, MatterAnchor::CC);
            this->move_to(this->desks[2], (width - sidebar_pos) * 0.75F + sidebar_pos, height * 0.64F, MatterAnchor::CC);
            this->move_to(this->desks[3], (width - sidebar_pos) * 0.25F + sidebar_pos, height * 0.32F, MatterAnchor::CC);
            this->move_to(this->desks[4], (width - sidebar_pos) * 0.50F + sidebar_pos, height * 0.45F, MatterAnchor::CC);
            this->move_to(this->desks[5], (width - sidebar_pos) * 0.75F + sidebar_pos, height * 0.32F, MatterAnchor::CC);
            this->move_to(this->desks[6], (width - sidebar_pos) * 0.50F + sidebar_pos, height * 0.15F, MatterAnchor::CC);

            this->move_to(this->platform, (width - sidebar_pos) * 0.50F + sidebar_pos, height * 0.95F, MatterAnchor::CC);

            for (int idx = 0; idx < this->kids.size(); idx ++) {
                if (idx == 0) {
                    this->move_to(this->kids[idx], this->agent, MatterAnchor::CB, MatterAnchor::CT, 0.0F, 20.0F);
                } else {
                    this->move_to(this->kids[idx], this->kids[idx - 1], MatterAnchor::CB, MatterAnchor::CT, 0.0F, 10.0F);
                }
            }

            for (int idx = 0; idx < this->students.size(); idx ++) {
                if (idx == 0) {
                    this->move_to(this->students[idx], this->kids[idx], MatterAnchor::RB, MatterAnchor::LB, 40.0F);
                } else {
                    this->move_to(this->students[idx], this->students[idx - 1], MatterAnchor::CB, MatterAnchor::CT, 0.0F, 10.0F);
                }
            }

            for (int idx = 0; idx < this->specials.size(); idx ++) {
                if (idx == 0) {
                    this->move_to(this->specials[idx], this->students[idx], MatterAnchor::RB, MatterAnchor::LB, 80.0F);
                } else {
                    this->move_to(this->specials[idx], this->specials[idx - 1], MatterAnchor::CB, MatterAnchor::CT, 0.0F, 10.0F);
                }
            }

            for (int idx = 0; idx < this->bracers.size(); idx ++) {
                if (idx == 0) {
                    this->move_to(this->bracers[idx], this->specials[idx], MatterAnchor::RB, MatterAnchor::LB, 80.0F);
                } else {
                    this->move_to(this->bracers[idx], this->bracers[idx - 1], MatterAnchor::CB, MatterAnchor::CT, 0.0F, 10.0F);
                }
            }
        }

    public:
        bool can_select(IMatter* m) override {
            return true;
        }

        void after_select(IMatter* m, bool yes) override {
            if (yes) {
                auto citizen = dynamic_cast<Citizen*>(m);
                
                if (citizen != nullptr) {
                    auto bracer = dynamic_cast<Bracer*>(m);
                
                    if (bracer != nullptr) {
                        if (is_ctrl_pressed()) {
                            this->bracer_winpose_demo(bracer);
                        } else if (is_shift_pressed()) {
                            bracer->switch_mode(BracerMode::Run);
                            this->citizen_motion_demo(bracer);
                        } else {
                            bracer->switch_mode(BracerMode::Walk);
                            this->citizen_motion_demo(bracer);
                        }
                    } else {
                        this->citizen_motion_demo(citizen);
                    }
                }
            }
        }

        void on_tap_selected(IMatter* m, float x, float y) override {
            this->after_select(m, true);
        }

        bool update_tooltip(IMatter* m, float lx, float ly, float gx, float gy) override {
            bool updated = false;
            auto citizen = dynamic_cast<Citizen*>(m);

            if (citizen != nullptr) {
                this->tooltip->set_text(" %s ", citizen->name());
                updated = true;
            }

            return updated;
        }

    private:
        void citizen_motion_demo(Citizen* citizen) {
            float distance = 64.0F;
            float duration = 1.0F;

            if (citizen->motion_stopped()) {
                this->glide(duration, citizen, distance, 0.0F);
                this->glide(duration, citizen, 0.0F, distance);
                this->glide(duration, citizen, -distance, 0.0F);
                this->glide(duration, citizen, 0.0F, -distance);
                this->glide(duration, citizen, distance, distance);
                this->glide(duration, citizen, -distance, distance);
                this->glide(duration, citizen, -distance, -distance);
                this->glide(duration, citizen, distance, -distance);
            }
        }

        void bracer_winpose_demo(Bracer* bracer) {
            if (bracer->motion_stopped()) {
                bracer->switch_mode(BracerMode::Win, 1);
            }
        }

    private:
        Rectanglet* platform;
        std::vector<IGraphlet*> desks;
    
    private:
        Labellet* title;
        Labellet* tooltip;
        Linelet* side_border;
        std::vector<Bracer*> bracers;
        std::vector<Citizen*> kids;
        std::vector<Citizen*> students;
        std::vector<Citizen*> specials;
        Linkmon* agent;
    };

    class GradeManagementSystem : public Cosmos {
    public:
        GradeManagementSystem() : Cosmos(60) {}

    public:
        void construct(int argc, char* argv[]) override {
            enter_digimon_zone(argv[0]);    

            this->set_snapshot_folder("/Users/wargrey/Desktop");
            this->set_cmdwin_height(24);
            this->set_window_size(1400, 0);

            this->push_plane(new GradeManagementPlane());
        }
    };
}

/*************************************************************************************************/
int main(int argc, char* args[]) {
    GradeManagementSystem universe;

    universe.construct(argc, args);
    universe.big_bang();

    return 0;
}
