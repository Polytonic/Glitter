#include "multi_light_renderer.hpp"

#include "rt_render_util.hpp"

namespace {

constexpr unsigned int SCR_WIDTH = 1200;
constexpr unsigned int SCR_HEIGHT = 800;
constexpr unsigned int SHADOW_WIDTH = 1024;
constexpr unsigned int SHADOW_HEIGHT = 1024;
constexpr int kNumLights = 16;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

Camera camera_(glm::vec3(0.0f, 0.0f, 2.0f));

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset =
      lastY - ypos;  // reversed since y-coordinates go from bottom to top

  lastX = xpos;
  lastY = ypos;

  camera_.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  camera_.ProcessMouseScroll(yoffset);
}

}  // namespace

MultiLightRenderer::MultiLightRenderer() {}

GLFWwindow* MultiLightRenderer::OpenWindow(const std::string& window_name) {
  window_ =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, window_name.c_str(), NULL, NULL);
  if (window_ == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    std::exit(-1);
  }
  glfwMakeContextCurrent(window_);
  glfwSetFramebufferSizeCallback(window_, &framebuffer_size_callback);
  glfwSetCursorPosCallback(window_, &mouse_callback);
  glfwSetScrollCallback(window_, &scroll_callback);

  // tell GLFW to capture our mouse
  glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    std::exit(-1);
  }

  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // build and compile shaders
  // -------------------------
  shader_.reset(new Shader("multi_light_basic.vs", "multi_light_basic.fs"));
  depth_shader_.reset(new Shader("3.1.3.shadow_mapping_depth.vs", "3.1.3.shadow_mapping_depth.fs"));
  light_box_shader_.reset(
      new Shader("8.1.deferred_light_box.vs", "8.1.deferred_light_box.fs"));

      // configure depth map FBO
    // -----------------------
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

  shader_->use();
  shader_->setInt("diffuseTexture", 0);
  shader_->setInt("shadowMap", 1);

  return window_;
}

void MultiLightRenderer::AddModel(const std::string& file_path,
                                  glm::mat4 model_matrix) {
  static_models_.push_back(
      std::unique_ptr<Model>(new Model(FileSystem::getPath(file_path))));
  static_model_matrices_.push_back(model_matrix);
}

void MultiLightRenderer::AddModel(std::unique_ptr<Renderable> model,
                                  glm::mat4 model_matrix) {
  static_models_.push_back(std::move(model));
  static_model_matrices_.push_back(model_matrix);
}

void MultiLightRenderer::AddDynamicModel(
    std::unique_ptr<DynamicRenderable> model) {
  dynamic_models_.push_back(std::move(model));
}

void MultiLightRenderer::AddEventHandler(CameraEventHandler* event_handler) {
  event_handlers_.push_back(event_handler);
}

void MultiLightRenderer::Render() {
  glfwMakeContextCurrent(window_);

  float currentFrame = glfwGetTime();
  float deltaTime = currentFrame - lastFrameTime;
  lastFrameTime = currentFrame;

  processInput(deltaTime);
  if (!pause_) {
    for (std::unique_ptr<DynamicRenderable>& model : dynamic_models_) {
      model->Tick(deltaTime);
    }
  }
  for (CameraEventHandler* handler : event_handlers_) {
    handler->TickUpdateCamera(&camera_, deltaTime);
  }

  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  shader_->use();
  glm::mat4 projection =
      glm::perspective(glm::radians(camera_.Zoom),
                       (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.5f, 500.0f);
  glm::mat4 view = camera_.GetViewMatrix();
  shader_->setMat4("projection", projection);
  shader_->setMat4("view", view);

  // set lighting uniforms

  // glm::vec3 color(0.7, 0.3, 0.68);
  glm::vec3 color(1, 1, 1);
  for (int i = 0; i < lights_.size() && i < kNumLights; i++) {
    const Light& light = lights_[i];
    shader_->setVec3("lights[" + std::to_string(i) + "].Position",
                     light.Position);
    shader_->setVec3("lights[" + std::to_string(i) + "].Color", light.Color);
    shader_->setFloat("lights[" + std::to_string(i) + "].Linear", light.Linear);
    shader_->setFloat("lights[" + std::to_string(i) + "].Quadratic",
                      light.Quadratic);
  }

  shader_->setVec3("viewPos", camera_.Position);
  {
    glm::mat4 model_mat;
    for (int i = 0; i < static_models_.size(); i++) {
      static_models_[i]->Draw({shader_.get()}, static_model_matrices_[i]);
    }
    for (const std::unique_ptr<DynamicRenderable>& model : dynamic_models_) {
      model->Draw({shader_.get()}, glm::mat4(1.0f));
    }
  }

  light_box_shader_->use();
  light_box_shader_->setMat4("projection", projection);
  light_box_shader_->setMat4("view", view);
  for (int i = 0; i < lights_.size() && i < kNumLights; i++) {
    const Light& light = lights_[i];
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, light.Position);
    model = glm::scale(model, glm::vec3(0.05f));
    light_box_shader_->setMat4("model", model);
    light_box_shader_->setVec3("lightColor", light.Color);
    RenderCube();
  }

  glfwSwapBuffers(window_);
  glfwPollEvents();
}

bool MultiLightRenderer::WindowShouldClose() {
  return glfwWindowShouldClose(window_);
}

bool MultiLightRenderer::AddLight(const Light& light) {
  if (lights_.size() < kNumLights) {
    lights_.push_back(light);
    return true;
  }
  return false;
}

int MultiLightRenderer::MaxNumLights() const { return kNumLights; }

void MultiLightRenderer::processInput(float deltaTime) {
  if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window_, true);

  if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS)
    camera_.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS)
    camera_.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS)
    camera_.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS)
    camera_.ProcessKeyboard(RIGHT, deltaTime);
  if (glfwGetKey(window_, GLFW_KEY_R) == GLFW_PRESS)
    camera_.ProcessKeyboard(UP, deltaTime);
  if (glfwGetKey(window_, GLFW_KEY_F) == GLFW_PRESS)
    camera_.ProcessKeyboard(DOWN, deltaTime);
  if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS)
    camera_.ProcessKeyboard(FORWARD, deltaTime);

  if (KeyNewlyPressed(window_, &key_states_, GLFW_KEY_P)) {
    pause_ = !pause_;
  }

  for (CameraEventHandler* handler : event_handlers_) {
    handler->KeyboardEvents(window_);
  }
}
