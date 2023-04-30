#include "grade.hpp"

#include <ios>
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

    scan_skip_space(src, &pos, end);

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
    this->points.clear();

    scan_skip_space(ss, &idx, end);
    
    while (idx < end) {
        double s = scan_flonum(ss, &idx, end);

        if ((s >= 0.0) && (s <= 150.0)) {    
            this->points.push_back(s);
        } else {
            throw exn_gms("分值(%lf)不合理", s);
        }
    }
}

void WarGrey::IMS::GradeEntity::feed_score_points(std::vector<double>& pts) {
    size_t total = this->points.size();

    if (pts.size() != total) {
        pts.resize(total);
    }

    for (size_t idx = 0; idx < total; idx ++) {
        pts[idx] = this->points[idx];
    }
}

double WarGrey::IMS::GradeEntity::get_score() {
    double score = 0.0;

    for (size_t idx = 0; idx < this->points.size(); idx ++) {
        score += this->points[idx];
    }

    return score;
}

std::string WarGrey::IMS::GradeEntity::to_string() {
    std::stringstream ss;

    ss << student_mark << discipline_mark << timestamp_mark << ':';
    ss << this->student_No << ',' << this->discipline_code << ',' << this->timestamp << ',';

    if (!this->points.empty()) {
        ss.precision(1);
        ss << std::fixed;
        ss << this->points[0];
   
        for (size_t idx = 1; idx < this->points.size(); idx ++) {
            ss << ' ' << this->points[idx];
        }
    }

    return ss.str();
}
