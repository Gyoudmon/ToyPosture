#pragma once

#include "entity.hpp"

namespace WarGrey::IMS {
    class ClassEntity : public WarGrey::IMS::GMSEntity {
    public:
        static int match(const std::string& line);
        static const char* prompt();

    public:
        ClassEntity(const char* s, int idx = 0) : ClassEntity(std::string(s), idx) {}
        ClassEntity(const std::string& s, int idx);
        
        virtual ~ClassEntity() {}

    public:
        bool okay() override { return this->id > 0U; }
        uint64_t uuid() override { return this->id; }
        std::string to_string() override;

    private:
        uint64_t id;
    };

    typedef std::shared_ptr<ClassEntity> shared_class_t;
}
