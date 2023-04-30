#pragma once

#include "entity/class.hpp"
#include "entity/discipline.hpp"
#include "entity/student.hpp"
#include "entity/seat.hpp"
#include "entity/grade.hpp"

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
        void delete_class_as_user_request(const char* text, size_t size);

        void create_discipline_from_user_input(const char* text, size_t size);
        void update_discipline_from_user_input(const char* text, size_t size);
        void delete_discipline_as_user_request(const char* text, size_t size);

        void create_student_from_user_input(const char* text, size_t size);
        void update_student_from_user_input(const char* text, size_t size);
        void delete_student_as_user_request(const char* text, size_t size);

        void register_student_scores_from_user_input(uint64_t sNo, uint64_t disCode, uint64_t ts, const char* text, size_t size);
        void update_student_scores_from_user_input(uint64_t sNo, uint64_t disCode, uint64_t ts, const char* text, size_t size);
        void delete_student_scores_as_user_request(uint64_t sNo, uint64_t disCode, uint64_t ts);

        void clear_detached_students();
        void clear_detached_grades();

    public:
        void bind_student_to_class(uint64_t sNo, uint64_t clsId);
        void bind_student_to_seat(uint64_t sNo, uint64_t desk_idx, uint64_t seat_idx);
        uint64_t get_student_at_seat(uint64_t clsId, uint64_t desk_idx, uint64_t seat_idx);
        uint64_t get_student_class(uint64_t sNo);
        void feed_student_seat(uint64_t sNo, uint64_t* dsk_idx, uint64_t* st_idx);

    public:
        uint64_t get_discipline_code(DisciplineType type);
        size_t get_class_population(uint64_t clsId);
        uint64_t get_class_latest_timestamp(uint64_t clsId, size_t offset = 0);
        uint64_t get_student_latest_timestamp(uint64_t sNo, size_t offset = 0);
        double get_class_average_score(uint64_t clsId, uint64_t disCode, uint64_t timestamp);
        void feed_student_score_points(uint64_t sNo, uint64_t disCode, uint64_t timestamp, std::vector<double>& pts);

    private:
        void register_class(shared_class_t cls, bool in_batching);
        void register_discipline(shared_discipline_t dis, bool in_batching);
        void register_student(shared_student_t stu, bool in_batching);
        void register_student_scores(shared_grade_t score);
        void clear(bool broadcast = true);

    private:
        std::map<uint64_t, shared_class_t> classes;
        std::map<uint64_t, shared_discipline_t> disciplines;
        std::map<uint64_t, shared_student_t> students;
        std::map<uint64_t, shared_seat_t> seats;
        std::map<uint64_t, std::map<uint64_t, std::map<uint64_t, shared_grade_t>>> scores;

    private:
        std::map<DisciplineType, uint64_t> dis_codes;

    private:
        IModelListener* listener;
    };
}
