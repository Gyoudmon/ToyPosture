#include "entity.hpp"

#include "../../big_bang/datum/box.hpp"

using namespace WarGrey::IMS;
using namespace WarGrey::STEM;

/*************************************************************************************************/
bool WarGrey::IMS::GMSEntity::match(const std::string& line, char line_mark, int* offset) {
    bool matched = false;

    if (line.size() > 2) {
        if ((line[0] == line_mark) && (line[1] == ':')) {
            matched = true;
            SET_BOX(offset, 2);
        }
    }

    return matched;
}

bool WarGrey::IMS::GMSEntity::match(const std::string& line, char mark1, char mark2, int* offset) {
    bool matched = false;

    if (line.size() > 3) {
        if ((line[0] == mark1) && (line[1] == mark2) && (line[2] == ':')) {
            matched = true;
            SET_BOX(offset, 3);
        }
    }

    return matched;
}

bool WarGrey::IMS::GMSEntity::match(const std::string& line, char mark1, char mark2, char mark3, int* offset) {
    bool matched = false;

    if (line.size() > 4) {
        if ((line[0] == mark1) && (line[1] == mark2) && (line[2] == mark3) && (line[3] == ':')) {
            matched = true;
            SET_BOX(offset, 4);
        }
    }

    return matched;
}

/*************************************************************************************************/
WarGrey::IMS::exn_gms::exn_gms(const char* fmt, ...) {
    VSNPRINT(msg, fmt);
    this->message = msg;
}
