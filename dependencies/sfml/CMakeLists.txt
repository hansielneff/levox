message(STATUS "Fetching SFML...")

include(FetchContent)
FetchContent_Declare(
    sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 11b73743c42cf7ecd7c596ba83fdbf1150ffa96c # 2.6.0
)

set(SFML_BUILD_AUDIO FALSE)
set(SFML_BUILD_NETWORK FALSE)

FetchContent_MakeAvailable(sfml)
