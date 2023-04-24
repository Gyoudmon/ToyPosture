#include "menu.hpp"

using namespace WarGrey::STEM;
using namespace WarGrey::IMS;

/*************************************************************************************************/
void WarGrey::IMS::IMenu::load(float width, float height) {
    auto font = GameFont::monospace(FontSize::x_large);
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
static const char TLM_RTRN_KEY = '0';
static const char TLM_IMPT_KEY = '1';
static const char TLM_EXPT_KEY = '2';
static const char TLM_CLSS_KEY = '3';
static const char TLM_PRJT_KEY = '4';
static const char TLM_STDT_KEY = '5';
static const char TLM_GRDE_KEY = '6';

std::vector<std::pair<char, std::string>> WarGrey::IMS::TopLevelMenu::prepare_menu_items() {
    return {
        { TLM_RTRN_KEY, "退出系统"},
        { TLM_IMPT_KEY, "加载数据"},
        { TLM_EXPT_KEY, "保存数据"},
        { TLM_CLSS_KEY, "班级管理"},
        { TLM_PRJT_KEY, "课程管理"},
        { TLM_STDT_KEY, "学生管理"},
        { TLM_GRDE_KEY, "成绩管理"}
    };
}

void WarGrey::IMS::TopLevelMenu::on_menu_key(IMenuEventListener* master, MenuType self, char key) {
    switch (key) {
    case TLM_CLSS_KEY: master->on_menu_switch(self, MenuType::Class); break;
    case TLM_PRJT_KEY: master->on_menu_switch(self, MenuType::Project); break;
    case TLM_STDT_KEY: master->on_menu_switch(self, MenuType::Student); break;
    case TLM_GRDE_KEY: master->on_menu_switch(self, MenuType::Grade); break;
    case TLM_IMPT_KEY: master->on_menu_task(self, MenuTask::ImportData); break;
    case TLM_EXPT_KEY: master->on_menu_task(self, MenuTask::ExportData); break;
    case TLM_RTRN_KEY: master->on_menu_task(self, MenuTask::Exit); break;
    default: /* do nothing */;
    }
}

/*************************************************************************************************/
static const char CLASS_RETURN_KEY = '0';
static const char CLASS_CREATE_KEY = '1';
static const char CLASS_DELETE_KEY = '2';

std::vector<std::pair<char, std::string>> WarGrey::IMS::ClassMenu::prepare_menu_items() {
    return {
        { CLASS_RETURN_KEY, "返回上层"},
        { CLASS_CREATE_KEY, "创建班级"},
        { CLASS_DELETE_KEY, "删除班级"}
    };
}

void WarGrey::IMS::ClassMenu::on_menu_key(IMenuEventListener* master, MenuType self, char key) {
    switch (key) {
    case CLASS_RETURN_KEY: master->on_menu_switch(self, MenuType::TopLevel); break;
    case CLASS_CREATE_KEY: master->on_menu_task(self, MenuTask::CreateClass); break;
    case CLASS_DELETE_KEY: master->on_menu_task(self, MenuTask::DeleteClass); break;
    default: /* do nothing */;
    }
}
