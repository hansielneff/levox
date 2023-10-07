include(FetchContent)
set(BUILD_SHARED_LIBS OFF)

message(STATUS "Fetching SFML...")
FetchContent_Declare(
  sfml
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 11b73743c42cf7ecd7c596ba83fdbf1150ffa96c # 2.6.0
)
add_subdirectory(sfml)

message(STATUS "Fetching ImGui...")
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG c6e0284ac58b3f205c95365478888f7b53b077e2 # v1.89.9
)
FetchContent_MakeAvailable(imgui)

message(STATUS "Fetching ImGui-SFML...")
include(FetchContent)
FetchContent_Declare(
  imgui-sfml
  GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
  GIT_TAG de565ac8f2b795dedc0307b60830cb006afd2ecd # v2.6
)
add_subdirectory(imgui-sfml)

message(STATUS "Fetching GLM...")
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG bf71a834948186f4097caa076cd2663c69a10e1e # 0.9.9.8
)
FetchContent_MakeAvailable(glm)
