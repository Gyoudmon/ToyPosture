#pragma once

#include "entity.hpp"

namespace WarGrey::IMS {
    class SeatEntity : public WarGrey::IMS::GMSEntity {
    public:
        static bool match(const std::string& line, int* offset);
        static const char* prompt();

    public:
        SeatEntity(const char* s, int idx = 0) : SeatEntity(std::string(s), idx) {}
        SeatEntity(const std::string& s, int idx);
        SeatEntity(uint64_t sNo, uint64_t clsId, uint64_t desk = 0, uint64_t seat = 0);
        virtual ~SeatEntity() {}

    public:
        uint64_t get_student() { return this->student_No; }
        void set_class(uint64_t cls) { this->class_id = cls; }
        uint64_t get_class() { return this->class_id; }
        void set_seat(uint64_t dsk, uint64_t st) { this->desk = dsk; this->seat = st; }
        uint64_t get_desk() { return this->desk; }
        uint64_t get_seat() { return this->seat; }

    public:
        uint64_t primary_key() override { return (this->student_No << 32U) | this->class_id; }
        std::string to_string() override;

    private:
        uint64_t student_No;
        uint64_t class_id;
        uint64_t desk;
        uint64_t seat;
    };

    typedef std::shared_ptr<SeatEntity> shared_seat_t;
}
