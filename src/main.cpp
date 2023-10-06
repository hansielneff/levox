#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#include <iostream>

#include "glad/gl.h"
#include "glad/egl.h"
#ifdef X11_FOUND
    #include "glad/glx.h"
#endif
#include "types.h"
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
    window.pushGLStates();
    ImGui::SFML::Update(window, delta);
    ImGui::ShowDemoWindow();
    ImGui::SFML::Render(window);
    window.popGLStates();
}

static u32 compileShader(GLenum type, const char *source)
{
    u32 shader = glCreateShader(type);
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
    u32 vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    u32 fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
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

static u32 initTriangleVAO()
{
    f32 vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    u32 VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    u32 VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(f32), NULL);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return VAO;
}

static void renderTriangle(u32 shaderProgram, u32 VAO)
{
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

static void renderScene(sf::RenderWindow &window)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    u32 shaderProgram = linkShaderProgram();
    u32 triangleVAO = initTriangleVAO();
    renderTriangle(shaderProgram, triangleVAO);
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

    sf::Clock deltaClock;
    bool isAppRunning = true;
    while (isAppRunning)
    {
        handleWindowEvents(window, isAppRunning);
        renderScene(window);
        renderImGui(window, deltaClock.restart());
        window.display();
    }

    window.setActive(false);
    ImGui::SFML::Shutdown();
    return 0;
}
