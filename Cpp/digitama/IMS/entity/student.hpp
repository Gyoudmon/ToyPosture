#pragma once

#include "entity.hpp"

namespace WarGrey::IMS {
    enum class StudentGender { Girl, Boy, _ };

    class StudentEntity : public WarGrey::IMS::GMSEntity {
    public:
        static bool match(const std::string& line, int* offset);
        static const char* prompt();

    public:
        StudentEntity(const char* s, int idx = 0) : StudentEntity(std::string(s), idx) {}
        StudentEntity(const std::string& s, int idx);
        
        virtual ~StudentEntity() {}

    public:
        uint8_t get_avatar() { return this->avatar; }
        std::string get_nickname() { return this->nickname; }
        uint8_t get_age() { return this->age; }

    public:
        uint64_t primary_key() override { return this->No; }
        std::string to_string() override;

    private:
        uint8_t avatar;
        uint64_t No;
        std::string nickname;
        StudentGender gender;
        uint8_t age;
    };

    typedef std::shared_ptr<StudentEntity> shared_student_t;
}
