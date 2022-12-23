##==================================================================================================
##  TTS - Tiny Test System
##  Copyright : TTS Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##==================================================================================================
include(GNUInstallDirs)
set(MAIN_DEST     "${CMAKE_INSTALL_LIBDIR}/tts-${PROJECT_VERSION}")
set(INSTALL_DEST  "${CMAKE_INSTALL_INCLUDEDIR}/tts-${PROJECT_VERSION}")
set(DOC_DEST      "${CMAKE_INSTALL_DOCDIR}-${PROJECT_VERSION}")

## =================================================================================================
## Exporting target for external use
## =================================================================================================
add_library(tts_lib INTERFACE)
target_include_directories( tts_lib INTERFACE
                            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                            $<INSTALL_INTERFACE:${INSTALL_DEST}>
                          )
target_compile_features(tts_lib INTERFACE cxx_std_20)
set_target_properties(tts_lib PROPERTIES EXPORT_NAME tts)
add_library(tts::tts ALIAS tts_lib)

## =================================================================================================
## Install target with versioned folder
## =================================================================================================
install(TARGETS   tts_lib EXPORT tts-targets DESTINATION "${MAIN_DEST}")
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/tts               DESTINATION "${INSTALL_DEST}" )
install(FILES     ${PROJECT_SOURCE_DIR}/cmake/tts-config.cmake    DESTINATION "${MAIN_DEST}"    )
install(FILES     ${PROJECT_SOURCE_DIR}/LICENSE.md                DESTINATION "${DOC_DEST}"     )
install(EXPORT    tts-targets NAMESPACE "tts::" DESTINATION "${MAIN_DEST}")
