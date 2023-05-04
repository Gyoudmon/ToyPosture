#pragma once

#include "entity.hpp"

#include <vector>

namespace WarGrey::IMS {
    class GradeEntity : public WarGrey::IMS::GMSEntity {
    public:
        static bool match(const std::string& line, int* offset);
        static const char* prompt();

    public:
        GradeEntity(const char* s, int idx = 0) : GradeEntity(std::string(s), idx) {}
        GradeEntity(const std::string& s, int idx);
        GradeEntity(uint64_t sNo, uint64_t disCode, uint64_t timestamp);
        virtual ~GradeEntity() {}

    public:
        uint64_t get_student() { return this->student_No; }
        uint64_t get_discipline() { return this->discipline_code; }
        void set_discipline(uint64_t dis) { this->discipline_code = dis; }
        uint64_t get_timestamp() { return this->timestamp; }
        void set_timestamp(uint64_t ts) { this->timestamp = ts; }
        void extract_scores(const char* ss, size_t end, size_t idx = 0);
        void feed_score_points(std::vector<double>& pts);
        double get_score();
        
    public:
        uint64_t primary_key() override { return (this->student_No << 32U) | this->discipline_code; }
        std::string to_string() override;

    private:
        uint64_t student_No;
        uint64_t discipline_code;
        uint64_t timestamp;
        std::vector<double> points;
    };

    typedef std::shared_ptr<GradeEntity> shared_grade_t;
}
