#include "entity.hpp"

using namespace WarGrey::IMS;
using namespace WarGrey::STEM;

/*************************************************************************************************/
bool WarGrey::IMS::GMSEntity::match(const std::string& line, char line_mark, int* offset) {
    bool matched = false;

    if (line.size() > 2) {
        if ((line[0] == line_mark) && (line[1] == ':')) {
            matched = true;
            if (offset != nullptr) {
                (*offset) = 2;
            }
        }
    }

    return matched;
}

/*************************************************************************************************/
WarGrey::IMS::exn_gms::exn_gms(const char* fmt, ...) {
    VSNPRINT(msg, fmt);
    this->message = msg;
}
