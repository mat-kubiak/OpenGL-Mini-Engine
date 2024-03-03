#include "OpenGL/GLObject.h"

void GLObject::setId(const glint& id) noexcept {
    this->id = id;
}

glint GLObject::getId() const noexcept {
    return this->id;
}

bool GLObject::empty() const noexcept {
    return this->id == 0;
}