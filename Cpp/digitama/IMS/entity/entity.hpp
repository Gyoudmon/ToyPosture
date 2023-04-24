#pragma once

#include <cstdint>
#include <exception>

#include "../../big_bang/datum/string.hpp"

/*************************************************************************************************/
namespace WarGrey::IMS {
    class GMSEntity {
    public:
        virtual ~GMSEntity() {}

    public:
        virtual bool okay() = 0;
        virtual uint64_t uuid() = 0;
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
