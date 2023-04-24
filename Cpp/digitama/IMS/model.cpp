#include "model.hpp"

using namespace WarGrey::STEM;
using namespace WarGrey::IMS;

/*************************************************************************************************/
void WarGrey::IMS::GradeManagementSystemModel::create_class_from_user_input(const char* text, size_t size) {
    ClassEntity* cls = new ClassEntity(text, 0);

    if (cls->okay()) {
        uint64_t id = cls->uuid();

        if (this->classes.find(id) == this->classes.end()) {
            this->classes[id] = std::shared_ptr<ClassEntity>(cls);
            this->listener->on_class_created(id, this->classes[id]);
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
        this->listener->on_class_deleted(cls_id, entity);
    } else {
        throw exn_gms("查无此班(%llu)", cls_id);
    }
}
