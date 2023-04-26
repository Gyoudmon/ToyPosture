#pragma once

#include <cstdint>
#include <exception>

#include "../../big_bang/datum/string.hpp"

/*************************************************************************************************/
namespace WarGrey::IMS {
#define FIELD_DELIM ','

    class GMSEntity {
    protected:
        static bool match(const std::string& line, char mark, int* offset);
        
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
