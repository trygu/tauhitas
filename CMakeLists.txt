# Tau-Argus HiTaS: hierarchical table suppression (modular) and FullJJ (optimal).
#
# Calls into the CSP engine (native/csp), which is built against the open-source
# HiGHS LP solver. The historical CPLEX/XPRESS code paths have been removed;
# every solver name is dispatched to the single "SCIP" (HiGHS-backed) CSP API.
#
# The legacy SWIG JNI wrapper (HiTaSCtrl_wrap) and the CPLEX/XPRESS solver
# headers have been removed; bindings to the new stack are generated with
# pybind11 (see python/cpp/bind_hitas.cpp in the top-level repo).

cmake_minimum_required(VERSION 3.16)

project(tauargus_hitas CXX)

# Embed the legacy error strings (ErrorStrings.txt, lines "NAME = code = text")
# at build time. The legacy GUI loaded the file from the working directory at
# runtime; the headless engine has no working directory to rely on, so the
# table is generated into the binary directory and compiled in.
set(HITAS_ERRORSTRINGS_TXT ${CMAKE_CURRENT_SOURCE_DIR}/ErrorStrings.txt)
set(HITAS_ERRORSTRINGS_INC ${CMAKE_CURRENT_BINARY_DIR}/ErrorStrings_generated.inc)
file(READ ${HITAS_ERRORSTRINGS_TXT} _hitas_es_raw)
string(REGEX REPLACE "\r\n" "\n" _hitas_es_raw "${_hitas_es_raw}")
string(REGEX MATCHALL "[^\n]+" _hitas_es_lines "${_hitas_es_raw}")
file(WRITE ${HITAS_ERRORSTRINGS_INC}
     "// Auto-generated from ErrorStrings.txt at configure time. Do not edit.\n"
     "struct EmbeddedErrorString { int code; const char* text; };\n"
     "static const EmbeddedErrorString EmbeddedErrorStrings[] = {\n")
foreach(_hitas_es_line IN LISTS _hitas_es_lines)
    if(_hitas_es_line MATCHES "^([A-Za-z0-9_]+) *= *([0-9]+) *= *(.+)")
        file(APPEND ${HITAS_ERRORSTRINGS_INC} "    {${CMAKE_MATCH_2}, \"${CMAKE_MATCH_3}\"},\n")
    endif()
endforeach()
file(APPEND ${HITAS_ERRORSTRINGS_INC} "};\n")

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
endif()

add_library(tauargus_hitas SHARED ${HITAS_SRC})
add_library(tauargus::hitas ALIAS tauargus_hitas)

target_include_directories(tauargus_hitas
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}
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
