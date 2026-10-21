cmake_minimum_required(VERSION 3.16)
project(z80_recomp_core LANGUAGES C)

# Header-only state, host ABI, and verified instruction semantics. Generated
# code links no platform implementation from this repository: each consumer
# supplies its own memory, I/O, scheduler, and fallback hooks.
add_library(z80_recomp_core INTERFACE)
add_library(z80-recomp-core::core ALIAS z80_recomp_core)
target_include_directories(z80_recomp_core INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/include")

include(CTest)
if(BUILD_TESTING)
    add_executable(z80_recomp_core_header_smoke tests/header_smoke.c)
    target_link_libraries(z80_recomp_core_header_smoke PRIVATE
        z80-recomp-core::core)
    add_test(NAME z80_recomp_core_header_smoke
             COMMAND z80_recomp_core_header_smoke)
endif()

