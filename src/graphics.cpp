#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glad/gl.h"
#include "glad/egl.h"
#ifdef X11_FOUND
    #include "glad/glx.h"
#endif

#include <exception>

#include "graphics.hpp"
#include "shader.hpp"
#include "shader_source.hpp"
#include "voxel_mesh.hpp"
#include "test_model.hpp"

void initOpenGLContext()
{
    if (!gladLoaderLoadGL())
        throw std::runtime_error("Error: Failed to load OpenGL extension libraries");
    glEnable(GL_DEPTH_TEST);
}

void renderModel(sf::RenderWindow &window, Camera &camera)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Shader shader(vertexShaderSource, fragmentShaderSource);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = camera.getProjectionMatrix();

    glUseProgram(shader.ID);

    u32 modelLoc = glGetUniformLocation(shader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    u32 viewLoc = glGetUniformLocation(shader.ID, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    u32 projectionLoc = glGetUniformLocation(shader.ID, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    VoxelMesh voxelMesh((VoxelData*)testModel, 4, 4, 4);
    voxelMesh.render();
}
