#pragma once

#include "entity.hpp"

namespace WarGrey::IMS {
    class ClassEntity : public WarGrey::IMS::GMSEntity {
    public:
        static bool match(const std::string& line, int* offset);
        static const char* prompt();

    public:
        ClassEntity(const char* s, int idx = 0) : ClassEntity(std::string(s), idx) {}
        ClassEntity(const std::string& s, int idx);
        virtual ~ClassEntity() {}

    public:
        uint64_t primary_key() override { return this->seq; }
        std::string to_string() override;

    private:
        uint64_t seq;
    };

    typedef std::shared_ptr<ClassEntity> shared_class_t;
}
