#include "digitama/game.hpp"
#include "digitama/text.hpp"

using namespace WarGrey::STEM;

/*************************************************************************************************/
namespace {
    class GradeManagementSystem : public Universe {
        public:
            GradeManagementSystem(int fps) : Universe("Grade Management System", fps) {}
        
        public:
            void construct(int argc, char* argv[]) {
                game_text_size(NULL, NULL, &this->usrline_height, "%s", "成绩管理系统");
                this->usrline_height += 2;
                this->set_snapshot_folder("/Users/wargrey/Desktop");
            }

            void reflow(int width, int height) {
                this->set_input_echo_area(0, height - this->usrline_height, width, this->usrline_height);
            }

            void draw(SDL_Renderer* renderer, int x, int y, int width, int height) {
                int ul_y = height - this->usrline_height;

                game_draw_line(renderer, x, y + ul_y, width, ul_y, GRAY);
            }

        private:
            int usrline_height;
    };
}

/*************************************************************************************************/
int main(int argc, char* args[]) {
    GradeManagementSystem universe(24);

    universe.construct(argc, args);
    universe.big_bang();

    return 0;
}


