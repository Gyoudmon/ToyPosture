#include "calorie_counting.hpp"

#include "../big_bang/datum/path.hpp"
#include "../big_bang/physics/random.hpp"

#include <iostream>
#include <fstream>
#include <string>

using namespace WarGrey::STEM;
using namespace WarGrey::AoC;

/*************************************************************************************************/
static const char* population_fmt = "精灵数量: %d";
static const char* puzzle_fmt = "谜题%d答案: %d";

static inline const char* elf_name() {
    switch (random_uniform(0, 2)) {
    case 1: return "elf"; break;
    case 2: return "fairy"; break;
    default: return "goblin";
    }
}

/*************************************************************************************************/
// 实现 AoCWorld::construct 方法，加载输入数据
void WarGrey::AoC::AoCWorld::construct(int argc, char* argv[]) {
    if (argc > 1) {
        this->load_calories(std::string(argv[1]));
    } else {
        this->load_calories(digimon_path("2022/calorie.counting.dat"));
    }

    this->big_font = game_create_font(font_basename(game_font::unicode), 42);
}

// 实现 AoCWorld::load 方法，加载精灵
void WarGrey::AoC::AoCWorld::load(float width, float height) {
    this->population = this->insert(new Labellet(this->big_font, GOLDENROD, population_fmt, this->elves.size()));
    this->p1_answer = this->insert(new Labellet(this->big_font, ROYALBLUE, puzzle_fmt, 1, this->find_maximum_calories(1)));
    this->p2_answer = this->insert(new Labellet(this->big_font, CRIMSON, puzzle_fmt, 2, this->find_maximum_calories(3)));
    this->insert(this->elves[0]);
}

// 实现 AoCWorld::reflow 方法，调整精灵位置
void WarGrey::AoC::AoCWorld::reflow(float width, float height) {
    this->move_to(this->p1_answer, this->population, MatterAnchor::LB, MatterAnchor::LT);
    this->move_to(this->p2_answer, this->p1_answer, MatterAnchor::LB, MatterAnchor::LT);
    this->move_to(this->elves[0], width * 0.5F, height * 0.5F, MatterAnchor::CC);
}

// 实现 AoCWorld::update 方法
void WarGrey::AoC::AoCWorld::update(uint32_t interval, uint32_t count, uint32_t uptime) {
}

/*************************************************************************************************/
// 实现 AoCWorld::load_calories 方法，加载输入数据
void WarGrey::AoC::AoCWorld::load_calories(const std::string& pathname) {
    std::ifstream datin(pathname);
    Elfmon* elf = nullptr;

    this->elves.clear();

    if (datin.is_open()) {    
        std::string line;

        while (std::getline(datin, line)) {
            if (line.size() == 0) {
                this->elves.push_back(elf);
                elf = nullptr;
            } else {
                if (elf == nullptr) {
                    elf = new Elfmon(elf_name());
                }

                elf->calories.push_back(std::stoi(line));
            }
        }

        if (elf != nullptr) {
            this->elves.push_back(elf);
        }

        datin.close();
    }
}

// 实现 AoCWorld::find_maximum_calories 方法，计算携带最多卡路里的 n 个精灵所带卡路里的总和
int WarGrey::AoC::AoCWorld::find_maximum_calories(int n) {
    std::vector<int> calories(n);
    int total = 0;

    for (int eidx = 0; eidx < this->elves.size(); eidx ++) {
        int cal = this->elves[eidx]->calorie_total();

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
WarGrey::AoC::Elfmon::Elfmon(const char* dirname)
    : Sprite(digimon_path(dirname, "", "stone/sprite")) {}

int WarGrey::AoC::Elfmon::calorie_total() {
    int total = 0;

    for (auto cal : this->calories) {
        total += cal;        
    }

    return total;  
}
