#pragma once

#include "../big_bang/game.hpp"

#include <vector>
#include <map>

namespace WarGrey::IMS {
    enum class MenuType { TopLevel, Class, Project, Student, Grade };
    enum class MenuTask { Exit, ImportData, ExportData, CreateClass, DeleteClass };

    class IMenuEventListener {
    public:
        virtual void on_menu_switch(MenuType self, MenuType to) = 0;
        virtual void on_menu_task(MenuType self, MenuTask task) = 0;
    };

    /*********************************************************************************************/
    class IMenu : public WarGrey::STEM::Plane {
    using Plane::Plane;

    public:
        void load(float width, float height) override;
        void reflow(float width, float height) override;

    public:
        void on_menu_char(IMenuEventListener* master, MenuType self, char key);

    public:
        size_t count() { return this->menuitems.size(); }
        void select_menu(char key);

    protected:
        virtual void on_menu_key(IMenuEventListener* master, MenuType self, char key) = 0;
        virtual std::vector<std::pair<char, std::string>> prepare_menu_items() = 0;
        
    private:
        std::vector<std::pair<char, std::string>> menuitems;
        std::map<char, WarGrey::STEM::Labellet*> menulabels;

    private:
        char current_menuitem = '\0';
    };

    /*********************************************************************************************/
    class TopLevelMenu : public WarGrey::IMS::IMenu {
    public:
        TopLevelMenu() : IMenu("Top Level Menu") {}
        virtual ~TopLevelMenu() {}

    protected:
        void on_menu_key(IMenuEventListener* master, MenuType self, char key) override;
        std::vector<std::pair<char, std::string>> prepare_menu_items() override;
    };

    class ClassMenu : public WarGrey::IMS::IMenu {
    public:
        ClassMenu() : IMenu("Class Management Menu") {}
        virtual ~ClassMenu() {}

    protected:
        void on_menu_key(IMenuEventListener* master, MenuType self, char key) override;
        std::vector<std::pair<char, std::string>> prepare_menu_items() override;
    };
}
