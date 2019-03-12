if (UNIX)
  SET(IRRKLANG_INCLUDE_PATH ${CMAKE_CURRENT_BINARY_DIR}/irrKlangLib/src/irrKlangLib/include)
  SET(IRRKLANG_LIBRARY_PATH ${CMAKE_CURRENT_BINARY_DIR}/irrKlangLib/src/irrKlangLib/bin/linux-gcc-64)
  find_path(IRRKLANG_INCLUDE_DIR irrKlang.h ${IRRKLANG_INCLUDE_PATH})
  file(GLOB IRRKLANG_LIBRARY_DIR ${IRRKLANG_LIBRARY_PATH}/libIrrKlang.so*)
  if (NOT IRRKLANG_INCLUDE_DIR OR NOT IRRKLANG_LIBRARY_DIR)
    message ("Cannot find irrKlang")
    include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)
    ExternalProject_Add( irrKlangLib
      URL http://www.ambiera.at/downloads/irrKlang-64bit-1.6.0.zip
      PREFIX irrKlangLib
      CONFIGURE_COMMAND ""
      BUILD_COMMAND ""
      INSTALL_COMMAND ""
      )
    SET(IRRKLANG_INCLUDE_DIR ${IRRKLANG_INCLUDE_PATH})
    SET(IRRKLANG_LIBRARY_DIR ${IRRKLANG_LIBRARY_PATH}/libIrrKlang.so)
  endif()
endif()

if (WIN32)
  message("GetIrrklang WIN32")
  message("CMAKE_CURRENT_BINARY_DIR => ${CMAKE_CURRENT_BINARY_DIR}")
  message("CMAKE_SOURCE_DIR => ${CMAKE_SOURCE_DIR}")
  SET(IRRKLANG_INCLUDE_PATH ${CMAKE_SOURCE_DIR}/irrKlangLib/src/irrKlangLib/irrKlang-1.6.0/include)
  find_path(IRRKLANG_INCLUDE_DIR irrKlang.h ${IRRKLANG_INCLUDE_PATH})
  file(GLOB IRRKLANG_LIBRARY_DIR C:\\Program Files\\irrKlang.lib)
  if (NOT IRRKLANG_INCLUDE_DIR OR NOT IRRKLANG_LIBRARY_DIR)
    message ("Cannot find irrKlang")
    include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)
    ExternalProject_Add( irrKlangLib
      URL http://www.ambiera.at/downloads/irrKlang-32bit-1.6.0.zip
      PREFIX ${CMAKE_SOURCE_DIR}/irrKlangLib
      CONFIGURE_COMMAND ""
      BUILD_COMMAND ""
      INSTALL_COMMAND ""
      )
    SET(IRRKLANG_INCLUDE_DIR ${IRRKLANG_INCLUDE_PATH})
    SET(IRRKLANG_LIBRARY_DIR "C:\\Program Files\\irrKlang.lib")

  endif()
endif()
