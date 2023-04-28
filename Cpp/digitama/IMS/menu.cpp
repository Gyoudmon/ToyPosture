#include "menu.hpp"

using namespace WarGrey::STEM;
using namespace WarGrey::IMS;

/*************************************************************************************************/
static const char MENU_RETURN_KEY = '0';

/*************************************************************************************************/
void WarGrey::IMS::IMenu::load(float width, float height) {
    auto font = GameFont::monospace(FontSize::large);
    this->menuitems = this->prepare_menu_items();

    for (auto menuitem : this->menuitems) {
        const char* caption = menuitem.second.c_str();
        char key = menuitem.first;

        this->menulabels[key] = this->insert(new Labellet(font, BLACK, "%c. %s", key, caption));
    }
}

void WarGrey::IMS::IMenu::reflow(float width, float height) {
    for (size_t idx = 1; idx < this->menuitems.size(); idx ++) {
        Labellet* prev_label = this->menulabels[this->menuitems[idx - 1].first];
        Labellet* self_label = this->menulabels[this->menuitems[idx].first];
        
        this->move_to(self_label, prev_label, MatterAnchor::LB, MatterAnchor::LT, 0.0F, 2.0F);
    }
}

void WarGrey::IMS::IMenu::on_menu_char(IMenuEventListener* master, MenuType self, char key) {
    this->select_menu(key);
    this->on_menu_key(master, self, key);
}

void WarGrey::IMS::IMenu::select_menu(char key) {
    if (this->menulabels.find(this->current_menuitem) != this->menulabels.end()) {
        this->menulabels[this->current_menuitem]->set_text_color(BLACK);
    }

    if (this->menulabels.find(key) != this->menulabels.end()) {
        this->menulabels[key]->set_text_color(ROYALBLUE);
    }
    
    this->current_menuitem = key;    
}

/*************************************************************************************************/
static const char TLM_IMPT_KEY = '1';
static const char TLM_EXPT_KEY = '2';
static const char TLM_CLSS_KEY = '3';
static const char TLM_CRSE_KEY = '4';
static const char TLM_STDT_KEY = '5';
static const char TLM_GRDE_KEY = '6';

std::vector<std::pair<char, std::string>> WarGrey::IMS::TopLevelMenu::prepare_menu_items() {
    return {
        { MENU_RETURN_KEY, "退出系统"},
        { TLM_IMPT_KEY, "加载数据"},
        { TLM_EXPT_KEY, "保存数据"},
        { TLM_CLSS_KEY, "班级管理"},
        { TLM_CRSE_KEY, "课程管理"},
        { TLM_STDT_KEY, "学生管理"},
        { TLM_GRDE_KEY, "成绩管理"}
    };
}

void WarGrey::IMS::TopLevelMenu::on_menu_key(IMenuEventListener* master, MenuType self, char key) {
    switch (key) {
    case TLM_CLSS_KEY: master->on_menu_switch(self, MenuType::Class); break;
    case TLM_CRSE_KEY: master->on_menu_switch(self, MenuType::Discipline); break;
    case TLM_STDT_KEY: master->on_menu_switch(self, MenuType::Student); break;
    case TLM_GRDE_KEY: master->on_menu_switch(self, MenuType::Grade); break;
    case TLM_IMPT_KEY: master->on_menu_task(self, MenuTask::ImportData); break;
    case TLM_EXPT_KEY: master->on_menu_task(self, MenuTask::ExportData); break;
    case MENU_RETURN_KEY: master->on_menu_task(self, MenuTask::Exit); break;
    default: /* do nothing */;
    }
}

/*************************************************************************************************/
static const char CLASS_CREATE_KEY = '1';
static const char CLASS_DELETE_KEY = '2';

std::vector<std::pair<char, std::string>> WarGrey::IMS::ClassMenu::prepare_menu_items() {
    return {
        { MENU_RETURN_KEY, "返回上层"},
        { CLASS_CREATE_KEY, "创建班级"},
        { CLASS_DELETE_KEY, "删除班级"}
    };
}

void WarGrey::IMS::ClassMenu::on_menu_key(IMenuEventListener* master, MenuType self, char key) {
    switch (key) {
    case CLASS_CREATE_KEY: master->on_menu_task(self, MenuTask::CreateClass); break;
    case CLASS_DELETE_KEY: master->on_menu_task(self, MenuTask::DeleteClass); break;
    case MENU_RETURN_KEY: master->on_menu_switch(self, MenuType::TopLevel); break;
    default: /* do nothing */;
    }
}

/*************************************************************************************************/
static const char DSCPLN_CREATE_KEY = '1';
static const char DSCPLN_UPDATE_KEY = '2';
static const char DSCPLN_DELETE_KEY = '3';

std::vector<std::pair<char, std::string>> WarGrey::IMS::DisciplineMenu::prepare_menu_items() {
    return {
        { MENU_RETURN_KEY, "返回上层"},
        { DSCPLN_CREATE_KEY, "创建课程"},
        { DSCPLN_UPDATE_KEY, "更新课程"},
        { DSCPLN_DELETE_KEY, "删除课程"}
    };
}

void WarGrey::IMS::DisciplineMenu::on_menu_key(IMenuEventListener* master, MenuType self, char key) {
    switch (key) {
    case DSCPLN_CREATE_KEY: master->on_menu_task(self, MenuTask::CreateDiscipline); break;
    case DSCPLN_UPDATE_KEY: master->on_menu_task(self, MenuTask::UpdateDiscipline); break;
    case DSCPLN_DELETE_KEY: master->on_menu_task(self, MenuTask::DeleteDiscipline); break;
    case MENU_RETURN_KEY: master->on_menu_switch(self, MenuType::TopLevel); break;
    default: /* do nothing */;
    }
}

/*************************************************************************************************/
static const char STDT_CREATE_KEY = '1';
static const char STDT_UPDATE_KEY = '2';
static const char STDT_DELETE_KEY = '3';
static const char STDT_CLASS_KEY = '4';
static const char STDT_CLEAR_KEY = '5';

std::vector<std::pair<char, std::string>> WarGrey::IMS::StudentMenu::prepare_menu_items() {
    return {
        { MENU_RETURN_KEY, "返回上层"},
        { STDT_CREATE_KEY, "添加新生信息"},
        { STDT_UPDATE_KEY, "更新学生信息"},
        { STDT_DELETE_KEY, "删除学生信息"},
        { STDT_CLASS_KEY,  "绑定班级"},
        { STDT_CLEAR_KEY,  "清理游离学生"}
    };
}

void WarGrey::IMS::StudentMenu::on_menu_key(IMenuEventListener* master, MenuType self, char key) {
    switch (key) {
    case STDT_CREATE_KEY: master->on_menu_task(self, MenuTask::CreateStudent); break;
    case STDT_UPDATE_KEY: master->on_menu_task(self, MenuTask::UpdateStudent); break;
    case STDT_DELETE_KEY: master->on_menu_task(self, MenuTask::DeleteStudent); break;
    case STDT_CLASS_KEY: master->on_menu_task(self, MenuTask::BindClass); break;
    case STDT_CLEAR_KEY: master->on_menu_task(self, MenuTask::ClearStudent); break;
    case MENU_RETURN_KEY: master->on_menu_switch(self, MenuType::TopLevel); break;
    default: /* do nothing */;
    }
}

/*************************************************************************************************/
static const char GRADE_CREATE_KEY = '1';
static const char GRADE_UPDATE_KEY = '2';
static const char GRADE_DELETE_KEY = '3';
static const char GRADE_CLEAR_KEY = '4';

std::vector<std::pair<char, std::string>> WarGrey::IMS::GradeMenu::prepare_menu_items() {
    return {
        { MENU_RETURN_KEY, "返回上层"},
        { GRADE_CREATE_KEY, "添加成绩信息"},
        { GRADE_UPDATE_KEY, "更新成绩信息"},
        { GRADE_DELETE_KEY, "删除成绩信息"},
        { GRADE_CLEAR_KEY,  "清理游离成绩"}
    };
}

void WarGrey::IMS::GradeMenu::on_menu_key(IMenuEventListener* master, MenuType self, char key) {
    switch (key) {
    case GRADE_CREATE_KEY: master->on_menu_task(self, MenuTask::CreateGrade); break;
    case GRADE_UPDATE_KEY: master->on_menu_task(self, MenuTask::UpdateGrade); break;
    case GRADE_DELETE_KEY: master->on_menu_task(self, MenuTask::DeleteGrade); break;
    case GRADE_CLEAR_KEY: master->on_menu_task(self, MenuTask::ClearGrade); break;
    case MENU_RETURN_KEY: master->on_menu_switch(self, MenuType::TopLevel); break;
    default: /* do nothing */;
    }
}
