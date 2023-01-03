#include "calorie_counting.hpp"

#include <iostream>
#include <fstream>
#include <string>

using namespace WarGrey::STEM;
using namespace WarGrey::AoC;

/*************************************************************************************************/
// 实现 AoCWorld::construct 方法，加载输入数据
void WarGrey::AoC::AoCWorld::construct(int argc, char* argv[]) {
    std::string pathname;

    if (argc > 1) {
        pathname = argv[1];
    } else {
        pathname = std::string(__ZONE__).append("stone/2022/calorie.counting.dat");
    }

    this->load_calories(pathname);
    this->big_font = game_create_font(font_basename(game_font::DEFAULT), 72);
}

// 实现 AoCWorld::load 方法，加载精灵
void WarGrey::AoC::AoCWorld::load(float width, float height) {
    this->top1_cal = this->insert(new Labellet(this->big_font, BLACK, "%d", this->find_maximum_calories(1)));
    this->top3_cal = this->insert(new Labellet(this->big_font, BLACK, "%d", this->find_maximum_calories(3)));
}

// 实现 AoCWorld::reflow 方法，调整精灵位置
void WarGrey::AoC::AoCWorld::reflow(float width, float height) {
    this->move_to(this->top3_cal, 0.0F, height, MatterAnchor::LB);
}

// 实现 AoCWorld::update 方法
void WarGrey::AoC::AoCWorld::update(uint32_t interval, uint32_t count, uint32_t uptime) {
}

/*************************************************************************************************/
// 实现 AoCWorld::load_calories 方法，加载输入数据
void WarGrey::AoC::AoCWorld::load_calories(std::string& pathname) {
    std::ifstream datin(pathname);
    Elf elf;

    this->elves.clear();

    if (datin.is_open()) {    
        std::string line;

        while (std::getline(datin, line)) {
            if (line.size() == 0) {
                this->elves.push_back(elf);
                elf.calories.clear();
            } else {
                elf.calories.push_back(std::stoi(line));
            }
        }

        datin.close();
    }
}

// 实现 AoCWorld::find_maximum_calories 方法，计算携带最多卡路里的 n 个精灵所带卡路里的总和
int WarGrey::AoC::AoCWorld::find_maximum_calories(int n) {
    std::vector<int> calories(n);
    int total = 0;

    for (int eidx = 0; eidx < this->elves.size(); eidx ++) {
        int cal = this->elves[eidx].calorie_total();

        for (int cidx = 0; cidx < n; cidx ++) {
            if (cal > calories[cidx]) {
                calories[cidx] = cal;
                break;
            }
        }
    }

    for (int idx = 0; idx < n; idx ++) {
        total += calories[idx];
    }

    return total;
}

/*************************************************************************************************/
int WarGrey::AoC::Elf::calorie_total() {
    int total = 0;

    for (auto cal : this->calories) {
        total += cal;        
    }

    return total;  
}
