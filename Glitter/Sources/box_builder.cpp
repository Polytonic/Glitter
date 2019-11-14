#include "box_builder.hpp"

#include <vector>

#inclue "learnopengl/mesh.h"

namespace {

  std::vector<Vertex> GetCubeVertices(){
    std::vector<Vertex> verts;
    // back face
    verts.push_back({{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}, glm::vec3(), glm::vec3()});  // bottom-left
    verts.push_back({{1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}, glm::vec3(), glm::vec3()});  // top-right
    verts.push_back({{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f   }, glm::vec3(), glm::vec3()});  // bottom-right
    verts.push_back({{1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f    }, glm::vec3(), glm::vec3()});  // top-right
    verts.push_back({{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f  }, glm::vec3(), glm::vec3()});  // bottom-left
    verts.push_back({{-1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f   }, glm::vec3(), glm::vec3()});  // top-left
 // front face
    verts.push_back({{-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f  }, glm::vec3(), glm::vec3()});  // bottom-left
    verts.push_back({{1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f   }, glm::vec3(), glm::vec3()});  // bottom-right
    verts.push_back({{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f    }, glm::vec3(), glm::vec3()});  // top-right
    verts.push_back({{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f    }, glm::vec3(), glm::vec3()});  // top-right
    verts.push_back({{-1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f   }, glm::vec3(), glm::vec3()});  // top-left
    verts.push_back({{-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f  }, glm::vec3(), glm::vec3()});  // bottom-left
 // left face
    verts.push_back({{-1.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f    }, glm::vec3(), glm::vec3()});  // top-right
    verts.push_back({{-1.0f, 1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f   }, glm::vec3(), glm::vec3()});  // top-left
    verts.push_back({{-1.0f, -1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f  }, glm::vec3(), glm::vec3()});  // bottom-left
    verts.push_back({{-1.0f, -1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f  }, glm::vec3(), glm::vec3()});  // bottom-left
    verts.push_back({{-1.0f, -1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f   }, glm::vec3(), glm::vec3()});  // bottom-right
    verts.push_back({{-1.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f    }, glm::vec3(), glm::vec3()});  // top-right
 // right face
    verts.push_back({{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f    }, glm::vec3(), glm::vec3()});  // top-left
    verts.push_back({{1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f  }, glm::vec3(), glm::vec3()});  // bottom-right
    verts.push_back({{1.0f, 1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f   }, glm::vec3(), glm::vec3()});  // top-right
    verts.push_back({{1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f  }, glm::vec3(), glm::vec3()});  // bottom-right
    verts.push_back({{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f    }, glm::vec3(), glm::vec3()});  // top-left
    verts.push_back({{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f   }, glm::vec3(), glm::vec3()});  // bottom-left
 // bottom face
    verts.push_back({{-1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f  }, glm::vec3(), glm::vec3()});  // top-right
    verts.push_back({{1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f   }, glm::vec3(), glm::vec3()});  // top-left
    verts.push_back({{1.0f, -1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f    }, glm::vec3(), glm::vec3()});  // bottom-left
    verts.push_back({{1.0f, -1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f    }, glm::vec3(), glm::vec3()});  // bottom-left
    verts.push_back({{-1.0f, -1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f   }, glm::vec3(), glm::vec3()});  // bottom-right
    verts.push_back({{-1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f  }, glm::vec3(), glm::vec3()});  // top-right
 // top face
    verts.push_back({{-1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f  }, glm::vec3(), glm::vec3()});  // top-left
    verts.push_back({{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f    }, glm::vec3(), glm::vec3()});  // bottom-right
    verts.push_back({{1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f   }, glm::vec3(), glm::vec3()});  // top-right
    verts.push_back({{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f    }, glm::vec3(), glm::vec3()});  // bottom-right
    verts.push_back({{-1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f  }, glm::vec3(), glm::vec3()});  // top-left
    verts.push_back({{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f    }, glm::vec3(), glm::vec3()});  // bottom-left
  }
  
}

Model BuildBoxModel(){
  
}
