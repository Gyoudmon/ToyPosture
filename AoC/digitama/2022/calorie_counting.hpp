#pragma once // 确保只被 include 一次

#include "../big_bang/game.hpp"

#include <vector>

namespace WarGrey::AoC {
    class Elfmon : public WarGrey::STEM::Sprite {
        public:
            Elfmon(const char* dirname);

        public:
            int calorie_total();

        public:
            std::vector<int> calories;
    };

    /******************************************* 声明游戏世界 ******************************************/
    class AoCWorld : public WarGrey::STEM::World {
        public:
            AoCWorld() : World("清点卡路里") {}
            virtual ~AoCWorld() { WarGrey::STEM::game_font_destroy(this->big_font); }

        public:    // 覆盖游戏基本方法
            void construct(int argc, char* argv[]) override;
            void load(float width, float height) override;
            void reflow(float width, float height) override;
            void update(uint32_t interval, uint32_t count, uint32_t uptime) override;

        private:
            void load_calories(const std::string& pathname);
            int find_maximum_calories(int n);

        private:   // 本游戏世界中的物体和数据
            WarGrey::STEM::Labellet* population;
            WarGrey::STEM::Labellet* p1_answer;
            WarGrey::STEM::Labellet* p2_answer;
            std::vector<WarGrey::AoC::Elfmon*> elves;

        private:
            TTF_Font* big_font;
    };
}
