#include "digitama/game.hpp"
#include "digitama/world.hpp"

#include "algolet/randomlet.hpp"
#include "algolet/fifolet.hpp"
#include "algolet/lrulet.hpp"

#include "digitama/datum/string.hpp"
#include "digitama/graphlet/ui/textlet.hpp"

#include <map>
#include <deque>
#include <cstdlib>
#include <cstring>

using namespace WarGrey::STEM;
using namespace WarGrey::OS;

/*************************************************************************************************/
namespace {
    static const char* help_str = "<快捷键: (V) 追加虚拟页流  (S) 手动调度  (A) 自动调度  (R) 重置  (Q) 退出>";

    enum ReplacementAlgorithm { RANDOM, FIFO, LRU, STACK };
    enum EditOperation { APPEND_VIRTUAL_PAGES, AUTO_REPLACING, MANUAL_STEPPING, EXIT };
    enum CmdlineOpts { PHYSICAL_PAGE, WINDOW_SIZE, GRIDSIZE, _ };

    class PageReplacementCosmos : public World {
        public:
            PageReplacementCosmos(int fps) : World("Page Replacement Algorithms", fps, 0xFFFFFFU, 0x000000U) {
                this->set_cmdwin_height(24);
            }

        public:
            void construct(int argc, char* argv[]) override {
                CmdlineOpts opt = CmdlineOpts::_;
                std::string vpages;
                int avail_phges = 0;
                int rep_window = 0;
                float gridsize = 0.0F;

                for (int i = 1; i < argc; i++) {
                    switch (opt) {
                        case CmdlineOpts::PHYSICAL_PAGE: avail_phges = std::atoi(argv[i]); opt = _; break;
                        case CmdlineOpts::GRIDSIZE: gridsize = float(std::atof(argv[i])); opt = _; break;
                        case CmdlineOpts::WINDOW_SIZE: rep_window = std::atoi(argv[i]); opt = _; break;
                        default: {
                            if (strncmp("-p", argv[i], 2) == 0) {
                                opt = CmdlineOpts::PHYSICAL_PAGE;
                            } else if (strncmp("-g", argv[i], 2) == 0) {
                                opt = CmdlineOpts::GRIDSIZE;
                            } else if (strncmp("-w", argv[i], 2) == 0) {
                                opt = CmdlineOpts::WINDOW_SIZE;
                            } else {
                                vpages.append(" ");
                                vpages.append(argv[i]);
                            }
                        }; break;
                    }
                }
    
                this->physical_page = (avail_phges > 0) ? avail_phges : 4;
                this->gridsize = (gridsize > 0.0) ? gridsize : 32.0F;
                this->replacement_window = (rep_window > 0) ? rep_window : 16;
                this->cmd_stream = vpages;
            }

            void load(float width, float height) override {
                this->help = self->insert_one(new Labellet(game_font::unicode, help_str));
                this->queue = self->insert_one(new Labellet(game_font::monospace, ""));
                this->algos[RANDOM] = self->insert_one(new Randomlet(this->physical_page, this->gridsize, this->replacement_window));
                this->algos[FIFO] = self->insert_one(new FIFOlet(this->physical_page, this->gridsize, this->replacement_window));
                this->algos[LRU] = self->insert_one(new LRUlet(this->physical_page, this->gridsize, this->replacement_window));
                this->algos[STACK] = self->insert_one(new LRUStacklet(this->physical_page, this->gridsize, this->replacement_window));

                /* setup initial page stream */ {
                    size_t maxidx = this->cmd_stream.size();

                    if (maxidx > 0) {
                        this->append_virtual_pages(this->cmd_stream.c_str(), maxidx);
                    }
                }
            }

            void reflow(float width, float height) override {
                float xspan = width * 0.25F;
                float yspan = height * 0.25F;

                self->move_to(this->help, 0.0F, height, GraphletAnchor::LB);
                self->move_to(this->queue, width, height, GraphletAnchor::RB);

                self->move_to(this->algos[RANDOM], xspan * 1.0F, yspan * 1.0F, GraphletAnchor::CC);
                self->move_to(this->algos[FIFO],   xspan * 3.0F, yspan * 1.0F, GraphletAnchor::CC);
                self->move_to(this->algos[LRU],    xspan * 1.0F, yspan * 3.0F, GraphletAnchor::CC);
                self->move_to(this->algos[STACK],  xspan * 3.0F, yspan * 3.0F, GraphletAnchor::CC);
            }

            void update(uint32_t count, uint32_t interval, uint32_t uptime) override {
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
                        case 'Q': case 'q': {
                            this->op = EXIT;
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
            bool can_exit() override {
                return (this->op == EditOperation::EXIT);
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
}

/*************************************************************************************************/
int main(int argc, char* args[]) {
    PageReplacementCosmos cosmos(2);

    cosmos.construct(argc, args);
    cosmos.big_bang();

    return 0;
}

