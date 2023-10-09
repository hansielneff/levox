#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glad/gl.h"
#include "glad/egl.h"
#ifdef X11_FOUND
    #include "glad/glx.h"
#endif

#include <iostream>

#include "types.h"
#include "camera.h"
#include "shader_source.h"

static void printContextSettings(sf::Window &window)
{
    auto settings = window.getSettings();
    std::cout << "depth bits:" << settings.depthBits << std::endl;
    std::cout << "stencil bits:" << settings.stencilBits << std::endl;
    std::cout << "antialiasing level:" << settings.antialiasingLevel << std::endl;
    std::cout << "version:" << settings.majorVersion << "." << settings.minorVersion << std::endl;
}

static void handleWindowEvents(sf::Window &window, bool &isAppRunning)
{
    for (auto event = sf::Event(); window.pollEvent(event);)
    {
        ImGui::SFML::ProcessEvent(event);
        switch (event.type)
        {
            case sf::Event::Closed:
                isAppRunning = false;
                break;
            case sf::Event::Resized:
                glViewport(0, 0, event.size.width, event.size.height);
                break;
            default:
                break;
        }
    }
}

static void renderImGui(sf::RenderWindow &window, sf::Time delta)
{
    // TODO: These push/pop operations are rather expensive to do every render cycle,
    // so find/create an OpenGL backend for ImGui instead of the imgui-sfml library.
    // Once this is done, change the SFML window back to a regular sf::Window.

    // ImGui relies on SFML's graphics module, which might change OpenGL state,
    // so we push and pop our state to preserve it.
    window.pushGLStates();
    ImGui::SFML::Update(window, delta);
    ImGui::ShowDemoWindow();
    ImGui::SFML::Render(window);
    window.popGLStates();
}

static u32 compileShaderSource(GLenum shaderType, const char *source)
{
    u32 shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        u32 const logSize = 512;
        char infoLog[logSize];
        glGetShaderInfoLog(shader, logSize, NULL, infoLog);
        std::cout << "Error: Failed to compile shader\n" << infoLog << std::endl;
    }

    return shader;
}

static u32 linkShaderProgram()
{
    u32 vertexShader = compileShaderSource(GL_VERTEX_SHADER, vertexShaderSource);
    u32 fragmentShader = compileShaderSource(GL_FRAGMENT_SHADER, fragmentShaderSource);
    u32 shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        u32 const logSize = 512;
        char infoLog[logSize];
        glGetProgramInfoLog(shaderProgram, logSize, NULL, infoLog);
        std::cout << "Error: Failed to link shader program\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

static u32 initQuadVAO()
{
    f32 vertices[] = {
        0.5f,  0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };

    u32 indices[] = {
        0, 1, 3,
        1, 2, 3
    };  

    u32 VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    u32 EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    u32 VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(f32), NULL);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return VAO;
}

static void renderScene(sf::RenderWindow &window, Camera &camera)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    u32 shaderProgram = linkShaderProgram();
    u32 VAO = initQuadVAO();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = camera.getProjectionMatrix(ProjectionMode::Perspective);

    glUseProgram(shaderProgram);

    u32 modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    u32 viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    u32 projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

i32 main()
{
    sf::RenderWindow window({1280, 720}, "Levox",
        sf::Style::Default, sf::ContextSettings(24, 8, 4, 3, 3));
    window.setActive();
    printContextSettings(window);

    if (!gladLoaderLoadGL())
    {
        std::cerr << "Failed to load OpenGL extension libraries" << std::endl;
        return 1;
    }

    if (!ImGui::SFML::Init(window))
    {
        std::cerr << "Failed to initialize ImGui" << std::endl;
        return 1;
    }

    glEnable(GL_DEPTH_TEST);

    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
    sf::Clock deltaClock;
    sf::Time deltaTime = sf::Time::Zero;
    bool isAppRunning = true;
    while (isAppRunning)
    {
        // This is a temporary input handling system for testing
        // the Camera class.
        bool right = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
        bool left = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
        bool up = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
        bool down = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
        f32 xOffset = (right - left) * deltaTime.asSeconds();
        f32 yOffset = (up - down) * deltaTime.asSeconds();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            camera.pan(xOffset, yOffset);
        else
            camera.orbit(xOffset * 10, yOffset * 10);

        handleWindowEvents(window, isAppRunning);
        renderScene(window, camera);
        renderImGui(window, deltaTime);
        window.display();
        deltaTime = deltaClock.restart();
    }

    window.setActive(false);
    ImGui::SFML::Shutdown();
    return 0;
}
