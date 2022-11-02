#include "digitama/game.hpp"
#include "digitama/cosmos.hpp"

#include "algolet/randomlet.hpp"

#include "digitama/datum/string.hpp"
#include "digitama/graphlet/ui/textlet.hpp"

#include <map>
#include <deque>
#include <cstdlib>

using namespace WarGrey::STEM;
using namespace WarGrey::OS;

/*************************************************************************************************/
namespace {
    static const char* help_str = "<键盘操作: (V) 追加虚拟页流  (S) 手动调度  (A) 自动调度  (R) 重置>";

    enum ReplacementAlgorithm { RANDOM };
    enum EditOperation { APPEND_VIRTUAL_PAGES, AUTO_REPLACING, MANUAL_STEPPING };

    class VMemPlanet : public Planet {
        public:
            VMemPlanet(int avail_phges, float gridsize, int repwin_size, const std::string& vpages) : Planet("Virtual Memory Page Replacement") {
                this->physical_page = (avail_phges > 0) ? avail_phges : 4;
                this->gridsize = (gridsize > 0.0) ? gridsize : 32.0F;
                this->replacement_window = (repwin_size > 0) ? repwin_size : 32;
                this->cmd_stream = vpages;
            }

        public:
            void load(float width, float height) override {
                this->help = this->insert_one(new Labellet(game_unicode_font, help_str));
                this->queue = this->insert_one(new Labellet(game_monospace_font, ""));
                this->algos[RANDOM] = this->insert_one(new Randomlet(this->physical_page, this->gridsize, this->replacement_window));

                /* setup initial page stream */ {
                    size_t maxidx = this->cmd_stream.size();

                    if (maxidx > 0) {
                        this->append_virtual_pages(this->cmd_stream.c_str(), maxidx);
                    }
                }
            }

            void reflow(float width, float height) override {
                float x = width / 2.0F;
                float y = height / 2.0F;

                this->move_to(this->help, 0.0F, height, GraphletAnchor::LB);
                this->move_to(this->queue, width, height, GraphletAnchor::RB);

                this->move_to(this->algos[RANDOM], x, y, GraphletAnchor::CB);
            }

            void update(long long count, long long interval, long long uptime) override {
                if (this->op == AUTO_REPLACING) {
                    if (!this->virtual_page_step()) {
                        this->op = MANUAL_STEPPING;
                    }
                }
            }

        public:
            void on_char(char key, uint16_t modifiers, uint8_t repeats, bool pressed) override {
                if (!pressed) {
                    this->begin_update_sequence();
                    switch (key) {
                        case 'V': case 'v': {
                            this->op = APPEND_VIRTUAL_PAGES;
                            this->start_input_text("请输入虚拟页流: ");
                        }; break;
                        case 'S': case 's': {
                            if (this->op != AUTO_REPLACING) {
                                this->virtual_page_step();
                            }
                            this->op = MANUAL_STEPPING;
                        }; break;
                        case 'A': case 'a': {
                            this->op = AUTO_REPLACING;
                            // let the timer do the replacement
                        }; break;
                        case 'R': case 'r': {
                            this->op = MANUAL_STEPPING;
                            this->reset_algorithms();
                        }; break;
                    }
                    this->end_update_sequence();
                }
            }

            void on_text(const char* text, size_t size, bool entire) override {
                if (entire) {
                    this->begin_update_sequence();
                    switch (this->op) {
                        case APPEND_VIRTUAL_PAGES: this->append_virtual_pages(text, size); break;
                        default: { /* nothing to do */ }; break;
                    }
                    this->end_update_sequence();
                }
            }

        public:
            bool can_select(IGraphlet* g) override {
                return false;
            }

        private:
            void append_virtual_pages(const char* src, size_t maxidx) {
                size_t pos = 0;

                scan_skip_space(src, &pos, maxidx);
                while (pos < maxidx) {
                    unsigned long long v = scan_natural(src, &pos, maxidx, false);

                    if (v > 0) {
                        page_stream.push_back(int(v));
                    } else if (src[pos] != ' ') {
                        scan_skip_token(src, &pos, maxidx, false);
                    }

                    scan_skip_space(src, &pos, maxidx);
                }

                this->update_queue_label();
            }

            bool virtual_page_step() {
                bool okay = false;

                if (!this->page_stream.empty()) {
                    int v = this->page_stream.front();

                    for (auto it = this->algos.begin(); it != this->algos.end(); ++it) {
                        it->second->step(v);
                    }

                    this->page_stream.pop_front();
                    this->update_queue_label();

                    okay = true;
                }

                return okay;
            }

            void reset_algorithms() {
                this->page_stream.clear();
                this->update_queue_label();

                for (auto it = this->algos.begin(); it != this->algos.end(); ++it) {
                    it->second->reset();
                }
            }

        private:
            void update_queue_label() {
                std::string q;

                for (auto it = this->page_stream.begin(); it != this->page_stream.end(); ++it) {
                    if (!q.empty()) {
                        q.append(" ");
                    }

                    q.append(std::to_string((*it)));
                }

                this->queue->set_text(q, GraphletAnchor::RB);
            }

        private:
            std::map<ReplacementAlgorithm, IAlgolet*> algos;
            Labellet* help;
            Labellet* queue;

        private:
            std::string cmd_stream;
            std::deque<int> page_stream;

        private:
            EditOperation op;
            int physical_page;
            int replacement_window;
            float gridsize;
    };

    class VMemPageReplacement : public Cosmos {
        public:
            VMemPageReplacement(int fps) : Cosmos(fps) {}
        
        public:
            void construct(int argc, char* argv[]) {
                std::string vpages;
                int avail_phges = 0;
                int rep_window = 0;
                float gridsize = 0.0F;

                if (argc > 2) {
                    avail_phges = std::atoi(argv[1]);

                    if (argc > 3) {
                        rep_window = std::atoi(argv[2]);

                        if (argc > 4) {
                            gridsize = std::atof(argv[3]);

                            for (int i = 4; i < argc; i++) {
                                vpages.append(" ");
                                vpages.append(argv[i]);
                            }
                        }
                    }
                }

                this->set_snapshot_folder("/Users/wargrey/Desktop");
                this->push_planet(new VMemPlanet(avail_phges, gridsize, rep_window, vpages));
            }
    };
}

/*************************************************************************************************/
int main(int argc, char* args[]) {
    VMemPageReplacement universe(2);

    universe.construct(argc, args);
    universe.big_bang();

    return 0;
}

