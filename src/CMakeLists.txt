# Enable Hot Reload for MSVC compilers if supported
if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Enable ccache on systems that provide it
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif(CCACHE_FOUND)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(Levox)
target_sources(Levox
  PRIVATE
    glad/gl.c
    glad/egl.c
    nfd/nfd_common.c
    main.cpp
    camera.hpp
    camera.cpp
    shader.hpp
    shader.cpp
    voxel_array.hpp
    voxel_array.cpp
    voxel_mesh.hpp
    voxel_mesh.cpp
    event_handler.hpp
    event_handler.cpp
    toolbox.hpp
    toolbox.cpp
)

if(WIN32)
  target_sources(Levox PRIVATE nfd/nfd_win.cpp)
elseif(APPLE)
  target_sources(Levox PRIVATE nfd/nfd_cocoa.m)
  target_link_libraries(Levox PRIVATE "-framework AppKit")
elseif(UNIX)
  target_sources(Levox PRIVATE nfd/nfd_gtk.c)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
  include_directories(${GTK3_INCLUDE_DIRS})
  link_directories(${GTK3_LIBRARY_DIRS})
  add_definitions(${GTK3_CFLAGS_OTHER})
endif()

find_package(OpenGL REQUIRED)
find_package(X11)
if(X11_FOUND)
  # Only use GLX on platforms that support/need it
  target_sources(Levox PRIVATE glad/glx.c)
  include_directories(Levox PRIVATE ${X11_INCLUDE_DIR})
  target_link_libraries(Levox PRIVATE ${X11_LIBRARIES})
endif()

target_include_directories(Levox PRIVATE glad/ nfd/include)
target_link_libraries(Levox PRIVATE ImGui-SFML::ImGui-SFML OpenGL::GL glm)
target_compile_features(Levox PRIVATE cxx_std_17)

install(TARGETS Levox)
