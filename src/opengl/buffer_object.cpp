#include "opengl/buffer_object.h"

BufferObject::BufferObject(glint_t type) : type(type) {
  glint_t temp = getId();
  glGenBuffers(1, &temp);
  setId(temp);
}

BufferObject::~BufferObject() {
  glint_t temp = getId();
  glDeleteBuffers(1, &temp);
  setId(temp);
}

glint_t BufferObject::getType() {
  return type;
}

// Binding

void BufferObject::bind() {
  glBindBuffer(type, getId());
}

void BufferObject::unbind() {
  glBindBuffer(type, 0);
}

void BufferObject::data(unsigned int size,
                        const void* data,
                        unsigned int usage) {
  glBufferData(type, size, data, usage);
}