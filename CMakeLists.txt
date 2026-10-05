# Tau-Argus HiTaS: hierarchical table suppression (modular) and FullJJ (optimal).
#
# Calls into the CSP engine (native/csp), which is built against the open-source
# HiGHS LP solver. The historical CPLEX/XPRESS code paths have been removed;
# every solver name is dispatched to the single "SCIP" (HiGHS-backed) CSP API.
#
# The SWIG JNI wrapper (HiTaSCtrl_wrap) is intentionally excluded; bindings to
# the new stack are generated with pybind11 instead of SWIG.

cmake_minimum_required(VERSION 3.16)

project(tauargus_hitas CXX)

set(HITAS_SRC
    src/ALList.cpp
    src/AMiscFunc.cpp
    src/AMyLoadProb.cpp
    src/ATabs.cpp
    src/Adefs.cpp
    src/Ahier.cpp
    src/Amyio.cpp
    src/HiTaSCtrl.cpp
    src/WrapCSP.cpp
)

if(WIN32)
    # Compile the Windows version resource (harmless no-op elsewhere).
    list(APPEND HITAS_SRC src/Versioninfo.rc)
    set_source_files_properties(src/Versioninfo.rc PROPERTIES LANGUAGE C)
endif()

add_library(tauargus_hitas SHARED ${HITAS_SRC})
add_library(tauargus::hitas ALIAS tauargus_hitas)

target_include_directories(tauargus_hitas
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
)

# The original Makefile built with -DSECBOUNDS (secondary bounds enabled).
target_compile_definitions(tauargus_hitas PRIVATE SECBOUNDS)

target_link_libraries(tauargus_hitas PUBLIC tauargus::csp)

target_compile_features(tauargus_hitas PUBLIC cxx_std_11)

target_compile_options(tauargus_hitas PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall -fno-strict-aliasing>
)

if(WIN32)
    target_compile_definitions(tauargus_hitas PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

install(TARGETS tauargus_hitas
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
install(DIRECTORY src/
    DESTINATION include/tauargus_hitas
    FILES_MATCHING PATTERN "*.h"
)
