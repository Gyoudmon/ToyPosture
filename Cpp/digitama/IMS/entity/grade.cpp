#include "grade.hpp"

#include <sstream>

using namespace WarGrey::IMS;
using namespace WarGrey::STEM;

/*************************************************************************************************/
static const char timestamp_mark = 't';

/*************************************************************************************************/
bool WarGrey::IMS::GradeEntity::match(const std::string& line, int* offset) {
    return GMSEntity::match(line, student_mark, discipline_mark, timestamp_mark, offset);
}

const char* WarGrey::IMS::GradeEntity::prompt() {
    return "{ sNo:nat, disCode:nat, timestamp:nat, score:nat ... }";
}

/*************************************************************************************************/
WarGrey::IMS::GradeEntity::GradeEntity(const std::string& s, int idx) {
    size_t pos = size_t(idx);
    size_t end = s.size();
    const char* src = s.c_str();

    this->student_No = scan_natural(src, &pos, end);
    if (this->student_No == 0U) throw exn_gms("无效学号");
    scan_skip_delimiter(src, &pos, end, field_delimiter);

    this->discipline_code = scan_natural(src, &pos, end);
    if (this->discipline_code == 0U) throw exn_gms("无效课程代号");
    scan_skip_delimiter(src, &pos, end, field_delimiter);

    this->timestamp = scan_natural(src, &pos, end);
    if (this->timestamp == 0U) throw exn_gms("无效时间戳");
    scan_skip_delimiter(src, &pos, end, field_delimiter);

    this->extract_scores(src, end, pos);
}

WarGrey::IMS::GradeEntity::GradeEntity(uint64_t sNo, uint64_t disCode, uint64_t timestamp)
    : student_No(sNo), discipline_code(disCode), timestamp(timestamp) {}

void WarGrey::IMS::GradeEntity::extract_scores(const char* ss, size_t end, size_t idx) {
    this->scores.clear();

    while (idx < end) {
        uint64_t s = scan_natural(ss, &idx, end);

        if (s > 150) throw exn_gms("分值(%llu)过高", s);
        this->scores.push_back(uint8_t(s));
    }
}

uint32_t WarGrey::IMS::GradeEntity::get_score() {
    uint32_t score = 0U;

    for (size_t idx = 0; idx < this->scores.size(); idx ++) {
        score += this->scores[idx];
    }

    return score;
}

std::string WarGrey::IMS::GradeEntity::to_string() {
    std::stringstream ss;

    ss << student_mark << discipline_mark << timestamp_mark << ':';
    ss << this->student_No << ',' << this->discipline_code << ',' << this->timestamp << ',';

    if (!this->scores.empty()) {
        ss << this->scores[0];
   
        for (size_t idx = 1; idx < this->scores.size(); idx ++) {
            ss << ' ' << this->scores[1];
        }
    }

    return ss.str();
}
