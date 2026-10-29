FILE(GLOB_RECURSE app_sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.*)

idf_component_register(SRCS ${app_sources})

target_compile_options(${COMPONENT_LIB} PRIVATE
    -Wall
    -Wextra
    -Wformat
    -Wformat=2
    -Wconversion
    -Wimplicit-fallthrough
    -Wshadow
    -Werror
)
