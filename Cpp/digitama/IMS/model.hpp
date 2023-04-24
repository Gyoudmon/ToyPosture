#pragma once

#include "entity/class.hpp"

#include <map>

/*************************************************************************************************/
namespace WarGrey::IMS {
    class IModelListener {
    public:
        virtual void on_class_created(uint64_t id, shared_class_t entity) = 0;
        virtual void on_class_deleted(uint64_t id, shared_class_t entity) = 0;
    };

    class GradeManagementSystemModel {
    public:
        GradeManagementSystemModel(IModelListener* listener) : listener(listener) {}
        virtual ~GradeManagementSystemModel() {}

    public:
        void import_from_file(const std::string& path_gms) {}
        void export_to_file(const std::string& path_gms) {}

    public:
        void create_class_from_user_input(const char* text, size_t size);
        void delete_class_as_user_required(const char* text, size_t size);

    private:
        std::map<uint64_t, shared_class_t> classes;

    private:
        IModelListener* listener;
    };
}
