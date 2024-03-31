#ifndef CORE_GLOBAL_H_
#define CORE_GLOBAL_H_

#include "core/camera.h"

/// @brief  namespace for default global data
namespace Global {
inline float cursorX = 0.0f, cursorY = 0.0f;
inline bool firstCameraRecord = true;
inline bool cameraMovementEnabled = true;

inline int debugNumber = 0;

inline Camera camera;
}  // namespace Global

#endif