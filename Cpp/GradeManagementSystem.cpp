#include "digitama/big_bang/game.hpp"

#include "digitama/IMS/menu.hpp"
#include "digitama/IMS/avatar.hpp"
#include "digitama/IMS/model.hpp"

#include "digitama/IMS/view/doorlet.hpp"
#include "digitama/IMS/view/disciplinelet.hpp"
#include "digitama/IMS/view/studentlet.hpp"
#include "digitama/IMS/view/desklet.hpp"

#include <sstream>
#include <vector>
#include <map>

using namespace WarGrey::STEM;
using namespace WarGrey::IMS;

/*************************************************************************************************/
namespace {
    static const size_t DESK_COUNT = 7;
    static const float platform_width = 512.0F;
    static const float platform_height = 80.0F;
    static const double gliding_duration = 0.4;

    enum class GradeTask { Create, Update, Delete, _ };

    /*********************************************************************************************/
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
            this->load_avatars(width, height);

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

        void on_tap(IMatter* m, float local_x, float local_y) override {
            Plane::on_tap(m, local_x, local_y);
            auto cls = dynamic_cast<DoorSprite*>(m);
            auto dis = dynamic_cast<DisciplineSprite*>(m);
            auto stu = dynamic_cast<StudentSprite*>(m);
            auto dsk = dynamic_cast<HexagonalDesklet*>(m);
                
            if (m == this->platform) {
                this->current_discipline = 0U;
                this->reflow_discipline_logos();
            } else if (stu != nullptr) {
                this->on_student_changed(stu->primary_key());
            } else if (cls != nullptr) {
                this->on_class_changed(cls->primary_key(), false);

                if ((this->current_task == MenuTask::BindClass) && (this->current_student > 0U)) {
                    this->glide_to(gliding_duration, this->students[this->current_student], cls, MatterAnchor::RC, MatterAnchor::LC);
                    this->model->bind_student_to_class(this->current_student, this->current_class);
                }
            } else if (dis != nullptr) {
                this->on_discipline_changed(dis->primary_key(), false);
            } else if (dsk != nullptr) {
                if (this->current_student > 0U) {
                    if (this->current_class > 0U) {
                        int idx = dsk->get_seat_by(local_x, local_y);

                        if (idx > 0) {
                            dsk->sit(this->students[this->current_student], idx, gliding_duration);

                            if (this->model->get_student_class(this->current_student) == 0U) {
                                this->model->bind_student_to_class(this->current_student, this->current_class);
                            }

                            this->model->bind_student_to_seat(this->current_student, dsk->get_index(), idx);
                        }
                    } else {
                        this->log_message(FIREBRICK, "请先绑定班级");
                    }
                }

                this->no_selected();
            }
        }

        bool update_tooltip(IMatter* m, float lx, float ly, float gx, float gy) override {
            bool updated = false;
            
            if (m != this->agent) {
                auto entity = dynamic_cast<ISprite*>(m);
                auto desk = dynamic_cast<IDesk*>(m);

                if (entity != nullptr) {
                    this->tooltip->set_text(" %s ", entity->name());
                    updated = true;
                } else if (desk != nullptr) {
                    this->tooltip->set_text(" %d ", desk->get_seat_by(lx, ly));
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
                        case MenuTask::CreateStudent: this->model->create_student_from_user_input(text, size); break;
                        case MenuTask::UpdateStudent: this->model->update_student_from_user_input(text, size); break;
                        case MenuTask::DeleteStudent: this->model->delete_student_as_user_required(text, size); break;
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

            this->avatar->show(this->current_menu_type == MenuType::Student);
        }
        
        void on_menu_task(MenuType self, MenuTask task) override {
            this->current_task = task;

            try {
                switch (task) {
                case MenuTask::Exit: this->mission_complete(); break;
                case MenuTask::CreateClass: this->start_input_text("请按格式输入待创建班级信息(%s): ", ClassEntity::prompt()); break;
                case MenuTask::DeleteClass: this->start_input_text("请输入待删除班级的班号: "); break;
                case MenuTask::CreateDiscipline: this->start_input_text("请按格式输入待创建课程信息(%s): ", DisciplineEntity::prompt()); break;
                case MenuTask::UpdateDiscipline: this->start_input_text("请按格式输入待修改课程信息(%s): ", DisciplineEntity::prompt()); break;
                case MenuTask::DeleteDiscipline: this->start_input_text("请输入待删除课程的代号: "); break;
                case MenuTask::CreateStudent: this->start_input_text("请按格式输入新生信息(%s): ", StudentEntity::prompt()); break;
                case MenuTask::UpdateStudent: this->start_input_text("请按格式输入待修改学生信息(%s): ", StudentEntity::prompt()); break;
                case MenuTask::DeleteStudent: this->start_input_text("请输入待删除学生的学号: "); break;
                case MenuTask::CreateGrade: this->on_grade_task(GradeTask::Create); break;
                case MenuTask::UpdateGrade: this->on_grade_task(GradeTask::Update); break;
                case MenuTask::DeleteGrade: this->on_grade_task(GradeTask::Delete); break;
                case MenuTask::ImportData: this->model->import_from_file(this->gmsin); this->reflow_model_sprites(gliding_duration); break;
                case MenuTask::ExportData: this->model->export_to_file(this->gmsout); this->log_message(GREEN, "done."); break;
                case MenuTask::ClearStudent: this->model->clear_detached_students(); this->log_message(GREEN, "done."); break;
                case MenuTask::ClearGrade: this->model->clear_detached_grades(); this->log_message(GREEN, "done."); break;
                default: /* do nothing */;
                }
            } catch (const std::exception& e) {
                this->log_message(CRIMSON, "%s", e.what());
            }
        }

        void on_grade_task(GradeTask task) {
            this->current_grade_task = task;
        }

        void ensure_student_and_discipline_for_grade_task() {
            if ((this->current_student == 0U) || (this->current_discipline == 0U)) {
                this->log_message(YELLOW, "请先选定学生和课程");
            }
        }

    public:
        void on_class_created(uint64_t pk, shared_class_t entity, bool in_batching) override {
            this->doors[pk] = this->insert(new DoorSprite(pk));
            this->doors[pk]->resize_by_height(platform_height * 0.72F);

            if (!in_batching) {
                this->reflow_class_logos(gliding_duration);
            }
        }

        void on_class_deleted(uint64_t pk, shared_class_t entity, bool in_batching) override {
            this->remove(this->doors[pk]);
            this->doors.erase(pk);

            if (this->current_class == pk) {
                if (!this->doors.empty()) {
                    this->on_class_changed(this->doors.begin()->first, in_batching);
                }
            }

            if (!in_batching) {
                this->reflow_class_logos(gliding_duration);
            }
        }

        void on_class_changed(uint64_t clsId, bool in_batching) {
            if (this->current_class > 0U) {
                this->doors[this->current_class]->close();
            }

            this->current_class = clsId;
            this->doors[current_class]->open();
            this->clsLabel->set_text(MatterAnchor::RT, "%u", clsId);
            this->stuLabel->set_text(MatterAnchor::RB, "");

            if (!in_batching) {
                this->reflow_students(gliding_duration);
            }
        }

        void on_discipline_created(uint64_t pk, shared_discipline_t entity, bool in_batching) override {
            this->disciplines[pk] = this->insert(new DisciplineSprite(pk, entity->discipline_type()));
            this->disciplines[pk]->resize_by_height(platform_height * 0.90F);

            if (!in_batching) {
                this->reflow_discipline_logos(gliding_duration);
            }
        }

        void on_discipline_deleted(uint64_t pk, shared_discipline_t entity, bool in_batching) override {
            this->remove(this->disciplines[pk]);
            this->disciplines.erase(pk);

            if (!in_batching) {
                this->reflow_discipline_logos(gliding_duration);
            }
        }

        void on_discipline_changed(uint64_t pk, bool in_batching) {
            this->current_discipline = pk;

            if (!in_batching) {
                this->reflow_discipline_logos(gliding_duration);
            }
        }

        void on_student_created(uint64_t pk, shared_student_t entity, bool in_batching) override {
            this->students[pk] = this->insert(new StudentSprite(pk, entity->get_nickname(), entity->get_avatar(), entity->get_age()));

            if (!in_batching) {
                this->reflow_students(gliding_duration);
            }
        }

        void on_student_changed(uint64_t sNo) {
            this->current_student = sNo;
            this->stuLabel->set_text(MatterAnchor::RB, "%s", this->students[sNo]->name());
        }

        void on_student_deleted(uint64_t pk, shared_student_t entity, bool in_batching) override {
            this->remove(this->students[pk]);
            this->students.erase(pk);

            if (!in_batching) {
                this->reflow_students(gliding_duration);
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
            this->tooltip->set_background_color(GHOSTWHITE);
        }

        float reflow_gui_elements(float width, float height) {
            float sidebar_pos, agent_width;

            this->move_to(this->title, this->agent, MatterAnchor::RB, MatterAnchor::LB);

            this->agent->feed_extent(0.0F, 0.0F, &agent_width, nullptr);
            this->feed_matter_location(this->title, &sidebar_pos, nullptr, MatterAnchor::RC);
            sidebar_pos += agent_width * 0.618F;
            
            this->move_to(this->side_border, sidebar_pos, height, MatterAnchor::CB);

            for (auto menu : this->menus) {
                this->move_to(menu.second, this->agent, MatterAnchor::LB, MatterAnchor::LT, 4.0F, 4.0F);
            }

            this->move_to(this->avatar, 0.0F, height, MatterAnchor::LB, 1.0F, -1.0F);
            
            return sidebar_pos;
        }

    private:
        void load_classroom(float width, float height) {
            this->platform = this->insert(new Rectanglet(platform_width, platform_height, STEELBLUE));
            this->clsLabel = this->insert(new Labellet(GameFont::monospace(FontSize::xx_large), GHOSTWHITE, "%u", this->current_class));
            this->stuLabel = this->insert(new Labellet(GameFont::serif(), GHOSTWHITE, "%s", "[学生]"));
            
            for (size_t idx = 1; idx <= DESK_COUNT; idx ++) {
                this->desks.push_back(this->insert(new HexagonalDesklet(idx, 90.0F, DARKORANGE)));
            }
        }

        void reflow_classroom(float width, float height, float sidebar_pos) {
            this->move_to(this->platform, (width - sidebar_pos) * 0.50F + sidebar_pos, height * 0.95F, MatterAnchor::CC);
            this->move_to(this->clsLabel, this->platform, MatterAnchor::RT, MatterAnchor::RT, -4.0F, +4.0F);
            this->move_to(this->stuLabel, this->platform, MatterAnchor::RB, MatterAnchor::RB, -4.0F, -4.0F);

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
            this->reflow_students(duration);
        }

        void reflow_class_logos(double duration = gliding_duration) {
            if (!this->doors.empty()) {
                float cls_x, cls_y, grid_height;
         
                this->feed_matter_location(this->side_border, &cls_x, &cls_y, MatterAnchor::CB);
                this->doors.begin()->second->feed_extent(0.0F, 0.0F, nullptr, &grid_height);
                grid_height *= 1.2F;

                for (auto cls = this->doors.rbegin(); cls != this->doors.rend(); cls ++) {
                    this->glide_to(duration, cls->second, cls_x, cls_y, MatterAnchor::CB, 0.0F, -1.0F);
                    cls_y -= grid_height;
                }
            }
        }

        void reflow_discipline_logos(double duration = gliding_duration) {
            if (!this->disciplines.empty()) {
                float dis_x0, dis_x, dis_y, grid_width;
                float gap = 4.0F;
         
                this->feed_matter_location(this->platform, &dis_x, &dis_y, MatterAnchor::LC);
                this->disciplines.begin()->second->feed_extent(0.0F, 0.0F, &grid_width);
                dis_x0 = dis_x = dis_x + gap;
                grid_width += gap;

                for (auto dis : this->disciplines) {
                    uint64_t disCode = dis.second->primary_key();

                    if ((disCode == this->current_discipline) || (this->current_discipline == 0U)) {
                        dis.second->show(true);
                        this->glide_to(duration, dis.second, dis_x, dis_y, MatterAnchor::LC);
                        dis_x += grid_width;
                    } else {
                        this->glide_to(duration, dis.second, dis_x0, dis_y, MatterAnchor::LC);
                    }
                }
            }
        }

        void reflow_students(double duration = gliding_duration) {
            if (!this->students.empty()) {
                float nocls_stu_x, nocls_stu_y, grid_height;
                uint64_t desk_idx, seat_idx;
                float gap = 4.0F;
         
                this->feed_matter_location(this->side_border, &nocls_stu_x, &nocls_stu_y, MatterAnchor::LB);
                this->students.begin()->second->feed_extent(0.0F, 0.0F, nullptr, &grid_height);
                nocls_stu_x *= 0.80F;
                grid_height += gap;

                for (auto stu : this->students) {
                    uint64_t stuClsId = this->model->get_student_class(stu.second->primary_key());

                    stu.second->show((stuClsId == this->current_class) || (stuClsId == 0U));

                    if (stuClsId == 0U) {
                        this->glide_to(duration, stu.second, nocls_stu_x, nocls_stu_y, MatterAnchor::RB);
                        nocls_stu_y -= grid_height;
                    } else {
                        if (stu.second->visible()) {
                            this->model->feed_student_seat(stu.first, &desk_idx, &seat_idx);

                            if ((desk_idx > 0U) && (seat_idx > 0U)) {
                                this->glide_to(duration, stu.second, this->doors[stuClsId], MatterAnchor::RC, MatterAnchor::LC);
                                this->desks[desk_idx - 1]->sit(stu.second, seat_idx, duration);
                            } else {
                                this->glide_to(duration, stu.second, this->doors[stuClsId], MatterAnchor::RC, MatterAnchor::LC);
                            }
                        } else {
                            this->move_to(stu.second, this->doors[stuClsId], MatterAnchor::RC, MatterAnchor::LC);
                        }
                    }
                }
            }
        }

    protected:
        void on_motion_complete(IMatter* m, float x, float y, double xspd, double yspd) override {
            auto dis = dynamic_cast<DisciplineSprite*>(m);

            if (dis != nullptr) {
                if (this->current_discipline > 0U) {
                    if (dis->primary_key() != this->current_discipline) {
                        dis->show(false);
                    }
                }
            }
        }

    private:
        void load_menus(float width, float height) {
            this->menus[MenuType::TopLevel] = this->insert(new Continent(new TopLevelMenu()));
            this->menus[MenuType::Class] = this->insert(new Continent(new ClassMenu()));
            this->menus[MenuType::Discipline] = this->insert(new Continent(new DisciplineMenu()));
            this->menus[MenuType::Student] = this->insert(new Continent(new StudentMenu()));
            this->menus[MenuType::Grade] = this->insert(new Continent(new GradeMenu()));

            for (auto menu : this->menus) {
                if (this->current_menu_type != menu.first) {
                    menu.second->show(false);
                }

                menu.second->camouflage(true);
            }
        }

        void load_avatars(float width, float height) {
            this->avatar = this->insert(new Continent(new AvatarPlane("学生形象")));

            this->avatar->set_border_color(ROYALBLUE);
            this->avatar->set_background_color(DIMGRAY, 0.64);

            this->avatar->show(false);
            this->avatar->camouflage(true);
        }

    private:
        Rectanglet* platform;
        std::vector<HexagonalDesklet*> desks;
    
    private:
        Labellet* title;
        Labellet* tooltip;
        Labellet* clsLabel;
        Labellet* stuLabel;
        Linelet* side_border;
        std::map<uint64_t, DoorSprite*> doors;
        std::map<uint64_t, DisciplineSprite*> disciplines;
        std::map<uint64_t, StudentSprite*> students;
        std::map<MenuType, Continent*> menus;
        Continent* avatar;
        Linkmon* agent;

    private:
        MenuType current_menu_type = MenuType::TopLevel;
        MenuTask current_task = MenuTask::ImportData;
        uint64_t current_class = 0U;
        uint64_t current_discipline = 0U;
        uint64_t current_student = 0U;
        uint64_t current_timestamp = 0U;
        GradeManagementSystemModel* model;

    private:
        GradeTask current_grade_task = GradeTask::_;

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

            GameFont::fontsize(20);

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
