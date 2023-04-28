#include "model.hpp"

#include "../big_bang/datum/box.hpp"

#include <fstream>
#include <filesystem>

using namespace WarGrey::STEM;
using namespace WarGrey::IMS;

using namespace std::filesystem;

/*************************************************************************************************/
void WarGrey::IMS::GradeManagementSystemModel::import_from_file(const std::string& path_gms) {
    if (exists(path_gms) && (!is_directory(path_gms))) {
        std::ifstream gmsin;
        std::string line;
        int offset;

        gmsin.exceptions(std::ios_base::badbit | std::ios_base::failbit);
        gmsin.open(path_gms);

        this->clear();

        while (std::getline(gmsin, line)) {
            try {
                if (GradeEntity::match(line, &offset)) {
                    this->register_student_scores(std::make_shared<GradeEntity>(line, offset));
                } else if (StudentEntity::match(line, &offset)) {
                    this->register_student(std::make_shared<StudentEntity>(line, offset), true);
                } else if (ClassEntity::match(line, &offset)) {
                    this->register_class(std::make_shared<ClassEntity>(line, offset), true);
                } else if (DisciplineEntity::match(line, &offset)) {
                    this->register_discipline(std::make_shared<DisciplineEntity>(line, offset), true);
                } else if (SeatEntity::match(line, &offset)) {
                    shared_seat_t seat = std::make_shared<SeatEntity>(line, offset);
                    uint64_t sNo = seat->get_student();

                    if (this->seats.find(sNo) == this->seats.end()) {
                        this->seats[sNo] = seat;
                    }
                }
            } catch (const std::exception& e) {
                printf("%s in line \"%s\"\n", e.what(), line.c_str());
            }
        }

        gmsin.close();
    }
}

void WarGrey::IMS::GradeManagementSystemModel::export_to_file(const std::string& path_gms, bool override_if_exists) {
    if (!exists(path_gms) || override_if_exists) {
        std::ofstream gmsout;
        
        gmsout.exceptions(std::ios_base::badbit | std::ios_base::failbit);
        gmsout.open(path_gms);

        for (auto cls : this->classes) {
            gmsout << cls.second->to_string() << std::endl;
        }

        for (auto dis : this->disciplines) {
            gmsout << dis.second->to_string() << std::endl;
        }

        for (auto stu : this->students) {
            gmsout << stu.second->to_string() << std::endl;
        }

        for (auto seat : this->seats) {
            gmsout << seat.second->to_string() << std::endl;
        }

        for (auto ts_score : this->scores) {
            for (auto dis_score : ts_score.second) {
                for (auto score : dis_score.second) {
                    gmsout << score.second->to_string() << std::endl;
                }
            }
        }
        
        gmsout.close();
    }
}

/*************************************************************************************************/
void WarGrey::IMS::GradeManagementSystemModel::clear() {
    for (auto cls : this->classes) {
        this->listener->on_class_deleted(cls.first, cls.second, true);
    }
    this->classes.clear();

    for (auto dis : this->disciplines) {
        this->listener->on_discipline_deleted(dis.first, dis.second, true);
    }
    this->disciplines.clear();

    for (auto stu : this->students) {
        this->listener->on_student_deleted(stu.first, stu.second, true);
    }
    this->students.clear();
    this->seats.clear();
    this->scores.clear();
}

/**
 * Delete student records that binding classes have been deleted
 * but leave those binding no classes as-is.
 */
void WarGrey::IMS::GradeManagementSystemModel::clear_detached_students() { 
    auto it = this->students.begin();

    while (it != this->students.end()) {
        if (this->seats.find(it->first) != this->seats.end()) {
            auto seat = this->seats[it->first];
            uint64_t clsId = seat->get_class();

            // clsId will never be 0 if there is a seat record.
            if (this->classes.find(clsId) == this->classes.end()) {
                this->listener->on_student_deleted(it->first, it->second, true);
                this->seats.erase(it->first);
            }

            it = this->students.erase(it);
        } else { // no seat ==> no binding class
            ++ it;
        }
    }
}

/**
 * Delete grade records that related students or diciplines have been deleted
 */
void WarGrey::IMS::GradeManagementSystemModel::clear_detached_grades() {
    auto it = this->scores.begin();

    while (it != this->scores.end()) {
        if (this->students.find(it->first) == this->students.end()) {
            it = this->scores.erase(it);
        } else {
            for (auto& ts_scores : it->second) {
                auto dis_it = ts_scores.second.begin();

                while (dis_it != ts_scores.second.end()) {
                    if (this->disciplines.find(dis_it->first) != this->disciplines.end()) {
                        dis_it = ts_scores.second.erase(dis_it);
                    } else {
                        ++ dis_it;
                    }
                }
            }

            ++ it;
        }
    }
}

void WarGrey::IMS::GradeManagementSystemModel::register_student_scores(shared_grade_t grade) {
    uint64_t sNo = grade->get_student();
    uint64_t ts = grade->get_timestamp();
    uint64_t dis = grade->get_discipline();

    if (this->scores.find(sNo) != this->scores.end()) {
        auto ts_scores = this->scores[sNo];

        if (ts_scores.find(ts) != ts_scores.end()) {
            auto dis_scores = ts_scores[ts];

            if (dis_scores.find(dis) == dis_scores.end()) {
                dis_scores[dis] = grade;
            } else {
                if (this->disciplines.find(dis) != this->disciplines.end()) {
                    throw exn_gms("成绩已(%s@%llu)登记", this->disciplines[dis]->cannonical_name(), ts);
                } else {
                    throw exn_gms("成绩已(%llu@%llu)登记", dis, ts);
                }
            }
        } else {
            ts_scores[ts] = { { dis, grade } };
        }
    } else {
        this->scores[sNo] = { { ts, { { dis, grade } }} };
    }
}

void WarGrey::IMS::GradeManagementSystemModel::register_class(shared_class_t cls, bool in_batching) {
    uint64_t pk = cls->primary_key();

    if (this->classes.find(pk) == this->classes.end()) {
        this->classes[pk] = cls;
        this->listener->on_class_created(pk, cls, in_batching);
    } else {
        throw exn_gms("班级(%llu)已存在", cls->primary_key());
    }
}

void WarGrey::IMS::GradeManagementSystemModel::register_discipline(shared_discipline_t dis, bool in_batching) {
    uint64_t pk = dis->primary_key();

    if (this->disciplines.find(pk) == this->disciplines.end()) {
        this->disciplines[pk] = dis;
        this->listener->on_discipline_created(pk, dis, in_batching);
    } else {
        throw exn_gms("课程(%llu)已存在", dis->primary_key());
    }
}

void WarGrey::IMS::GradeManagementSystemModel::register_student(shared_student_t stu, bool in_batching) {
    uint64_t pk = stu->primary_key();

    if (this->students.find(pk) == this->students.end()) {
        this->students[pk] = stu;
        this->listener->on_student_created(pk, stu, in_batching);
    } else {
        throw exn_gms("学号(%llu)已存在", stu->primary_key());
    }
}

/*************************************************************************************************/
void WarGrey::IMS::GradeManagementSystemModel::create_class_from_user_input(const char* text, size_t size) {
    this->register_class(std::make_shared<ClassEntity>(text, 0), false);
}

void WarGrey::IMS::GradeManagementSystemModel::delete_class_as_user_required(const char* text, size_t size) {
    size_t pos = 0;
    uint64_t cls_pk = scan_natural(text, &pos, size);

    if (this->classes.find(cls_pk) != this->classes.end()) {
        shared_class_t entity = this->classes[cls_pk];

        this->classes.erase(cls_pk);
        this->listener->on_class_deleted(cls_pk, entity, false);
    } else {
        throw exn_gms("查无此班(%llu)", cls_pk);
    }
}

void WarGrey::IMS::GradeManagementSystemModel::create_discipline_from_user_input(const char* text, size_t size) {
    this->register_discipline(std::make_shared<DisciplineEntity>(text, 0), false);
}

void WarGrey::IMS::GradeManagementSystemModel::delete_discipline_as_user_required(const char* text, size_t size) {
    size_t pos = 0;
    uint64_t dis_pk = scan_natural(text, &pos, size);

    if (this->disciplines.find(dis_pk) != this->disciplines.end()) {
        shared_discipline_t entity = this->disciplines[dis_pk];

        this->disciplines.erase(dis_pk);
        this->listener->on_discipline_deleted(dis_pk, entity, false);
    } else {
        throw exn_gms("查无此课(%llu)", dis_pk);
    }
}

void WarGrey::IMS::GradeManagementSystemModel::create_student_from_user_input(const char* text, size_t size) {
    this->register_student(std::make_shared<StudentEntity>(text, 0), false);
}

void WarGrey::IMS::GradeManagementSystemModel::delete_student_as_user_required(const char* text, size_t size) {
    size_t pos = 0;
    uint64_t stu_pk = scan_natural(text, &pos, size);

    if (this->students.find(stu_pk) != this->students.end()) {
        shared_student_t entity = this->students[stu_pk];

        this->students.erase(stu_pk);
        this->listener->on_student_deleted(stu_pk, entity, false);
    } else {
        throw exn_gms("查无此人(%llu)", stu_pk);
    }
}

/*************************************************************************************************/
void WarGrey::IMS::GradeManagementSystemModel::bind_student_to_class(uint64_t sNo, uint64_t clsId) {
    if (this->seats.find(sNo) == this->seats.end()) {
        this->seats[sNo] = std::make_shared<SeatEntity>(sNo, clsId);
    } else {
        this->seats[sNo]->set_class(clsId);
    }
}

void WarGrey::IMS::GradeManagementSystemModel::bind_student_to_seat(uint64_t sNo, uint64_t dsk_idx, uint64_t seat_idx) {
    if (this->seats.find(sNo) != this->seats.end()) {
        this->seats[sNo]->set_seat(dsk_idx, seat_idx);
    }
}

uint64_t WarGrey::IMS::GradeManagementSystemModel::get_student_class(uint64_t sNo) {
    uint64_t clsId = 0U;

    if (this->seats.find(sNo) != this->seats.end()) {
        clsId = this->seats[sNo]->get_class();        
    }

    return clsId;
}

uint64_t WarGrey::IMS::GradeManagementSystemModel::get_student_at_seat(uint64_t clsId, uint64_t desk_idx, uint64_t seat_idx) {
    uint64_t sNo = 0U;

    for (auto seat : this->seats) {
        auto st = seat.second;

        if ((st->get_class() == clsId)
                && (st->get_desk() == desk_idx)
                && (st->get_seat() == seat_idx)) {
            sNo = seat.first;
            break;
        }
    }

    return sNo;
}

void WarGrey::IMS::GradeManagementSystemModel::feed_student_seat(uint64_t sNo, uint64_t* dsk_idx, uint64_t* st_idx) {
    if (this->seats.find(sNo) != this->seats.end()) {
        SET_BOX(dsk_idx, this->seats[sNo]->get_desk());
        SET_BOX(st_idx, this->seats[sNo]->get_seat());
    } else {
        SET_BOXES(dsk_idx, st_idx, 0.0F);
    }
}

/*************************************************************************************************/
void WarGrey::IMS::GradeManagementSystemModel::register_student_scores(uint64_t sNo, uint64_t disCode, uint64_t ts, const char* text, size_t size) {
    shared_grade_t grade = std::make_shared<GradeEntity>(sNo, disCode, ts);

    grade->extract_scores(text, size, 0U);
    this->register_student_scores(grade);
}

void WarGrey::IMS::GradeManagementSystemModel::feed_student_latest_scores(uint64_t sNo, uint64_t disCode, std::vector<uint8_t>& scores) {

}
