#include "entity.hpp"

using namespace WarGrey::IMS;
using namespace WarGrey::STEM;

/*************************************************************************************************/
WarGrey::IMS::exn_gms::exn_gms(const char* fmt, ...) {
    VSNPRINT(msg, fmt);
    this->message = msg;
}
