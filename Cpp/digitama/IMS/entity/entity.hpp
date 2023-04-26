#pragma once

#include <cstdint>
#include <exception>

#include "../../big_bang/datum/string.hpp"

/*************************************************************************************************/
namespace WarGrey::IMS {
    static const char field_delimiter = ',';
    
    static const char class_mark = 'c';
    static const char discipline_mark = 'd';
    static const char student_mark = 's';

    /*********************************************************************************************/
    class GMSEntity {
    protected:
        static bool match(const std::string& line, char mark, int* offset);
        static bool match(const std::string& line, char mark1, char mark2, int* offset);
        
    public:
        virtual ~GMSEntity() {}

    public:
        virtual uint64_t primary_key() = 0;
        virtual std::string to_string() = 0;
    };

    class exn_gms : public std::exception {
    public:
        exn_gms(const char* msg, ...);

    public:
        const char* what() const noexcept override { return this->message.c_str(); }

    private:
        std::string message;
    };
}
