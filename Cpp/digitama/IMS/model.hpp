#pragma once

#include "entity/class.hpp"
#include "entity/discipline.hpp"

#include <map>

/*************************************************************************************************/
namespace WarGrey::IMS {
    class IModelListener {
    public:
        virtual void on_class_created(uint64_t id, shared_class_t entity, bool in_batching) = 0;
        virtual void on_class_deleted(uint64_t id, shared_class_t entity, bool in_batching) = 0;
        virtual void on_discipline_created(uint64_t id, shared_discipline_t entity, bool in_batching) = 0;
        virtual void on_discipline_updated(uint64_t id, shared_discipline_t entity) {}
        virtual void on_discipline_deleted(uint64_t id, shared_discipline_t entity, bool in_batching) = 0;
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

    private:
        void clear();

    private:
        std::map<uint64_t, shared_class_t> classes;
        std::map<uint64_t, shared_discipline_t> disciplines;

    private:
        IModelListener* listener;
    };
}