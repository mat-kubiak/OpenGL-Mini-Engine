#pragma once

// VERSION
#define VERSION_TAG ""
#define VERSION_MAJOR 1
#define VERSION_MINOR 0
#define VERSION_PATCH 0

#define GLSL_VERSION "#version 330 core"

// FILES AND DIRECTORIES
#define LOG_FILENAME "log.txt"
#define SETTINGS_FILENAME "settings.ini"
#define APPDATA_DIRNAME "OpenGL Mini Engine"

// WINDOW
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "OpenGL Mini Graphics Engine"
#define BG_COLOR 0.2f, 0.3f, 0.3f, 1.0f

// TYPES
#define glint uint32_t
#define path_t std::filesystem::path
#define tree_t boost::property_tree::ptree