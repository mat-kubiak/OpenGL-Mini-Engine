#include "core/app.h"

#include "core/global.h"
#include "core/util.h"
#include "core/input.h"
#include "settings/settings_loader.h"

void App::displayPerformanceWindow() {
  ImGuiWindowFlags window_flags = 0;
  window_flags |= ImGuiWindowFlags_NoTitleBar;
  window_flags |= ImGuiWindowFlags_NoBackground;
  window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
  window_flags |= ImGuiWindowFlags_NoInputs;
  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
  bool p_open = true;

  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::Begin("Performance", &p_open, window_flags);
  ImGui::Text("%.3f ms\n%.1f FPS", 1000.0f / ImGui::GetIO().Framerate,
              ImGui::GetIO().Framerate);
  ImGui::End();
}

//  GLFW Callbacks

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
  Global::window_width = width;
  Global::window_height = height;
}

void glfwErrorCallback(int error, const char* description) {
  std::cerr << "GLFW Error " + std::to_string(error) + ": " +
                   std::string(description);
}

void App::initialize() {
  // GLFW
  if (glfwInit() == GL_FALSE) {
    logger.log("Failed to initialize GLFW");
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // WINDOW CREATION
  window =
      glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
  if (window == NULL) {
    logger.log("Failed to create GLFW window");
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
  glfwSetErrorCallback(glfwErrorCallback);
  glfwSwapInterval(0);  // disable VSYNC

  // INPUT
  glint_t cursor_mode =
      Global::cameraMovementEnabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL;
  glfwSetInputMode(window, GLFW_CURSOR, cursor_mode);
  Input::init(window);

  //  GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    logger.log("Failed to initialize GLAD");
    exit(EXIT_FAILURE);
  }

  // ADDITIONAL
  glEnable(GL_DEPTH_TEST);

  // IMGUI
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(GLSL_VERSION);

  // APPDATA FOLDER
  path_t appdata = Path::getDataPath();
  if (!FileManager::dirExists(appdata))
    FileManager::createDir(appdata);

  imgui_settings = (appdata / IMGUI_SETTINGS_FILENAME).string();
  imgui_log = (appdata / IMGUI_LOG_FILENAME).string().c_str();

  io.IniFilename = imgui_settings.c_str();
  io.LogFilename = imgui_log.c_str();

  // SETTINGS
  settings.setDefaults();

  if (FileManager::fileExists(appdata / "settings.ini")) {
    try {
      std::string loaded = FileManager::readTextFile(appdata / "settings.ini");
      settings = SettingsLoader::loadFrom(loaded);
    } catch (const std::exception& e) {
      logger.log(e.what());
    }
  }

  std::tuple<float, float, float> color =
      HexColor::values(settings.get("background.color"));
  auto [red, green, blue] = color;
  this->bg_color[0] = (float)red / 255;
  this->bg_color[1] = (float)green / 255;
  this->bg_color[2] = (float)blue / 255;
}

void App::terminate() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwTerminate();

  path_t appdata = Path::getDataPath();

  if (!logger.empty())
    FileManager::appendTextFile(appdata / "log.txt", logger.pullLogs());

  settings.set(
      "background.color",
      HexColor::toStr(bg_color[0] * 255, bg_color[1] * 255, bg_color[2] * 255));
  FileManager::writeTextFile(appdata / "settings.ini",
                             SettingsLoader::saveInto(settings));
}

GLFWwindow* App::getWindow() const {
  return window;
}

bool App::shouldClose() const {
  return glfwWindowShouldClose(window);
}

void App::startFrame() {
  Input::processCamera(window);

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  // glClearColor(BG_COLOR);
  glClearColor(bg_color[0], bg_color[1], bg_color[2], 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (display_diagnostics)
    displayPerformanceWindow();
}

void App::endFrame() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  glfwSwapBuffers(window);
  glfwPollEvents();
}
