#include "digitama/big_bang/game.hpp"

#include "digitama/IMS/menu.hpp"
#include "digitama/IMS/model.hpp"

#include "digitama/IMS/view/class.hpp"
#include "digitama/IMS/view/discipline.hpp"

#include <vector>
#include <map>

using namespace WarGrey::STEM;
using namespace WarGrey::IMS;

/*************************************************************************************************/
namespace {
    static const size_t DESK_COUNT = 7;
    static const float platform_width = 512.0F;
    static const float platform_height = 64.0F;
    static const double gliding_duration = 0.5;

    class GradeManagementPlane : public Plane, public IMenuEventListener, public IModelListener {
    public:
        GradeManagementPlane(std::string gmsin, std::string gmsout) : Plane("成绩管理系统") {
            this->model = new GradeManagementSystemModel(this);
            this->gmsin = gmsin;
            this->gmsout = (gmsout.empty()) ? this->gmsin : gmsout;
        }

        virtual ~GradeManagementPlane() {
            delete this->model;
        }

    public:
        void load(float width, float height) override {
            this->load_gui_elements(width, height);
            this->load_menus(width, height);
            this->load_classroom(width, height);

            this->on_menu_task(MenuType::TopLevel, MenuTask::ImportData);
        }

        void reflow(float width, float height) override {
            float sidebar_pos = this->reflow_gui_elements(width, height);
            
            this->reflow_classroom(width, height, sidebar_pos);
            this->reflow_class_logos();
            this->reflow_discipline_logos();
        }

    public:
        bool can_select(IMatter* m) override {
            return true;
        }

        void after_select(IMatter* m, bool yes) override {
            if (yes) {
            }
        }

        void on_tap_selected(IMatter* m, float x, float y) override {
            this->after_select(m, true);
        }

        bool update_tooltip(IMatter* m, float lx, float ly, float gx, float gy) override {
            bool updated = false;

            if (m != this->agent) {
                auto entity = dynamic_cast<ISprite*>(m);

                if (entity != nullptr) {
                    this->tooltip->set_text(" %s ", entity->name());
                    updated = true;
                }
            }

            return updated;
        }

    public:
        void on_char(char key, uint16_t modifiers, uint8_t repeats, bool pressed) override {
            if (!pressed) {
                if (isdigit(key)) {
                    int idx = key - '0';
                    IMenu* self = this->menus[this->current_menu_type]->unsafe_plane<IMenu>();

                    if ((idx >= 0) && (idx < self->count())) {
                        self->on_menu_char(this, this->current_menu_type, key);
                    }
                }
            }
        }

        void on_text(const char* text, size_t size, bool entire) override {
            if (entire && (size > 0)) {
                try {
                    switch (this->current_task) {
                        case MenuTask::CreateClass: this->model->create_class_from_user_input(text, size); break;
                        case MenuTask::DeleteClass: this->model->delete_class_as_user_required(text, size); break;
                        case MenuTask::CreateDiscipline: this->model->create_discipline_from_user_input(text, size); break;
                        case MenuTask::UpdateDiscipline: this->model->update_discipline_from_user_input(text, size); break;
                        case MenuTask::DeleteDiscipline: this->model->delete_discipline_as_user_required(text, size); break;
                        default: /* do nothing */;
                    }
                } catch (const std::exception& e) {
                    this->log_message(CRIMSON, "%s", e.what());
                }
            }
        }

    public:
        void on_menu_switch(MenuType self, MenuType to) override {
            if (this->menus.find(to) != this->menus.end()) {
                this->menus[self]->unsafe_plane<IMenu>()->select_menu('\0');
                this->menus[self]->show(false);
                this->menus[to]->show(true);
                this->current_menu_type = to;
            }
        }
        
        void on_menu_task(MenuType self, MenuTask task) override {
            this->current_task = task;

            try {
                switch (task) {
                case MenuTask::Exit: this->mission_complete(); break;
                case MenuTask::CreateClass: this->start_input_text("请按格式输入待创建班级信息(%s): ", ClassEntity::prompt()); break;
                case MenuTask::DeleteClass: this->start_input_text("请输入待删除班级代号: "); break;
                case MenuTask::CreateDiscipline: this->start_input_text("请按格式输入待创建课程信息(%s): ", DisciplineEntity::prompt()); break;
                case MenuTask::UpdateDiscipline: this->start_input_text("请按格式输入待修改课程信息(%s)【代号不可能修改，“_”提示保留字段】: ", DisciplineEntity::prompt()); break;
                case MenuTask::DeleteDiscipline: this->start_input_text("请输入待删除课程代号: "); break;
                case MenuTask::ImportData: this->model->import_from_file(this->gmsin); this->reflow_model_sprites(gliding_duration); break;
                case MenuTask::ExportData: this->model->export_to_file(this->gmsout); this->log_message(GREEN, "done."); break;
                default: /* do nothing */;
                }
            } catch (const std::exception& e) {
                this->log_message(CRIMSON, "%s", e.what());
            }
        }

    public:
        void on_class_created(uint64_t id, shared_class_t entity, bool in_batching) override {
            this->classes[id] = this->insert(new ClassSprite(id));
            this->classes[id]->resize_by_height(platform_height * 0.618F);

            if (!in_batching) {
                this->reflow_class_logos(gliding_duration);
            }
        }

        void on_class_deleted(uint64_t id, shared_class_t entity, bool in_batching) override {
            this->remove(this->classes[id]);
            this->classes.erase(id);

            if (!in_batching) {
                this->reflow_class_logos(gliding_duration);
            }
        }

        void on_discipline_created(uint64_t id, shared_discipline_t entity, bool in_batching) override {
            this->disciplines[id] = this->insert(new DisciplineSprite(id, entity->discipline_type()));
            this->disciplines[id]->resize_by_height(platform_height * 0.85F);

            if (!in_batching) {
                this->reflow_discipline_logos(gliding_duration);
            }
        }

        void on_discipline_deleted(uint64_t id, shared_discipline_t entity, bool in_batching) override {
            this->remove(this->disciplines[id]);
            this->disciplines.erase(id);

            if (!in_batching) {
                this->reflow_discipline_logos(gliding_duration);
            }
        }

    private:
        void load_gui_elements(float width, float height) {
            this->agent = this->insert(new Linkmon());
            this->title = this->insert(new Labellet(GameFont::Title(), BLACK, "%s", this->name()));
            this->side_border = this->insert(new VLinelet(height, GRAY));
            this->tooltip = this->insert(make_label_for_tooltip(GameFont::Tooltip()));
            
            this->set_sentry_sprite(this->agent);
            this->set_tooltip_matter(this->tooltip);
            this->agent->scale(-1.0F, 1.0F);
        }

        float reflow_gui_elements(float width, float height) {
            float sidebar_pos, agent_width;

            this->move_to(this->title, this->agent, MatterAnchor::RB, MatterAnchor::LB);

            this->agent->feed_extent(0.0F, 0.0F, &agent_width, nullptr);
            this->feed_matter_location(this->title, &sidebar_pos, nullptr, MatterAnchor::RC);
            sidebar_pos += agent_width;
            
            this->move_to(this->side_border, sidebar_pos, height, MatterAnchor::CB);

            for (auto menu : this->menus) {
                this->move_to(menu.second, this->agent, MatterAnchor::LB, MatterAnchor::LT, 4.0F, 4.0F);
            }
            
            return sidebar_pos;
        }

    private:
        void load_classroom(float width, float height) {
            this->platform = this->insert(new Rectanglet(platform_width, platform_height, STEELBLUE));
            
            for (size_t idx = 0; idx < DESK_COUNT; idx ++) {
                this->desks.push_back(this->insert(new RegularPolygonlet(6, 90.0F, DARKORANGE)));
            }
        }

        void reflow_classroom(float width, float height, float sidebar_pos) {
            this->move_to(this->platform, (width - sidebar_pos) * 0.50F + sidebar_pos, height * 0.95F, MatterAnchor::CC);

            this->move_to(this->desks[0], (width - sidebar_pos) * 0.25F + sidebar_pos, height * 0.64F, MatterAnchor::CC);
            this->move_to(this->desks[1], (width - sidebar_pos) * 0.50F + sidebar_pos, height * 0.75F, MatterAnchor::CC);
            this->move_to(this->desks[2], (width - sidebar_pos) * 0.75F + sidebar_pos, height * 0.64F, MatterAnchor::CC);
            this->move_to(this->desks[3], (width - sidebar_pos) * 0.25F + sidebar_pos, height * 0.32F, MatterAnchor::CC);
            this->move_to(this->desks[4], (width - sidebar_pos) * 0.50F + sidebar_pos, height * 0.45F, MatterAnchor::CC);
            this->move_to(this->desks[5], (width - sidebar_pos) * 0.75F + sidebar_pos, height * 0.32F, MatterAnchor::CC);
            this->move_to(this->desks[6], (width - sidebar_pos) * 0.50F + sidebar_pos, height * 0.15F, MatterAnchor::CC);

            this->reflow_model_sprites(0.0);
        }

        void reflow_model_sprites(double duration = gliding_duration) {
            this->reflow_class_logos(duration);
            this->reflow_discipline_logos(duration);
        }

        void reflow_class_logos(double duration = gliding_duration) {
            if (!this->classes.empty()) {
                float cls_x, cls_y, grid_width;
         
                this->feed_matter_location(this->platform, &cls_x, &cls_y, MatterAnchor::LB);
                this->classes.begin()->second->feed_extent(0.0F, 0.0F, &grid_width);
                grid_width += 4.0F;

                for (auto cls : this->classes) {
                    this->glide_to(duration, cls.second, cls_x, cls_y, MatterAnchor::LB);
                    cls_x += grid_width;
                }
            }
        }

        void reflow_discipline_logos(double duration = gliding_duration) {
            if (!this->disciplines.empty()) {
                float dis_x, dis_y, grid_width;
         
                this->feed_matter_location(this->side_border, &dis_x, &dis_y, MatterAnchor::RB);
                this->disciplines.begin()->second->feed_extent(0.0F, 0.0F, &grid_width);
                grid_width += 4.0F;
                dis_x += 2.0F;
                dis_y -= 2.0F;

                for (auto dis : this->disciplines) {
                    this->glide_to(duration, dis.second, dis_x, dis_y, MatterAnchor::LB);
                    dis_x += grid_width;
                }
            }
        }

    private:
        void load_menus(float width, float height) {
            this->menus[MenuType::TopLevel] = this->insert(new Continent(new TopLevelMenu()));
            this->menus[MenuType::Class] = this->insert(new Continent(new ClassMenu()));
            this->menus[MenuType::Discipline] = this->insert(new Continent(new DisciplineMenu()));

            for (auto menu : this->menus) {
                if (this->current_menu_type != menu.first) {
                    menu.second->show(false);
                }

                menu.second->camouflage(true);
            }
        }

    private:
        Rectanglet* platform;
        std::vector<IGraphlet*> desks;
    
    private:
        Labellet* title;
        Labellet* tooltip;
        Linelet* side_border;
        std::map<uint64_t, ClassSprite*> classes;
        std::map<uint64_t, DisciplineSprite*> disciplines;
        std::map<MenuType, Continent*> menus;
        Linkmon* agent;

    private:
        MenuType current_menu_type = MenuType::TopLevel;
        MenuTask current_task = MenuTask::ImportData;
        uint64_t current_class = 0;
        uint64_t current_discipline = 0;
        GradeManagementSystemModel* model;

    private:
        std::string gmsin;
        std::string gmsout;
    };

    /*********************************************************************************************/
    enum GMSCmdOpt { GMSIn, GMSOut, _ };

    class GradeManagementSystem : public Cosmos {
    public:
        GradeManagementSystem() : Cosmos(60) {}

    public:
        void construct(int argc, char* argv[]) override {
            enter_digimon_zone(argv[0]);

            this->parse_commandline_argument(argc, argv);
            this->set_snapshot_folder("/Users/wargrey/Desktop");
            this->set_cmdwin_height(24);

            this->push_plane(new GradeManagementPlane(this->gmsin, this->gmsout));
        }

    private:
        void parse_commandline_argument(int argc, char* argv[]) {
            GMSCmdOpt opt = GMSCmdOpt::_;

            for (int idx = 1; idx < argc; idx ++) {
                switch (opt) {
                case GMSCmdOpt::GMSIn: {
                    this->gmsin = argv[idx];
                    opt = GMSCmdOpt::_;
                 }; break;
                case GMSCmdOpt::GMSOut: {
                    this->gmsin = argv[idx];
                    opt = GMSCmdOpt::_;
                }; break;
                default: {
                    if ((strcmp(argv[idx], "-i") == 0) || (strcmp(argv[idx], "--in") == 0)) {
                        opt = GMSCmdOpt::GMSIn;
                    } else if ((strcmp(argv[idx], "-o") == 0) || (strcmp(argv[idx], "--out") == 0)) {
                        opt = GMSCmdOpt::GMSOut;
                    }
                }; break;
                }
            }
        }

    private:
        std::string gmsin;
        std::string gmsout;
    };
}

/*************************************************************************************************/
int main(int argc, char* args[]) {
    GradeManagementSystem universe;

    universe.construct(argc, args);
    universe.big_bang();

    return 0;
}
