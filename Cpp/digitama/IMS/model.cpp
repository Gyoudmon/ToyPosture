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
                    ClassEntity* cls = new ClassEntity(line, offset);
                    uint64_t id = cls->uuid();

                    if (cls->okay() && (this->classes.find(id) == this->classes.end())) {
                        this->classes[id] = std::shared_ptr<ClassEntity>(cls);
                        this->listener->on_class_created(id, this->classes[id], true);
                    }
                } else if (DisciplineEntity::match(line, &offset)) {
                    DisciplineEntity* dis = new DisciplineEntity(line, offset);
                    uint64_t id = dis->uuid();

                    if (dis->okay() && (this->disciplines.find(id) == this->disciplines.end())) {
                        this->disciplines[id] = std::shared_ptr<DisciplineEntity>(dis);
                        this->listener->on_discipline_created(id, this->disciplines[id], true);
                    }
                }
            } catch (const std::exception& e) {}
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
}

/*************************************************************************************************/
void WarGrey::IMS::GradeManagementSystemModel::create_class_from_user_input(const char* text, size_t size) {
    ClassEntity* cls = new ClassEntity(text, 0);

    if (cls->okay()) {
        uint64_t id = cls->uuid();

        if (this->classes.find(id) == this->classes.end()) {
            this->classes[id] = std::shared_ptr<ClassEntity>(cls);
            this->listener->on_class_created(id, this->classes[id], false);
        } else {
            throw exn_gms("班级(%llu)已存在", cls->uuid());
        }
    } else {
        throw exn_gms("无效输入");
    }
}

void WarGrey::IMS::GradeManagementSystemModel::delete_class_as_user_required(const char* text, size_t size) {
    size_t pos = 0;
    uint64_t cls_id = scan_natural(text, &pos, size);

    if (this->classes.find(cls_id) != this->classes.end()) {
        shared_class_t entity = this->classes[cls_id];

        this->classes.erase(cls_id);
        this->listener->on_class_deleted(cls_id, entity, false);
    } else {
        throw exn_gms("查无此班(%llu)", cls_id);
    }
}

void WarGrey::IMS::GradeManagementSystemModel::create_discipline_from_user_input(const char* text, size_t size) {
    DisciplineEntity* dis = new DisciplineEntity(text, 0);

    if (dis->okay()) {
        uint64_t id = dis->uuid();

        if (this->disciplines.find(id) == this->disciplines.end()) {
            this->disciplines[id] = std::shared_ptr<DisciplineEntity>(dis);
            this->listener->on_discipline_created(id, this->disciplines[id], false);
        } else {
            throw exn_gms("课程(%llu)已存在", dis->uuid());
        }
    } else {
        throw exn_gms("无效输入");
    }
}

void WarGrey::IMS::GradeManagementSystemModel::delete_discipline_as_user_required(const char* text, size_t size) {
    size_t pos = 0;
    uint64_t dis_id = scan_natural(text, &pos, size);

    if (this->disciplines.find(dis_id) != this->disciplines.end()) {
        shared_discipline_t entity = this->disciplines[dis_id];

        this->disciplines.erase(dis_id);
        this->listener->on_discipline_deleted(dis_id, entity, false);
    } else {
        throw exn_gms("查无此课(%llu)", dis_id);
    }
}
