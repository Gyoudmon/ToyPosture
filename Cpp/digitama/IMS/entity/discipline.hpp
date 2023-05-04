#pragma once

#include "entity.hpp"

namespace WarGrey::IMS {
    enum class DisciplineType { Mathematics, Programming, Physics, Chemistry, Biology, Blank };

    class DisciplineEntity : public WarGrey::IMS::GMSEntity {
    public:
        static bool match(const std::string& line, int* offset);
        static const char* prompt();
        static const char* update_prompt();

    public:
        static const char* type_to_name(DisciplineType type);
        static DisciplineType name_to_type(const char* name);

    public:
        DisciplineEntity(const char* s, int idx = 0) : DisciplineEntity(std::string(s), idx) {}
        DisciplineEntity(const std::string& s, int idx);
        virtual ~DisciplineEntity() {}

    public:
        bool update(const char* s, size_t end, size_t idx = 0);

    public:
        const char* cannonical_name();
        DisciplineType cannonical_type() { return this->type; }
        uint64_t get_credit() { return this->credit; }

    public:
        uint64_t primary_key() override { return this->code; }
        std::string to_string() override;

    private:
        uint64_t code;
        DisciplineType type;
        uint64_t credit;
    };

    typedef std::shared_ptr<DisciplineEntity> shared_discipline_t;
}
