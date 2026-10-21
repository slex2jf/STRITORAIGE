cmake_minimum_required(VERSION 3.19)

project(SOR1Case4Playable C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(RECOMP_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/framework")
set(CASE4_GENERATED "${CMAKE_CURRENT_SOURCE_DIR}/generated")

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)

include("${RECOMP_ROOT}/cmake/GenesisRecompRunner.cmake")

genesisrecomp_runner_sources(
    SOR1_RUNNER
    TRACE OFF
    REVERSE_DEBUG OFF
)

file(GLOB SOR1_PARTS
    "${CASE4_GENERATED}/sor1_part*.c"
)

list(SORT SOR1_PARTS)

set(SOR1_GENERATED
    ${SOR1_PARTS}
    "${CASE4_GENERATED}/sor1_dispatch.c"
    "${CASE4_GENERATED}/sor1_layout.c"
)

add_executable(sor1_visual_case4
    ${SOR1_RUNNER}
    ${SOR1_GENERATED}
    "${CASE4_GENERATED}/sor1_cycles.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/game/sor1_spec.c"
)

target_include_directories(sor1_visual_case4 PRIVATE
    "${RECOMP_ROOT}/runner"
    "${RECOMP_ROOT}/runner/include"
    "${RECOMP_ROOT}/runner/audio"
    "${RECOMP_ROOT}/runner/video"
    "${RECOMP_ROOT}/runner/external"
    "${RECOMP_ROOT}/runner/external/clowncommon"
    "${RECOMP_ROOT}/runner/external/ymfm/src"
    "${RECOMP_ROOT}/runner/external/superzazu"
    "${RECOMP_ROOT}/recompiler/src"
    "${CASE4_GENERATED}"
)

target_link_libraries(sor1_visual_case4 PRIVATE
    PkgConfig::SDL2
    m
)

genesisrecomp_runner_target(sor1_visual_case4)

# O RETURN04 substituía esses dois arquivos oficiais por seu runtime híbrido.
get_target_property(diag_sources sor1_visual_case4 SOURCES)

list(REMOVE_ITEM diag_sources
    "${RECOMP_ROOT}/runner/main.c"
    "${RECOMP_ROOT}/runner/glue.c"
    "${RECOMP_ROOT}/runner/m68k_interp.c"
)

set_property(
    TARGET sor1_visual_case4
    PROPERTY SOURCES "${diag_sources}"
)

target_sources(sor1_visual_case4 PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/runtime/glue.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/runtime/m68k_interp.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/diag.c"
)

target_include_directories(sor1_visual_case4 PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/runtime"
    "${RECOMP_ROOT}/runner/video"
    "${RECOMP_ROOT}/runner"
)

target_sources(sor1_visual_case4 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/main.c" "${CMAKE_CURRENT_SOURCE_DIR}/src/profile.c")
target_include_directories(sor1_visual_case4 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

target_sources(sor1_visual_case4 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/state_hash.c")
target_include_directories(sor1_visual_case4 BEFORE PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/runtime/include")

target_include_directories(sor1_visual_case4 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
