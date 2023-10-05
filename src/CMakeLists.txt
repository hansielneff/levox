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

find_package(OpenGL REQUIRED)

add_executable(Levox)
target_sources(Levox
    PRIVATE
        main.cpp
)

target_link_libraries(Levox PRIVATE ImGui-SFML::ImGui-SFML OpenGL::GL)
target_compile_features(Levox PRIVATE cxx_std_17)

# Copy DLLs needed for runtime on Windows
if(WIN32)
    add_custom_command(
        TARGET Levox
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:Levox>
        VERBATIM)
    add_custom_command(
        TARGET Levox
        COMMENT "Copy SFML and ImGui DLLs"
        POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:ImGui-SFML::ImGui-SFML>
            $<TARGET_FILE:sfml-graphics>
            $<TARGET_FILE:sfml-window>
            $<TARGET_FILE:sfml-system>
            $<TARGET_FILE_DIR:Levox>)
endif()

install(TARGETS Levox)