#include "calorie_counting.hpp"

#include "../big_bang/datum/path.hpp"
#include "../big_bang/datum/flonum.hpp"
#include "../big_bang/physics/random.hpp"

#include <iostream>
#include <fstream>
#include <string>

using namespace WarGrey::STEM;
using namespace WarGrey::AoC;

/*************************************************************************************************/
static const char* population_unknown_fmt = "精灵数量: (未知)";
static const char* population_fmt = "精灵数量: %d";
static const char* puzzle_fmt = "谜题%d答案: %d";

static inline const char* elf_name(int hint) {
    switch ((hint < 3) ? hint : random_uniform(0, 2)) {
    case 1: return "fairy"; break;
    case 2: return "goblin"; break;
    default: return "elf";
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
    this->status = WorldStatus::Wait;
}

// 实现 AoCWorld::load 方法，加载精灵
void WarGrey::AoC::AoCWorld::load(float width, float height) {
    this->population = this->insert(new Labellet(this->big_font, GOLDENROD, population_unknown_fmt));
    this->p1_answer = this->insert(new Labellet(this->big_font, ROYALBLUE, puzzle_fmt, 1, this->find_maximum_calories(1)));
    this->p2_answer = this->insert(new Labellet(this->big_font, CRIMSON, puzzle_fmt, 2, this->find_maximum_calories(3)));

    if (this->elves.size() > 0) {
        this->begin_update_sequence();
        for (auto elf : this->elves) {
            this->insert(elf);
            elf->switch_to_custome("normal");
            elf->resize(0.25F);
        }
        this->end_update_sequence();
    }
}

// 实现 AoCWorld::reflow 方法，调整精灵位置
void WarGrey::AoC::AoCWorld::reflow(float width, float height) {
    this->move_to(this->p1_answer, this->population, MatterAnchor::LB, MatterAnchor::LT);
    this->move_to(this->p2_answer, this->p1_answer, MatterAnchor::LB, MatterAnchor::LT);

    if (this->elves.size() > 0) {
        float elf_width, elf_height;
        float lbl_width, lbl_height;
        
        this->elves[0]->feed_extent(0.0F, 0.0F, &elf_width, &elf_height);
        this->p2_answer->feed_extent(0.0F, 0.0F, &lbl_width, &lbl_height);
        
        this->cell_width = elf_width * 1.618F;
        this->cell_height = elf_height * 1.618F;
        this->grid_xoff = lbl_width * 1.2F;
        this->grid_yoff = lbl_height * 0.0F;

        this->col = int(flfloor((width - this->grid_xoff) / this->cell_width));
        this->row = int(flfloor((height - this->grid_yoff) / this->cell_height));
        
        this->begin_update_sequence();
        for (auto elf : this->elves) {
            this->move_elf_to_grid(elf);
        }
        this->end_update_sequence();
    }
}

// 实现 AoCWorld::update 方法
void WarGrey::AoC::AoCWorld::update(uint32_t interval, uint32_t count, uint32_t uptime) {
    switch (this->status) {
        case WorldStatus::CountOff: {
            if (this->current_elf_idx > 0) {
                this->elves[this->current_elf_idx - 1]->switch_to_custome("normal");
                this->elves[this->current_elf_idx - 1]->resize(0.75F);
            }
                
            if (this->current_elf_idx < this->elves.size()) {
                this->elves[this->current_elf_idx]->switch_to_custome("greeting");
                this->elves[this->current_elf_idx]->resize(1.5F);
                this->population->set_text(population_fmt, ++ this->current_elf_idx);
            } else {
                this->status = WorldStatus::Wait;
            }
        }; break;
        default: /* do nothing */;
    }
}

bool WarGrey::AoC::AoCWorld::can_select(IMatter* m) {
    return (this->status == WorldStatus::Wait);
}

void WarGrey::AoC::AoCWorld::after_select(IMatter* m, bool yes_or_no) {
    if (yes_or_no) {
        if (m == this->population) {
            this->status = WorldStatus::CountOff;
            this->current_elf_idx = 0;
        }
    }
}

void WarGrey::AoC::AoCWorld::move_elf_to_grid(Elfmon* elf) {
    int c = elf->idx % this->col;
    int r = elf->idx / this->col;

    this->move_to(elf,
        (c + 0.5F) * this->cell_width + this->grid_xoff,
        (r + 1.0F) * this->cell_height + this->grid_yoff,
        MatterAnchor::CB);
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
                    int idx = this->elves.size();

                    elf = new Elfmon(elf_name(idx), idx);
                    elf->enable_resizing(true, MatterAnchor::CB);
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
WarGrey::AoC::Elfmon::Elfmon(const char* dirname, int idx)
    : Sprite(digimon_path(dirname, "", "stone/sprite")), idx(idx) {}

int WarGrey::AoC::Elfmon::calorie_total() {
    int total = 0;

    for (auto cal : this->calories) {
        total += cal;        
    }

    return total;  
}
