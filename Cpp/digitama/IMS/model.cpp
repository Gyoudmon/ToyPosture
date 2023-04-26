#include "model.hpp"

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
                if (ClassEntity::match(line, &offset)) {
                    shared_class_t cls = std::make_shared<ClassEntity>(line, offset);
                    uint64_t pk = cls->primary_key();

                    if (this->classes.find(pk) == this->classes.end()) {
                        this->classes[pk] = cls;
                        this->listener->on_class_created(pk, cls, true);
                    }
                } else if (DisciplineEntity::match(line, &offset)) {
                    shared_discipline_t dis = std::make_shared<DisciplineEntity>(line, offset);
                    uint64_t pk = dis->primary_key();

                    if (this->disciplines.find(pk) == this->disciplines.end()) {
                        this->disciplines[pk] = dis;
                        this->listener->on_discipline_created(pk, dis, true);
                    }
                } else if (StudentEntity::match(line, &offset)) {
                    shared_student_t stu = std::make_shared<StudentEntity>(line, offset);
                    uint64_t pk = stu->primary_key();

                    if (this->students.find(pk) == this->students.end()) {
                        this->students[pk] = stu;
                        this->listener->on_student_created(pk, stu, true);
                    }
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
        
        gmsout.close();
    }
}

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
}

/*************************************************************************************************/
void WarGrey::IMS::GradeManagementSystemModel::create_class_from_user_input(const char* text, size_t size) {
    shared_class_t cls = std::make_shared<ClassEntity>(text, 0);
    uint64_t pk = cls->primary_key();

    if (this->classes.find(pk) == this->classes.end()) {
        this->classes[pk] = cls;
        this->listener->on_class_created(pk, cls, false);
    } else {
        throw exn_gms("班级(%llu)已存在", cls->primary_key());
    }
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
    shared_discipline_t dis = std::make_shared<DisciplineEntity>(text, 0);
    uint64_t pk = dis->primary_key();

    if (this->disciplines.find(pk) == this->disciplines.end()) {
        this->disciplines[pk] = dis;
        this->listener->on_discipline_created(pk, dis, false);
    } else {
        throw exn_gms("课程(%llu)已存在", dis->primary_key());
    }
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
    shared_student_t stu = std::make_shared<StudentEntity>(text, 0);
    uint64_t pk = stu->primary_key();

    if (this->students.find(pk) == this->students.end()) {
        this->students[pk] = stu;
        this->listener->on_student_created(pk, stu, false);
    } else {
        throw exn_gms("学号(%llu)已存在", stu->primary_key());
    }
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

uint64_t WarGrey::IMS::GradeManagementSystemModel::get_student_class(uint64_t sNo) {
    uint64_t clsId = 0U;

    if (this->seats.find(sNo) != this->seats.end()) {
        clsId = this->seats[sNo]->get_class();        
    }

    return clsId;
}
