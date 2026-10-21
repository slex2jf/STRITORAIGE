set(M68K_RECOMP_CORE_ROOT "${CMAKE_CURRENT_LIST_DIR}/..")

function(m68k_recomp_core_sources out_var profile)
    if(profile STREQUAL "genesis")
        set(profile_dir "${M68K_RECOMP_CORE_ROOT}/profiles/genesis")
        set(profile_extra_sources
            "${profile_dir}/return_capture.c")
    elseif(profile STREQUAL "scc68070")
        set(profile_dir "${M68K_RECOMP_CORE_ROOT}/profiles/scc68070")
        set(profile_extra_sources)
    else()
        message(FATAL_ERROR
            "Unknown m68k-recomp-core profile '${profile}'; expected genesis or scc68070")
    endif()

    set(sources
        "${M68K_RECOMP_CORE_ROOT}/common/m68k_decoder.c"
        "${M68K_RECOMP_CORE_ROOT}/common/m68k_validator.c"
        "${M68K_RECOMP_CORE_ROOT}/common/annotations.c"
        ${profile_extra_sources}
        "${profile_dir}/function_finder.c"
        "${profile_dir}/code_generator.c"
        "${profile_dir}/codegen_diag.c")
    set(${out_var} "${sources}" PARENT_SCOPE)
endfunction()

function(m68k_recomp_core_include_dirs out_var profile)
    if(NOT profile STREQUAL "genesis" AND NOT profile STREQUAL "scc68070")
        message(FATAL_ERROR
            "Unknown m68k-recomp-core profile '${profile}'; expected genesis or scc68070")
    endif()
    set(${out_var}
        "${M68K_RECOMP_CORE_ROOT}/common"
        "${M68K_RECOMP_CORE_ROOT}/profiles/${profile}"
        PARENT_SCOPE)
endfunction()
