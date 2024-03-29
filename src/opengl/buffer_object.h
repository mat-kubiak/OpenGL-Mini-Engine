#ifndef OPENGL_BUFFEROBJECT_H_
#define OPENGL_BUFFEROBJECT_H_

#include "opengl/gl_object.h"

class BufferObject : public GLObject {
 protected:
  glint_t type;

 public:
  BufferObject(glint_t type);
  ~BufferObject();

  glint_t getType();

  void bind();
  void unbind();

  void data(unsigned int size, const void* data, unsigned int usage);

  template <typename T>
  void data(size_t size, T data[], unsigned int usage) {
    glBufferData(type, size, data, usage);
  }
};

class Vbo : public BufferObject {
 public:
  Vbo() : BufferObject(GL_ARRAY_BUFFER){};
};

class Ebo : public BufferObject {
 public:
  Ebo() : BufferObject(GL_ELEMENT_ARRAY_BUFFER){};
};

#endif