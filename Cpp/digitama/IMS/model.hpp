#pragma once

#include "entity/class.hpp"
#include "entity/discipline.hpp"
#include "entity/student.hpp"
#include "entity/seat.hpp"

#include <map>

/*************************************************************************************************/
namespace WarGrey::IMS {
    class IModelListener {
    public:
        virtual void on_class_created(uint64_t pk, shared_class_t entity, bool in_batching) = 0;
        virtual void on_class_deleted(uint64_t pk, shared_class_t entity, bool in_batching) = 0;

        virtual void on_discipline_created(uint64_t pk, shared_discipline_t entity, bool in_batching) = 0;
        virtual void on_discipline_updated(uint64_t pk, shared_discipline_t entity) {}
        virtual void on_discipline_deleted(uint64_t pk, shared_discipline_t entity, bool in_batching) = 0;
        
        virtual void on_student_created(uint64_t pk, shared_student_t entity, bool in_batching) = 0;
        virtual void on_student_updated(uint64_t pk, shared_student_t entity) {}
        virtual void on_student_deleted(uint64_t pk, shared_student_t entity, bool in_batching) = 0;
    };

    class GradeManagementSystemModel {
    public:
        GradeManagementSystemModel(IModelListener* listener) : listener(listener) {}
        virtual ~GradeManagementSystemModel() {}

    public:
        void import_from_file(const std::string& path_gms);
        void export_to_file(const std::string& path_gms, bool override_if_exists = true);

    public:
        void create_class_from_user_input(const char* text, size_t size);
        void delete_class_as_user_required(const char* text, size_t size);

        void create_discipline_from_user_input(const char* text, size_t size);
        void update_discipline_from_user_input(const char* text, size_t size);
        void delete_discipline_as_user_required(const char* text, size_t size);

        void create_student_from_user_input(const char* text, size_t size);
        void update_student_from_user_input(const char* text, size_t size);
        void delete_student_as_user_required(const char* text, size_t size);

    public:
        void bind_student_to_class(uint64_t sNo, uint64_t clsId);
        uint64_t get_student_class(uint64_t sNo);

    private:
        void clear();

    private:
        std::map<uint64_t, shared_class_t> classes;
        std::map<uint64_t, shared_discipline_t> disciplines;
        std::map<uint64_t, shared_student_t> students;
        std::map<uint64_t, shared_seat_t> seats;

    private:
        IModelListener* listener;
    };
}
